
#include "MenuHandler.h"

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


Menu_Handler::Menu_Handler():
    m_menus{ },
    m_menu_selected{ 0 }
{

};

void Menu_Handler::drawSelf(DrawingContext* ctx) {
    m_menus[m_menu_selected].drawSelf(ctx);
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