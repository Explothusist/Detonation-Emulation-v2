
#ifndef INTERFACE_MENU_
#define INTERFACE_MENU_

// #include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <cstdint>
#include "../utils.h"

class DrawingContext;
class Emulator_Options;
union SDL_Event;
class DMG_Emulator;

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

        void drawSelf(DrawingContext* ctx, bool bind_next_key);
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
        void setSelected(int select);

        void replaceKeyEntry(int entry, std::string new_key);
        void setEntryColor(int entry, EntryColor new_color);
        void setEntries(std::vector<std::string> entries);
        void setEffects(std::vector<EntryEffect> entry_effects);
        void setColors(std::vector<EntryColor> entry_colors);
    private:
        std::string m_menu_title;
        std::vector<std::string> m_entries;
        std::vector<EntryEffect> m_entry_effects;
        int m_entry_selected;

        std::vector<EntryColor> m_entry_colors;
};

class Popup {
    public:
        Popup();
        Popup(std::string text, std::vector<std::string> buttons, std::vector<EntryEffect> button_effects);

        void drawSelf(DrawingContext* ctx);
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
        void setSelected(int select);
    private:
        std::string m_text;
        std::vector<std::string> m_buttons;
        std::vector<EntryEffect> m_button_effects;
        int m_button_selected;
};

class Menu_Handler {
    public:
        Menu_Handler(DrawingContext* ctx, std::vector<std::string>* recent_games, std::vector<uint32_t>* keybindings, std::vector<uint32_t>* temp_keybindings, Emulator_Options* options);
        ~Menu_Handler();

        void drawSelf();

        int addMenu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects);
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
        void switchToMenu(int menu);
        bool processEvent(SDL_Event* event); // Returns whether emulation should begin

        bool interpretMenuEffect(EntryEffect effect); // Returns whether emulation should begin
        void addGameToRecent(std::string filepath);

        void createPopup(Popup popup);

        std::vector<uint8_t>* getROM(); // Use once it has been retreived (interpretMenuEffect returns true)

        void replaceKeyEntriesOnMenu(std::vector<std::string> new_keys, int menu);
        void setColorsOnMenu(std::vector<EntryColor> new_colors, int menu);
        void reloadRecentGamesMenu(int menu);

        DrawingContext* getCtx();
        void setEmulator(DMG_Emulator* m_cpu);
    private:
        std::vector<Menu> m_menus;
        Popup m_popup;
        bool m_popup_active;
        int m_menu_selected;
        int m_last_menu;

        DrawingContext* m_ctx;
        DMG_Emulator* m_emulator;
        std::vector<std::string>* m_recent_games;
        std::vector<uint32_t>* m_keybindings;
        std::vector<uint32_t>* m_temp_keybindings;
        Emulator_Options* m_options;

        std::vector<uint8_t>* m_rom;

        bool m_bind_next_key;
        int m_bind_to_key;
};

#endif