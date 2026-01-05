
#ifndef INTERFACE_MENU_
#define INTERFACE_MENU_

#include <vector>
#include <string>

typedef enum {
    TO_MENU
} EntryEffectType;

class EntryEffect {
    public:
        EntryEffect();
        EntryEffect(EntryEffectType effect_type, int effect_arg);

        EntryEffectType getType();
        int getArg();
    private:
        EntryEffectType m_effect_type;
        int m_effect_arg;
};

class Menu {
    public:
        Menu();
        Menu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects);

        void drawSelf();
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
    private:
        std::string m_menu_title;
        std::vector<std::string> m_entries;
        std::vector<EntryEffect> m_entry_effects;
        int m_entry_selected;
};

class Menu_Handler {
    public:
        Menu_Handler();
        // Define destructor only if needed

        void drawSelf();

        int addMenu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects);
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
        void switchToMenu(int menu);
    private:
        std::vector<Menu> m_menus;
        int m_menu_selected;
};

#endif