#ifndef EMULATOR_CPU_H_
#define EMULATOR_CPU_H_

#include <cstdint>
#include <vector>
#include "memory.h"
#include "ppu.h"
#include "apu.h"
#include "opcodes.h"
#include "../utils.h"

class Menu_Handler;
class DrawingContext;

class DMG_CPU {
    public:
        DMG_CPU();
        
        Cart_Details Trigger_InitializeAll(std::vector<uint8_t>* rom, bool use_boot_rom);
        void Trigger_PowerCycle(); // Full Reset on everything, including ROM
        void Trigger_ResetButton(); // Resets all RAM, but keeps ROM
        void PowerCycle();
        void Reset(); // WORKING HERE

        void Initialize(bool use_boot_rom);

        Opcode* parseOpcode(uint8_t opcode);
        void clearOpcode(); // So that variable length ones can fix themselves
        void runMCycle();
        void runTCycle();

        void callAbort();

        uint8_t PC_Eat_Byte();
        uint8_t ALU_B8_ADDER(uint8_t num1, uint8_t num2, uint8_t carry_bit = 0);
        uint8_t ALU_B8_SUBBER(uint8_t num1, uint8_t num2, uint8_t carry_bit = 0);
        // void PUSH_B16(Reg_u8 reg1, Reg_u8 reg2);
        // void POP_B16(Reg_u8 reg1, Reg_u8 reg2); // Pops into registers

        Memory_Handler m_Memory;
        Register_Handler m_regs;
        DMG_PPU m_ppu;
        DMG_APU m_apu;
    private:
        int m_cycle_count;

        Opcode* m_curr_opcode;
        // int m_curr_opcode_length;
        int m_curr_opcode_mcycle;

        bool m_abort;
};

#endif