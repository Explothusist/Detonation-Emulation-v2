
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "enable_logging.h"
#include "menus/MenuHandler.h"
#include "emulator/emulator_frontend.h"
#include "filehandle.h"
#include "utils.h"
#include "SDL-Drawing-Library/DrawingContext.h"


void setupMenus(Menu_Handler* m_menus, std::vector<std::string>* recent_games, std::vector<uint32_t>* keybindings, Emulator_Options* options) {
    std::vector<std::string> recent_entries{ std::vector<std::string>() }; // For Recent Menu
    std::vector<EntryEffect> recent_effects{ std::vector<EntryEffect>() };
    for (int i = 0; i < static_cast<int>(recent_games->size()); i++) {
        if (recent_games->at(i) != "") {
            recent_entries.push_back(trimPathAndLength(recent_games->at(i), 40));
            recent_effects.push_back(EntryEffect(EFFECT_LOAD_RECENT_RELOAD_RECENT, i));
        }
    }
    recent_entries.push_back("Back");
    recent_effects.push_back(EntryEffect(EFFECT_TO_MENU, Main_Menu));
    
    // For Keybindings Menu
    std::vector<std::string> keybinding_entries{ {"Up: ", "Right: ", "Down: ", "Left: ", "Start: ", "Select: ", "A: ", "B: ", "Cancel", "Save"} };
    for (int i = 0; i < 8; i++) {
        keybinding_entries[i] += SDL_GetKeyName(keybindings->at(i));
    }

    m_menus->addMenu( // Main Menu
        "Welcome! Please select an action.",
        {"Load Recent", "Load Game", "Keybindings", "Options", 
#ifdef DEBUG_LOGGING
            "Logfile"
#endif
        },
        {EntryEffect(EFFECT_TO_MENU, Recent_Menu), EntryEffect(EFFECT_SELECT_ROM_RELOAD_RECENT, -1), EntryEffect(EFFECT_TO_MENU, Keybindings_Menu), EntryEffect(EFFECT_TO_MENU, Options_Menu),
#ifdef DEBUG_LOGGING
            EntryEffect(EFFECT_TO_MENU, Logfile_Menu)
#endif
}
    );
    m_menus->addMenu( // Pause Menu
        "Game Paused",
        {"Back to Game", "Main Menu", "Keybindings", "Options", 
#ifdef DEBUG_LOGGING
            "Logfile"
#endif
        },
        {EntryEffect(EFFECT_BACK_TO_EMULATOR, -1), EntryEffect(EFFECT_RETURN_TO_MAIN, Main_Menu), EntryEffect(EFFECT_TO_MENU, Keybindings_Menu), EntryEffect(EFFECT_TO_MENU, Options_Menu),
#ifdef DEBUG_LOGGING
            EntryEffect(EFFECT_TO_MENU, Logfile_Menu)
#endif
        }
    );
    m_menus->addMenu( // Recent Menu
        "Select a game to load.",
        recent_entries,
        recent_effects
    );
    m_menus->addMenu( // Keybindings Menu
        "Select a game to load.",
        keybinding_entries,
        {
            EntryEffect(EFFECT_SET_KEYBIND, Key_Up), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_Right), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_Down), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_Left), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_Start), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_Select), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_A), 
            EntryEffect(EFFECT_SET_KEYBIND, Key_B), 
            EntryEffect(EFFECT_FORGET_KEYBIND, -1), // To Last Menu
            EntryEffect(EFFECT_SAVE_KEYBIND, -1) // To Last Menu
        }
    );
    m_menus->addMenu( // Options Menu
        "Options",
        {
            "Run Boot ROM: "+std::string(options->m_run_boot_rom ? "True" : "False"),
            "Strict Loading: "+std::string(options->m_strict_loading ? "True" : "False"),
            "Display Cart Info: "+std::string(options->m_display_cart_info ? "True" : "False"),
            "Cancel",
            "Save"
        },
        {
            EntryEffect(EFFECT_TOGGLE, TOGGLE_BOOT_ROM),
            EntryEffect(EFFECT_TOGGLE, TOGGLE_STRICT_LOADING),
            EntryEffect(EFFECT_TOGGLE, TOGGLE_DISPLAY_CART_INFO),
            EntryEffect(EFFECT_FORGET_OPTIONS, -1),
            EntryEffect(EFFECT_SAVE_OPTIONS, -1)
        }
    );
    m_menus->addMenu( // Cartridge Info
        "Cartridge Info",
        {
            "ROM Name: ",
            "Cartridge Type: ",
            "ROM Size: ",
            "RAM Size: ",
            "Platform: ",
            "Licensee: ",
            "Header Checksum: ",
            "Nintendo Logo: ",
            "Cancel",
            "Load"
        },
        {
            EntryEffect(),
            EntryEffect(),
            EntryEffect(),
            EntryEffect(),
            EntryEffect(),
            EntryEffect(),
            EntryEffect(),
            EntryEffect(),
            EntryEffect(EFFECT_TO_MENU_UNINITIALIZE_EMULATOR, -1),
            EntryEffect(EFFECT_LOAD_ANYWAY, -1)
        }
    );
    
