#ifndef EMULATOR_CPU_H_
#define EMULATOR_CPU_H_

#include <cstdint>
#include <vector>
#include "memory.h"
#include "ppu.h"
#include "apu.h"
#include "opcodes.h"
// #include "emulator_log.h"
#include "../utils.h"

class Menu_Handler;
class DrawingContext;

class DMG_CPU {
    public:
        DMG_CPU();
        
        Cart_Details Trigger_InitializeAll(std::vector<uint8_t>* rom, Emulator_Options* options);
        void Trigger_PowerCycle(); // Full Reset on everything, including ROM
        void Trigger_ResetButton(); // Resets all RAM, but keeps ROM
        void PowerCycle();
        void Reset(); // WORKING HERE

        void Initialize(bool use_boot_rom, int log_length, bool log_enable);

        void UpdateSettings(Emulator_Options* options);

        Opcode* parseOpcode(uint8_t opcode);
        Opcode* parseCBOpcode(uint8_t opcode);
        void clearOpcode(); // So that variable length ones can fix themselves
        void loadCBOpcode();
        void runMCycle();
        void runTCycle();

        void runFrame();

        void callAbort();
        
        void STOP();
        void HALT();
        void HALT_Bug();
        bool WAKE();

        uint8_t PC_Eat_Byte();
        uint8_t ALU_B8_ADDER(uint8_t num1, uint8_t num2, uint8_t carry_bit = 0);
        uint8_t ALU_B8_SUBBER(uint8_t num1, uint8_t num2, uint8_t carry_bit = 0);
        // void PUSH_B16(Reg_u8 reg1, Reg_u8 reg2);
        // void POP_B16(Reg_u8 reg1, Reg_u8 reg2); // Pops into registers

        Memory_Handler m_Memory;
        Register_Handler m_regs;
        DMG_PPU m_ppu;
        DMG_APU m_apu;
        // Emulator_Logfile m_logfile;
    private:
        int m_cycle_count;

        Opcode* m_curr_opcode;
        // int m_curr_opcode_length;
        int m_curr_opcode_mcycle;

        bool m_stopped; // Slightly different rules
        bool m_halted;
        bool m_halt_bug;
        bool m_read_cb_opcode;

        bool m_abort;

        // std::string m_log_line;
        // bool m_log_enable;
};

#endif