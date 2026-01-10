#ifndef EMULATOR_CPU_H_
#define EMULATOR_CPU_H_

#include <cstdint>
#include <vector>
#include "memory.h"
#include "../utils.h"

class Menu_Handler;
class DrawingContext;

class DMG_CPU {
    public:
        DMG_CPU(DrawingContext* ctx, Menu_Handler* menus, Emulator_Options* options, EmulatorState &state);

        void Start_Emulation(std::vector<uint8_t>* rom);
        void Resume_Emulation();

        void drawSelf(); // This is mortally incorrect

        bool hasInitialized();
        void unInitialize();

    private:
        Memory_Handler m_Memory;
        Register_Handler m_regs;

        DrawingContext* m_ctx;
        Menu_Handler* m_menus;
        Emulator_Options* m_options;
        EmulatorState &m_state;

        bool m_initialized;
        bool m_emulation_begun;
};

#endif