#ifndef EMULATOR_CPU_H_
#define EMULATOR_CPU_H_

#include <cstdint>
#include <vector>
#include "memory.h"
#include "../utils.h"

class DMG_CPU {
    public:
        DMG_CPU(Emulator_Options* options, EmulatorState &state);

        void Start_Emulation(std::vector<uint8_t>* rom);

    private:
        Memory_Handler m_Memory;
        Register_Handler m_regs;

        Emulator_Options* m_options;
        EmulatorState &m_state;
};

#endif