
#include "MenuHandler.h"

#include "../filehandle.h"
#include "../SDL-Drawing-Library/DrawingContext.h"
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
    m_entry_selected{ 0 }
{

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
            // ctx->rect(base_x+70, base_y+75+(i*45), 300, 42);
            // int width = 20+ctx->measureTextWidth(m_entries[i]);
            ctx->rect(base_x+70, base_y+75+(i*45), 20+ctx->measureTextWidth(m_entries[i]), 42);

            // if (bind_next_key) {
            //     ctx->fill(50, 50, 50, 125);
            //     ctx->rect(0, 0, base_x+70, full_y);
            //     ctx->rect(base_x+70 + width, 0, full_x - (base_x+70 + width), full_y);
            //     ctx->rect(base_x+70, 0, width, base_y+75+(i*45));
            //     ctx->rect(base_x+70, base_y+75+(i*45) + 42, width, full_y - (base_y+75+(i*45) + 42));
            // }
            ctx->fill(50, 50, 50);
        }
        ctx->text(m_entries[i], base_x+80, base_y+80+(i*45));
    }

    ctx->presentRenderer();
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
    std::string base = m_entries[entry];
    m_entries[entry] = base.substr(0, base.find_first_of(":") + 1) + " " + new_key;
};


Menu_Handler::Menu_Handler(DrawingContext* ctx, std::vector<std::string>* recent_games, std::vector<uint32_t>* keybindings, std::vector<uint32_t>* temp_keybindings):
    m_menus{ },
    m_menu_selected{ 0 },
    m_ctx{ ctx },
    m_recent_games{ recent_games },
    m_keybindings{ keybindings },
    m_temp_keybindings{ temp_keybindings },
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
};
int Menu_Handler::addMenu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects) {
    m_menus.push_back(Menu(menu_title, entries, entry_effects));
    return static_cast<int>(m_menus.size()) - 1; // Index of new Menu
};
void Menu_Handler::triggerScrollEvent(int direction) {
    m_menus[m_menu_selected].triggerScrollEvent(direction);
};
EntryEffect Menu_Handler::triggerSelectEvent() {
    return m_menus[m_menu_selected].triggerSelectEvent();
};
void Menu_Handler::switchToMenu(int menu) {
    if (menu < m_menu_selected) { // Assume moving to a master list from a sublist
        m_menus[m_menu_selected].setSelected(0);
    }
    m_menu_selected = menu;
};
void Menu_Handler::processEvent(SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) { // KeyPress
        
    }else if (event->type == SDL_EVENT_KEY_UP) { // KeyRelease
        if (!m_bind_next_key) {
            if (event->key.key == SDLK_UP) { // TODO: Use customized controls as well
                triggerScrollEvent(-1);
            }else if (event->key.key == SDLK_DOWN) {
                triggerScrollEvent(1);
            }else if (
                event->key.key == SDLK_RETURN || 
                event->key.key == SDLK_SPACE || 
                event->key.key == SDLK_Z || 
                event->key.key == SDLK_X
            ) {
                interpretMenuEffect(triggerSelectEvent());
            }else {
                if (event->key.key == m_keybindings->at(Key_Up)) {
                    triggerScrollEvent(-1);
                }else if (event->key.key == m_keybindings->at(Key_Down)) {
                    triggerScrollEvent(1);
                }else if (
                    event->key.key == m_keybindings->at(Key_A) || 
                    event->key.key == m_keybindings->at(Key_B) || 
                    event->key.key == m_keybindings->at(Key_Start)
                ) {
                    interpretMenuEffect(triggerSelectEvent());
                }
            }
        }else {
            m_temp_keybindings->at(m_bind_to_key) = event->key.key;
            m_bind_next_key = false;
            m_menus[m_menu_selected].replaceKeyEntry(m_bind_to_key, SDL_GetKeyName(event->key.key));
        }
    }else if (event->type == SDL_EVENT_MOUSE_MOTION) {
        
    }else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        
    }else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        
    }
};

void Menu_Handler::interpretMenuEffect(EntryEffect effect) {
    switch (effect.getType()) {
        case EFFECT_NONE:
            break;
        case EFFECT_TO_MENU:
            switchToMenu(effect.getArg());
            break;
        case EFFECT_SELECT_ROM: { // Braces because the variable declaration
            std::string filepath = openROMFilePicker(getCtx()->getWindowHandle());
            if (filepath != "") {
                addGameToRecent(filepath);
                saveRecentGamesFile(m_recent_games);
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
    }
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

DrawingContext* Menu_Handler::getCtx() {
    return m_ctx;
};