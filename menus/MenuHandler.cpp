
#include "MenuHandler.h"

#include "../filehandle.h"
#include "../SDL-Drawing-Library/DrawingContext.h"
#include "../emulator/cpu.h"
#include "../utils.h"

EntryEffect::EntryEffect():
    EntryEffect(EFFECT_NONE, 0)
{

};
EntryEffect::EntryEffect(EntryEffectType effect_type, int effect_arg):
    m_effect_type{ effect_type },
    m_effect_arg{ effect_arg }
{

};
EntryEffectType EntryEffect::getType() { return m_effect_type; };
int EntryEffect::getArg() { return m_effect_arg; };


Menu::Menu():
    Menu("", {}, {})
{

};
Menu::Menu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects): 
    m_menu_title{ menu_title },
    m_entries{ entries },
    m_entry_effects{ entry_effects },
    m_entry_selected{ 0 },
    m_entry_colors{ std::vector<EntryColor>(entries.size()) }
{
    for (int i = 0; i < static_cast<int>(m_entries.size()); i++) {
        m_entry_colors[i] = COLOR_BLACK;
    }
};

void Menu::drawSelf(DrawingContext* ctx, bool bind_next_key) {
    if (!bind_next_key) {
        ctx->background(200, 200, 200);
    }else {
        ctx->background(100, 100, 100);
    }

    int base_w = 600;
    int base_h = 600;

    int full_x = ctx->getScreenWidth();
    int full_y = ctx->getScreenHeight();
    int base_x = (full_x - base_w) / 2;
    int base_y = (full_y - base_h) / 2;

    ctx->fill(50, 50, 50);
    ctx->rectOutline(base_x, base_y, base_w, base_h);

    ctx->fill(50, 50, 50);
    ctx->textSize(32);
    ctx->text(m_menu_title, base_x+20, base_y+20);

    ctx->textSize(32);
    for (int i = 0; i < static_cast<int>(m_entries.size()); i++) {
        if (i == m_entry_selected) {
            ctx->fill(160, 160, 160);
            ctx->rect(base_x+70, base_y+75+(i*45), 20+ctx->measureTextWidth(m_entries[i]), 42);
        }
        switch (m_entry_colors[i]) {
            case COLOR_BLACK:
                ctx->fill(50, 50, 50);
                break;
            case COLOR_ORANGE:
                ctx->fill(200, 100, 50);
                break;
            case COLOR_RED:
                ctx->fill(200, 50, 50);
                break;
        }

        ctx->text(m_entries[i], base_x+80, base_y+80+(i*45));
    }

    // ctx->presentRenderer();
};
void Menu::triggerScrollEvent(int direction) {
    if (direction > 0) {
        m_entry_selected += 1;
    }else if (direction < 0) {
        m_entry_selected -= 1;
    }
    int num_entries = static_cast<int>(m_entries.size());
    m_entry_selected = (((m_entry_selected % num_entries) + num_entries) % num_entries);
};
EntryEffect Menu::triggerSelectEvent() {
    return m_entry_effects[m_entry_selected];
};
void Menu::setSelected(int select) {
    m_entry_selected = select;
};
void Menu::replaceKeyEntry(int entry, std::string new_key) {
    if (entry == -1) {
        entry = m_entry_selected;
    }
    std::string base = m_entries[entry];
    m_entries[entry] = base.substr(0, base.find_first_of(":") + 1) + " " + new_key;
};
void Menu::setEntries(std::vector<std::string> entries) {
    m_entries = entries;
};
void Menu::setEffects(std::vector<EntryEffect> entry_effects) {
    m_entry_effects = entry_effects;
};
void Menu::setEntryColor(int entry, EntryColor new_color) {
    if (entry == -1) {
        entry = m_entry_selected;
    }
    m_entry_colors[entry] = new_color;
};
void Menu::setColors(std::vector<EntryColor> entry_colors) {
    m_entry_colors = entry_colors;
};


