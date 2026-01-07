
#include "MenuHandler.h"

#include "../filehandle.h"
#include "../SDL-Drawing-Library/DrawingContext.h"

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

void Menu::drawSelf(DrawingContext* ctx) {
    ctx->background(200, 200, 200);

    int base_w = 600;
    int base_h = 600;

    int base_x = (ctx->getScreenWidth() - base_w) / 2;
    int base_y = (ctx->getScreenHeight() - base_h) / 2;

    ctx->fill(50, 50, 50);
    ctx->rectOutline(base_x, base_y, base_w, base_h);

    ctx->fill(50, 50, 50);
    ctx->textSize(32);
    ctx->text(m_menu_title, base_x+20, base_y+20);

    ctx->textSize(32);
    for (int i = 0; i < static_cast<int>(m_entries.size()); i++) {
        if (i == m_entry_selected) {
            ctx->fill(160, 160, 160);
            ctx->rect(base_x+70, base_y+75+(i*45), 300, 42);
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


Menu_Handler::Menu_Handler(DrawingContext* ctx, std::vector<std::string>* recent_games):
    m_menus{ },
    m_menu_selected{ 0 },
    m_ctx{ ctx },
    m_recent_games{ recent_games }
{

};

void Menu_Handler::drawSelf() {
    m_menus[m_menu_selected].drawSelf(getCtx());
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
        if (event->key.key == SDLK_UP) { // TODO: Use customized controls as well
            triggerScrollEvent(-1);
        }else if (event->key.key == SDLK_DOWN) {
            triggerScrollEvent(1);
        }else if (event->key.key == SDLK_RETURN || event->key.key == SDLK_SPACE) {
            interpretMenuEffect(triggerSelectEvent());
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
        case EFFECT_SELECT_ROM:
            std::string filepath = openROMFilePicker(getCtx()->getWindowHandle());
            if (filepath != "") {
                addGameToRecent(filepath);
                saveRecentGamesFile(m_recent_games);
            }
            
            break;
    }
};
void Menu_Handler::addGameToRecent(std::string filepath) {
    bool match_found{ false };
    SDL_Log("Boo");
    for (int i = 0; i < static_cast<int>(m_recent_games->size()); i++) {
    SDL_Log("Radley");
        if (m_recent_games->at(i) == filepath) {
    SDL_Log("Was");
            m_recent_games->erase(m_recent_games->begin() + i);
            match_found = true;
            break;
        }
    }
    SDL_Log("Here");
    if (!match_found) {
        m_recent_games->pop_back();
    }
    SDL_Log("Bwa");
    m_recent_games->insert(m_recent_games->begin(), filepath);
    SDL_Log("Ha");
};

DrawingContext* Menu_Handler::getCtx() {
    return m_ctx;
};