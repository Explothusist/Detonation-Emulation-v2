
#ifndef INTERFACE_MENU_
#define INTERFACE_MENU_

#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include <cstdint>

class DrawingContext;
class Emulator_Options;

typedef enum {
    EFFECT_NONE,
    EFFECT_TO_MENU,
    EFFECT_SELECT_ROM,
    EFFECT_SET_KEYBIND,
    EFFECT_FORGET_KEYBIND,
    EFFECT_SAVE_KEYBIND,
    EFFECT_TOGGLE,
    EFFECT_FORGET_OPTIONS,
    EFFECT_SAVE_OPTIONS,
    EFFECT_LOAD_RECENT
} EntryEffectType;

typedef enum {
    TOGGLE_BOOT_ROM,
    TOGGLE_STRICT_LOADING,
    TOGGLE_DISPLAY_CART_INFO
} ToggleID;

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
    private:
        std::string m_menu_title;
        std::vector<std::string> m_entries;
        std::vector<EntryEffect> m_entry_effects;
        int m_entry_selected;
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

        std::vector<uint8_t>* getROM(); // Use once it has been retreived (interpretMenuEffect returns true)

        DrawingContext* getCtx();
    private:
        std::vector<Menu> m_menus;
        int m_menu_selected;

        DrawingContext* m_ctx;
        std::vector<std::string>* m_recent_games;
        std::vector<uint32_t>* m_keybindings;
        std::vector<uint32_t>* m_temp_keybindings;
        Emulator_Options* m_options;

        std::vector<uint8_t>* m_rom;

        bool m_bind_next_key;
        int m_bind_to_key;
};

#endif