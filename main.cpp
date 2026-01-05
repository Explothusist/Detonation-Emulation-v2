
#include "menus/MenuHandler.h"
#include "SDL-Drawing-Library/DrawingContext.h"

// Constants
constexpr int kFrameRate{ 60U };
constexpr int kNSPerFrame{ 16666666U }; // { 1,000,000,000 / kFrameRate };


void interpretMenuEffect(EntryEffect effect, Menu_Handler &m_menus) {
    switch (effect.getType()) {
        case EFFECT_NONE:
            break;
        case EFFECT_TO_MENU:
            m_menus.switchToMenu(effect.getArg());
            break;
    }
};


void setupMenus(Menu_Handler &m_menus) { // TODO: Needs to know contents of ROM directory and emulator settings
    const int main_menu = 0;
    const int recent_menu = 1;
    const int load_menu = 2;
    const int keybindings_menu = 3;
    const int options_menu = 4;

    m_menus.addMenu( // Main Menu
        "Welcome! Please select an action.",
        {"Load Recent", "Load Game", "Keybindings", "Options"},
        {EntryEffect(EFFECT_TO_MENU, recent_menu), EntryEffect(EFFECT_TO_MENU, load_menu), EntryEffect(EFFECT_TO_MENU, keybindings_menu), EntryEffect(EFFECT_TO_MENU, options_menu)}
    );
    m_menus.addMenu( // Recent Menu
        "Select a game to load.",
        {"ZELDA", "METROID2", "POKEMON RED", "Back"},
        {EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(EFFECT_TO_MENU, main_menu)}
    );
    m_menus.addMenu( // Load Menu
        "Select a game to load.",
        {"ZELDA", "METROID2", "POKEMON RED", "Next Page", "Back"},
        {EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(EFFECT_TO_MENU, main_menu)}
    );
    m_menus.addMenu( // Keybindings Menu
        "Select a game to load.",
        {"Left: ", "Down: ", "Right: ", "Up: ", "B: ", "A: ", "Start: ", "Select: ", "Cancel", "Save"},
        {EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(), EntryEffect(EFFECT_TO_MENU, main_menu), EntryEffect(EFFECT_TO_MENU, main_menu)}  
    );
    m_menus.addMenu( // Options Menu
        "Options",
        {"Screen Scale: 4", "Run Boot ROM: true", "Cancel", "Save"},
        {EntryEffect(), EntryEffect(), EntryEffect(EFFECT_TO_MENU, main_menu), EntryEffect(EFFECT_TO_MENU, main_menu)}
    );
};

void handleSDLEvents(bool &quit, bool &menus_have_control, Menu_Handler &m_menus) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            quit = true;
        }else {
            if (menus_have_control) {
                if (event.type == SDL_EVENT_KEY_DOWN) { // KeyPress
                    
                }else if (event.type == SDL_EVENT_KEY_UP) { // KeyRelease
                    if (event.key.key == SDLK_UP) { // TODO: Use customized controls as well
                        m_menus.triggerScrollEvent(-1);
                    }else if (event.key.key == SDLK_DOWN) {
                        m_menus.triggerScrollEvent(1);
                    }else if (event.key.key == SDLK_RETURN || event.key.key == SDLK_SPACE) {
                        interpretMenuEffect(m_menus.triggerSelectEvent(), m_menus);
                    }
                }else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    
                }else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    
                }else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    
                }
            }
        } 
    }
};

void handleScreen(DrawingContext* ctx, bool &menus_have_control, Menu_Handler &m_menus) {
    if (menus_have_control) {
        // Emulator Menus have control
        m_menus.drawSelf(ctx);
    }else {
        // Emulator has control
    }
};


int main() {

    DrawingContext* m_ctx{ new DrawingContext("Detonation Emulation") };

    if (m_ctx->init() != 0) {
        SDL_Log("ERROR: Main: Window.init() Failed");
    }else {

        Menu_Handler m_menus{ Menu_Handler() };
        bool menus_have_control{ true };
        setupMenus(m_menus);

        bool quit{ false };
        
        // SDL_Event event;
        // SDL_zero(event);

        Uint64 ticksNS{ 0 };
        Uint64 nextFrameNS{ SDL_GetTicksNS() };

        SDL_Log("COMPLETE: Initialization: Beginning Main Loop");

        // Do Setup things here:
        m_ctx->loadFont(Eight_Bit_Plus);
        m_ctx->setTextResizePixelated(true);

        // Loading Initial Screen

        while (!quit) {
            // SDL_Log("Ticks : %s, Next Frame : %s", tickNS, nextFrameNS);

            ticksNS = SDL_GetTicksNS();
            while (ticksNS > nextFrameNS) {
                // nextFrameNS += static_cast<Uint64>(kNSPerFrame); // Rigidly Refues to Adapt
                nextFrameNS = ticksNS+static_cast<Uint64>(kNSPerFrame); // Adapts to Lag

                handleSDLEvents(quit, menus_have_control, m_menus);

                handleScreen(m_ctx, menus_have_control, m_menus);

                ticksNS = SDL_GetTicksNS();
            }

            // SDL_Log("Delay : %s", (ticksNS - nextFrameNS));
            SDL_DelayNS(nextFrameNS - ticksNS);
        }
    }

    // DESTROY THINGS!
    SDL_Log("EXITING: Program: Destroying Objects");
    m_ctx->destroy();

    SDL_Log("EXIT");
    return 0;
};