Popup::Popup():
    Popup("", {}, {})
{

};
Popup::Popup(std::string text, std::vector<std::string> buttons, std::vector<EntryEffect> button_effects):
    m_text{ text },
    m_buttons{ buttons },
    m_button_effects{ button_effects },
    m_button_selected{ 0 }
{

};
void Popup::drawSelf(DrawingContext* ctx) {
    int base_w = 600;
    int base_h = 600;

    int full_x = ctx->getScreenWidth();
    int full_y = ctx->getScreenHeight();
    int base_x = (full_x - base_w) / 2;
    int base_y = (full_y - base_h) / 2;

    int this_w = 500;
    int this_h = 300;

    ctx->fill(150, 150, 150);
    ctx->rect(base_x + 50, base_y + 150, this_w, this_h);
    ctx->fill(50, 50, 50);
    ctx->rectOutline(base_x + 50, base_y + 150, this_w, this_h);

    ctx->textSize(32);
    ctx->text(m_text, base_x + 70, base_y + 170, 460, 40);

    int num_buttons = static_cast<int>(m_buttons.size());
    int interval = (this_w-40-(20 * (num_buttons-1)))/num_buttons;
    for (int i = 0; i < num_buttons; i++) {
        if (i == m_button_selected) {
            ctx->fill(200, 200, 200);
            ctx->rect((base_x+50)+20+((interval+20)*i), (base_y+150)+240, interval, 40);
            ctx->fill(50, 50, 50);
        }

        ctx->text(m_buttons[i], (base_x+50)+20+((interval+20)*i) + 20, (base_y+150)+240 + 5);
    }

    // ctx->presentRenderer();
};
void Popup::triggerScrollEvent(int direction) {
    if (direction > 0) {
        m_button_selected += 1;
    }else if (direction < 0) {
        m_button_selected -= 1;
    }
    int num_buttons = static_cast<int>(m_buttons.size());
    m_button_selected = (((m_button_selected % num_buttons) + num_buttons) % num_buttons);
};
EntryEffect Popup::triggerSelectEvent() {
    return m_button_effects[m_button_selected];
};
void Popup::setSelected(int select) {
    m_button_selected = select;
};


Menu_Handler::Menu_Handler(DrawingContext* ctx, std::vector<std::string>* recent_games, std::vector<uint32_t>* keybindings, std::vector<uint32_t>* temp_keybindings, Emulator_Options* options):
    m_menus{ },
    m_popup{ Popup() },
    m_popup_active{ false },
    m_menu_selected{ 0 },
    m_ctx{ ctx },
    m_cpu{ nullptr },
    m_recent_games{ recent_games },
    m_keybindings{ keybindings },
    m_temp_keybindings{ temp_keybindings },
    m_options{ options },
    m_bind_next_key{ false },
    m_bind_to_key{ 0 }
{

};
Menu_Handler::~Menu_Handler() {
    m_ctx = nullptr; // Don't delete them, that is a job for others
    m_recent_games = nullptr;
};

void Menu_Handler::drawSelf() {
    m_menus[m_menu_selected].drawSelf(getCtx(), m_bind_next_key);

    if (m_popup_active) {
        m_popup.drawSelf(getCtx());
    }
    getCtx()->presentRenderer();
};
int Menu_Handler::addMenu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects) {
    m_menus.push_back(Menu(menu_title, entries, entry_effects));
    return static_cast<int>(m_menus.size()) - 1; // Index of new Menu
};
void Menu_Handler::triggerScrollEvent(int direction) {
    if (!m_popup_active) {
        m_menus[m_menu_selected].triggerScrollEvent(direction);
    }else {
        m_popup.triggerScrollEvent(direction);
    }
};
EntryEffect Menu_Handler::triggerSelectEvent() {
    if (!m_popup_active) {
        return m_menus[m_menu_selected].triggerSelectEvent();
    }else {
        return m_popup.triggerSelectEvent();
    }
};
void Menu_Handler::switchToMenu(int menu) {
    if (menu == -1) {
        menu = m_last_menu;
    }
    m_last_menu = m_menu_selected;
    if (menu < m_menu_selected) { // Assume moving to a master list from a sublist
        m_menus[m_menu_selected].setSelected(0);
    }
    m_menu_selected = menu;
};
bool Menu_Handler::processEvent(SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) { // KeyPress
        if (!m_bind_next_key) {
            if (event->key.key == SDLK_UP || event->key.key == SDLK_RIGHT) { // TODO: Use customized controls as well
                triggerScrollEvent(-1);
            }else if (event->key.key == SDLK_DOWN || event->key.key == SDLK_LEFT) {
                triggerScrollEvent(1);
            }else if (
                event->key.key == SDLK_RETURN || 
                event->key.key == SDLK_SPACE || 
                event->key.key == SDLK_Z || 
                event->key.key == SDLK_X
            ) {
                return interpretMenuEffect(triggerSelectEvent());
            }else {
                if (event->key.key == m_keybindings->at(Key_Up) || event->key.key == m_keybindings->at(Key_Right)) {
                    triggerScrollEvent(-1);
                }else if (event->key.key == m_keybindings->at(Key_Down) || event->key.key == m_keybindings->at(Key_Left)) {
                    triggerScrollEvent(1);
                }else if (
                    event->key.key == m_keybindings->at(Key_A) || 
                    event->key.key == m_keybindings->at(Key_B) || 
                    event->key.key == m_keybindings->at(Key_Start)
                ) {
                    return interpretMenuEffect(triggerSelectEvent());
                }
            }
        }else {
            m_temp_keybindings->at(m_bind_to_key) = event->key.key;
            m_bind_next_key = false;
            m_menus[m_menu_selected].replaceKeyEntry(m_bind_to_key, SDL_GetKeyName(event->key.key));
        }
    }else if (event->type == SDL_EVENT_KEY_UP) { // KeyRelease
        
    }else if (event->type == SDL_EVENT_MOUSE_MOTION) {
        
    }else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        
    }else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        
    }
    return false;
};

