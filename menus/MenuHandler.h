
#ifndef INTERFACE_MENU_
#define INTERFACE_MENU_

#include <SDL3/SDL.h>
#include <vector>
#include <string>

class DrawingContext;

typedef enum {
    EFFECT_NONE,
    EFFECT_TO_MENU,
    EFFECT_SELECT_ROM
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

        void drawSelf(DrawingContext* ctx);
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
        void setSelected(int select);
    private:
        std::string m_menu_title;
        std::vector<std::string> m_entries;
        std::vector<EntryEffect> m_entry_effects;
        int m_entry_selected;
};

class Menu_Handler {
    public:
        Menu_Handler(DrawingContext* ctx, std::vector<std::string>* recent_games);
        // Define destructor only if needed

        void drawSelf();

        int addMenu(std::string menu_title, std::vector<std::string> entries, std::vector<EntryEffect> entry_effects);
        void triggerScrollEvent(int direction);
        EntryEffect triggerSelectEvent();
        void switchToMenu(int menu);
        void processEvent(SDL_Event* event);

        void interpretMenuEffect(EntryEffect effect);
        void addGameToRecent(std::string filepath);

        DrawingContext* getCtx();
    private:
        std::vector<Menu> m_menus;
        int m_menu_selected;

        DrawingContext* m_ctx;
        std::vector<std::string>* m_recent_games;
};

#endif