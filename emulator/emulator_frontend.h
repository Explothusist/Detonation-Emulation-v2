#ifndef EMULATOR_DMG_FRONTEND_
#define EMULATOR_DMG_FRONTEND_

#include <cstdint>
#include <vector>
#include <deque>
#include "cpu.h"
#include "../utils.h"

class DMG_CPU;
class DrawingContext;
class Menu_Handler;

class DMG_Emulator {
    public:
        DMG_Emulator(DrawingContext* ctx, Menu_Handler* menus, Emulator_Options* options, EmulatorState &state);

        void Start_Emulation(std::vector<uint8_t>* rom);
        void Resume_Emulation();

        void drawBackground();
        void runFrame();

        bool hasInitialized();
        void unInitialize();

        std::deque<std::string>* getLogfile();
        std::string getRomname();
    private:
        DrawingContext* m_ctx;
        Menu_Handler* m_menus;
        Emulator_Options* m_options;
        EmulatorState &m_state;
        
        DMG_CPU m_cpu;

        Cart_Details m_cart_details;

        bool m_initialized;
        bool m_emulation_begun;
};

#endif