bool Menu_Handler::interpretMenuEffect(EntryEffect effect) {
    m_popup_active = false;
    switch (effect.getType()) {
        case EFFECT_NONE:
            break;
        case EFFECT_TO_MENU:
            switchToMenu(effect.getArg());
            break;
        case EFFECT_SELECT_ROM_RELOAD_RECENT: { // Braces because the variable declaration
            std::string filepath = openROMFilePicker(getCtx()->getWindowHandle());
            if (filepath != "") {
                m_rom = readROMFile(filepath);
                if (m_rom != nullptr) {
                    addGameToRecent(filepath);
                    saveRecentGamesFile(m_recent_games);
                    reloadRecentGamesMenu(Recent_Menu);
                    return true; // Time to start emulation
                }else {
                    createPopup(Popup("ROM file could not be loaded.", {"Cancel"}, {EntryEffect(EFFECT_NONE, -1)}));
                }
            } }
            break;
        case EFFECT_SET_KEYBIND:
            m_bind_next_key = true;
            m_bind_to_key = effect.getArg();
            break;
        case EFFECT_FORGET_KEYBIND:
            *m_temp_keybindings = *m_keybindings;
            for (int i = 0; i < 8; i++) {
                m_menus[m_menu_selected].replaceKeyEntry(i, SDL_GetKeyName(m_keybindings->at(i)));
            }
            switchToMenu(effect.getArg());
            break;
        case EFFECT_SAVE_KEYBIND:
            *m_keybindings = *m_temp_keybindings;
            saveKeybindingsFile(m_keybindings);
            switchToMenu(effect.getArg());
            break;
        case EFFECT_TOGGLE:
            switch (effect.getArg()) {
                case TOGGLE_BOOT_ROM:
                    m_options->m_temp_run_boot_rom = !m_options->m_temp_run_boot_rom;
                    m_menus[m_menu_selected].replaceKeyEntry(-1, (m_options->m_temp_run_boot_rom ? "True" : "False"));
                    break;
                case TOGGLE_STRICT_LOADING:
                    m_options->m_temp_strict_loading = !m_options->m_temp_strict_loading;
                    m_menus[m_menu_selected].replaceKeyEntry(-1, (m_options->m_temp_strict_loading ? "True" : "False"));
                    break;
                case TOGGLE_DISPLAY_CART_INFO:
                    m_options->m_temp_display_cart_info = !m_options->m_temp_display_cart_info;
                    m_menus[m_menu_selected].replaceKeyEntry(-1, (m_options->m_temp_display_cart_info ? "True" : "False"));
                    break;
            }
            break;
        case EFFECT_FORGET_OPTIONS:
            m_options->forget_temps();
            m_menus[m_menu_selected].replaceKeyEntry(0, (m_options->m_run_boot_rom ? "True" : "False"));
            switchToMenu(effect.getArg());
            break;
        case EFFECT_SAVE_OPTIONS:
            m_options->save_temps();
            saveOptionsFile(m_options);
            switchToMenu(effect.getArg());
            break;
        case EFFECT_LOAD_RECENT_RELOAD_RECENT: { // Braces because the variable declaration
            std::string filepath = m_recent_games->at(effect.getArg());
            if (filepath != "") {
                m_rom = readROMFile(filepath);
                if (m_rom != nullptr) {
                    addGameToRecent(filepath);
                    saveRecentGamesFile(m_recent_games);
                    reloadRecentGamesMenu(Recent_Menu);
                    return true; // Time to start emulation
                }else {
                    createPopup(Popup("ROM file could not be loaded.", {"Cancel"}, {EntryEffect(EFFECT_NONE, -1)}));
                }
            } }
            break;
        case EFFECT_BACK_TO_EMULATOR:
            return true;
        case EFFECT_RETURN_TO_MAIN:
            createPopup(Popup("Are you sure you want to return to the Main Menu?", {"Cancel", "Confirm"}, {EntryEffect(EFFECT_NONE, -1), EntryEffect(EFFECT_TO_MENU_UNINITIALIZE_EMULATOR, effect.getArg())}));
            break;
        case EFFECT_ASK_LOAD_ANYWAY:
            createPopup(Popup("Are you sure you want to load despite the warning?", {"Cancel", "Confirm"}, {EntryEffect(EFFECT_LOAD_ANYWAY, -1), EntryEffect(EFFECT_TO_MENU_UNINITIALIZE_EMULATOR, effect.getArg())}));
            break;
        case EFFECT_LOAD_ANYWAY:
            return true;
        case EFFECT_TO_MENU_UNINITIALIZE_EMULATOR:
            m_cpu->unInitialize();
            switchToMenu(effect.getArg());
            break;
        case EFFECT_UNINITIALIZE_EMULATOR:
            m_cpu->unInitialize();
            break;
    }
    return false; // Not time to start emulation
};
void Menu_Handler::addGameToRecent(std::string filepath) {
    bool match_found{ false };
    for (int i = 0; i < static_cast<int>(m_recent_games->size()); i++) {
        if (m_recent_games->at(i) == filepath) {
            m_recent_games->erase(m_recent_games->begin() + i);
            match_found = true;
            break;
        }
    }
    if (!match_found) {
        m_recent_games->pop_back();
    }
    m_recent_games->insert(m_recent_games->begin(), filepath);
};