#ifdef DEBUG_LOGGING
    m_menus->addMenu( // Logfile Menu
        "Logfile",
        {
            "Log Enabled: "+std::string(options->m_log_enable ? "True" : "False"),
            "Log Length: "+std::to_string(options->m_log_length),
            "Dump Logfile",
            "Cancel",
            "Save"
        },
        {
            EntryEffect(EFFECT_TOGGLE, TOGGLE_LOG_ENABLE),
            EntryEffect(EFFECT_TOGGLE, TOGGLE_LOG_LENGTH),
            EntryEffect(EFFECT_DUMP_LOGFILE, -1),
            EntryEffect(EFFECT_FORGET_OPTIONS, -1),
            EntryEffect(EFFECT_SAVE_OPTIONS, -1)
        }
    );
#endif
};


int main() {
    
    std::vector<std::string>* m_recent_games{ readRecentGamesFile() };

    std::vector<uint32_t>* m_keybindings{ readKeybindingsFile() };
    std::vector<uint32_t>* m_temp_keybindings{ new std::vector<uint32_t>(*m_keybindings) };

    Emulator_Options* m_options{ readOptionsFile() };
    
#ifdef DEBUG_LOGGING
    printf("COMPLETE: Initialization: Emulator Data Retreived\n");
#endif

    DrawingContext* m_ctx{ new DrawingContext("Detonation Emulation") };

    if (m_ctx->init() != 0) {
#ifdef DEBUG_LOGGING
        printf("ERROR: Main: Window.init() Failed\n");
#endif
    }else {

        EmulatorState m_state{ State_InMenu };
        Menu_Handler* m_menus{ new Menu_Handler(m_ctx, m_recent_games, m_keybindings, m_temp_keybindings, m_options) };
        DMG_Emulator* m_emulator{ new DMG_Emulator(m_ctx, m_menus, m_options, m_state) };
        m_menus->setEmulator(m_emulator);
        setupMenus(m_menus, m_recent_games, m_keybindings, m_options);

        bool quit{ false };
        
        SDL_Event event;
        SDL_zero(event);

        Uint64 ticksNS{ 0 };
        Uint64 nextFrameNS{ SDL_GetTicksNS() };

#ifdef DEBUG_LOGGING
        printf("COMPLETE: Initialization: Beginning Main Loop\n");
#endif

        // Do Setup things here:
        m_ctx->loadFont("fonts/8bitOperatorPlus-Regular.ttf");
        m_ctx->setTextResizePixelated(true);

        // Loading Initial Screen

        while (!quit) {
            // printf("Ticks : %s, Next Frame : %s\n", tickNS, nextFrameNS);

            ticksNS = SDL_GetTicksNS();
            while (ticksNS > nextFrameNS) {
                // nextFrameNS += static_cast<Uint64>(k_NS_Per_Frame); // Rigidly Refues to Adapt
                nextFrameNS = ticksNS+static_cast<Uint64>(k_NS_Per_Frame); // Adapts to Lag

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT) {
                        quit = true;
                    }else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                        m_state = State_InMenu;
                    }else {
                        switch (m_state) {
                            case State_InMenu: {
                                bool begin_emulation = m_menus->processEvent(&event);
                                if (begin_emulation) {
                                    if (!m_emulator->hasInitialized()) {
                                        m_emulator->Start_Emulation(m_menus->getROM(), m_keybindings);
                                    }else {
                                        m_emulator->Resume_Emulation();
                                    }
                                } }
                                break;
                            case State_InEmulator:
                                if (event.type == SDL_EVENT_KEY_DOWN) {
                                    m_emulator->feedKeyEvent(event.key.key, true);
                                }else if (event.type == SDL_EVENT_KEY_UP) {
                                    m_emulator->feedKeyEvent(event.key.key, false);
                                }
                                break;
                        }
                    } 
                }

                switch (m_state) {
                    case State_InMenu:
                        m_menus->drawSelf();
                        break;
                    case State_InEmulator:
                        m_emulator->runFrame();
                        break;
                }

                ticksNS = SDL_GetTicksNS();
            }

            // printf("Delay : %s\n", (ticksNS - nextFrameNS));
            SDL_DelayNS(nextFrameNS - ticksNS);
        }

        // DESTROY THINGS!
        delete m_menus;
        m_menus = nullptr;
        delete m_emulator;
        m_emulator = nullptr;
    }

    // DESTROY THINGS!
#ifdef DEBUG_LOGGING
    printf("EXITING: Program: Destroying Objects\n");
#endif
    delete m_recent_games;
    m_recent_games = nullptr;
    delete m_ctx;
    m_ctx = nullptr;
    

#ifdef DEBUG_LOGGING
    printf("EXIT\n");
#endif
    return 0;
};