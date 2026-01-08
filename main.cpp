
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "menus/MenuHandler.h"
#include "filehandle.h"
#include "utils.h"
#include "SDL-Drawing-Library/DrawingContext.h"

// Constants
constexpr int kFrameRate{ 60U };
constexpr int kNSPerFrame{ 16666666U }; // { 1,000,000,000 / kFrameRate };

typedef enum {
    State_InMenu,
    State_InEmulator
} EmulatorState;


// void readSettingsFiles(std::vector<std::string>* recent_games) {
//     recent_games = readRecentGamesFile();
// };

void setupMenus(Menu_Handler* m_menus, std::vector<std::string>* recent_games, std::vector<uint32_t>* keybindings) { // TODO: Needs to know contents of ROM directory and emulator settings
    const int main_menu = 0;
    const int recent_menu = 1;
    // const int load_menu = 2;
    const int keybindings_menu = 2;
    const int options_menu = 3;

    std::vector<std::string> recent_entries{ std::vector<std::string>() }; // For Recent Menu
    std::vector<EntryEffect> recent_effects{ std::vector<EntryEffect>() };
    for (int i = 0; i < static_cast<int>(recent_games->size()); i++) {
        if (recent_games->at(i) != "") {
            recent_entries.push_back(trimPathAndLength(recent_games->at(i), 40));
            recent_effects.push_back(EntryEffect());
        }
    }
    recent_entries.push_back("Back");
    recent_effects.push_back(EntryEffect(EFFECT_TO_MENU, main_menu));
    
    // For Keybindings Menu
    std::vector<std::string> keybinding_entries{ {"Up: ", "Right: ", "Down: ", "Left: ", "Start: ", "Select: ", "A: ", "B: ", "Cancel", "Save"} };
    for (int i = 0; i < 8; i++) {
        keybinding_entries[i] += SDL_GetKeyName(keybindings->at(i));
    }

    m_menus->addMenu( // Main Menu
        "Welcome! Please select an action.",
        {"Load Recent", "Load Game", "Keybindings", "Options"},
        {EntryEffect(EFFECT_TO_MENU, recent_menu), EntryEffect(EFFECT_SELECT_ROM, -1), EntryEffect(EFFECT_TO_MENU, keybindings_menu), EntryEffect(EFFECT_TO_MENU, options_menu)}
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
            EntryEffect(EFFECT_FORGET_KEYBIND, main_menu), 
            EntryEffect(EFFECT_SAVE_KEYBIND, main_menu)
        }
    );
    m_menus->addMenu( // Options Menu
        "Options",
        {"Screen Scale: 4", "Run Boot ROM: true", "Cancel", "Save"},
        {EntryEffect(), EntryEffect(), EntryEffect(EFFECT_TO_MENU, main_menu), EntryEffect(EFFECT_TO_MENU, main_menu)}
    );
};


int main() {
    
    std::vector<std::string>* m_recent_games{ readRecentGamesFile() };

    std::vector<uint32_t>* m_keybindings{ readKeybindingsFile() };
    std::vector<uint32_t>* m_temp_keybindings{ new std::vector<uint32_t>(*m_keybindings) };
    
    SDL_Log("COMPLETE: Initialization: Emulator Data Retreived");

    DrawingContext* m_ctx{ new DrawingContext("Detonation Emulation") };

    if (m_ctx->init() != 0) {
        SDL_Log("ERROR: Main: Window.init() Failed");
    }else {

        Menu_Handler* m_menus{ new Menu_Handler(m_ctx, m_recent_games, m_keybindings, m_temp_keybindings) };
        EmulatorState m_state{ State_InMenu };
        setupMenus(m_menus, m_recent_games, m_keybindings);

        bool quit{ false };
        
        SDL_Event event;
        SDL_zero(event);

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

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT) {
                        quit = true;
                    }else {
                        switch (m_state) {
                            case State_InMenu:
                                m_menus->processEvent(&event);
                                break;
                            case State_InEmulator:
                                break;
                        }
                    } 
                }

                switch (m_state) {
                    case State_InMenu:
                        m_menus->drawSelf();
                    case State_InEmulator:
                        break;
                }

                ticksNS = SDL_GetTicksNS();
            }

            // SDL_Log("Delay : %s", (ticksNS - nextFrameNS));
            SDL_DelayNS(nextFrameNS - ticksNS);
        }

        // DESTROY THINGS!
        delete m_menus;
        m_menus = nullptr;
    }

    // DESTROY THINGS!
    SDL_Log("EXITING: Program: Destroying Objects");
    delete m_recent_games;
    m_recent_games = nullptr;
    delete m_ctx;
    m_ctx = nullptr;
    

    SDL_Log("EXIT");
    return 0;
};