void Menu_Handler::createPopup(Popup popup) {
    m_popup = popup;
    m_popup_active = true;
    printf("COMPLETE: Menu Handler: Popup Created\n");
};
std::vector<uint8_t>* Menu_Handler::getROM() {
    return m_rom;
};
DrawingContext* Menu_Handler::getCtx() {
    return m_ctx;
};

void Menu_Handler::replaceKeyEntriesOnMenu(std::vector<std::string> new_keys, int menu) {
    for (int i = 0; i < static_cast<int>(new_keys.size()); i++) {
        m_menus[menu].replaceKeyEntry(i, new_keys[i]);
    }
};
void Menu_Handler::setColorsOnMenu(std::vector<EntryColor> new_colors, int menu) {
    for (int i = 0; i < static_cast<int>(new_colors.size()); i++) {
        m_menus[menu].setEntryColor(i, new_colors[i]);
    }
};
void Menu_Handler::reloadRecentGamesMenu(int menu) {
    std::vector<std::string> recent_entries{ std::vector<std::string>() }; // For Recent Menu
    std::vector<EntryEffect> recent_effects{ std::vector<EntryEffect>() };
    for (int i = 0; i < static_cast<int>(m_recent_games->size()); i++) {
        if (m_recent_games->at(i) != "") {
            recent_entries.push_back(trimPathAndLength(m_recent_games->at(i), 40));
            recent_effects.push_back(EntryEffect(EFFECT_LOAD_RECENT_RELOAD_RECENT, i));
        }
    }
    recent_entries.push_back("Back");
    recent_effects.push_back(EntryEffect(EFFECT_TO_MENU, Main_Menu));

    m_menus[menu].setSelected(0);
    m_menus[menu].setEntries(recent_entries);
    m_menus[menu].setEffects(recent_effects);
    printf("Menu '%i' Modified", menu);
};

void Menu_Handler::setCPU(DMG_CPU* cpu) {
    m_cpu = cpu;
};
