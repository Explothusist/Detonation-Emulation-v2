
#include "MenuHandler.h"

EntryEffect::EntryEffect():
    EntryEffect(TO_MENU, 1)
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

void Menu::drawSelf() {

};