
#include "cpu.h"

#include "../menus/MenuHandler.h"
#include "../SDL-Drawing-Library/DrawingContext.h" // This is mortally incorrect

DMG_CPU::DMG_CPU():
    m_Memory{ Memory_Handler() },
    m_regs{ Register_Handler() },
    m_ppu{ DMG_PPU() },
    m_apu{ DMG_APU() },
    m_cycle_count{ 0 },
    m_curr_opcode{ nullptr },
    // m_curr_opcode_length{ 0 },
    m_curr_opcode_mcycle{ 0 },
    m_abort{ false }
{

};

Cart_Details DMG_CPU::Trigger_InitializeAll(std::vector<uint8_t>* rom, bool use_boot_rom) {
    Cart_Details cart_details = m_Memory.Initialize(rom, use_boot_rom);
    Initialize(use_boot_rom);
    m_ppu.Initialize();
    m_apu.Initialize();
    return cart_details;
};
void DMG_CPU::Trigger_PowerCycle() {
    m_Memory.PowerCycle();
    PowerCycle();
    m_ppu.PowerCycle();
    m_apu.PowerCycle();
};
void DMG_CPU::Trigger_ResetButton() {
    m_Memory.Reset();
    Reset();
    m_ppu.Reset();
    m_apu.Reset();
};

void DMG_CPU::Initialize(bool use_boot_rom) {
    if (use_boot_rom) {
        m_regs.set(Reg_u16::PC, 0);
        m_regs.set(Reg_u16::SP, 0xfffe); // 0xffff is not RAM
    }else {
        m_regs.set(Reg_u16::PC, 0x0100); // Assumes standard DMG Boot ROM was run
        m_regs.set(Reg_u8::A, 0x01);
        m_regs.set(Reg_u8::F, 0b1000'0000); // Z, n, H?, C?
        m_regs.set(Reg_u8::B, 0x00);
        m_regs.set(Reg_u8::C, 0x13);
        m_regs.set(Reg_u8::D, 0x00);
        m_regs.set(Reg_u8::E, 0xD8);
        m_regs.set(Reg_u8::H, 0x01);
        m_regs.set(Reg_u8::L, 0x46);
        m_regs.set(Reg_u16::SP, 0xfffe);
    }
};
void DMG_CPU::PowerCycle() {
    Reset(); // CPU never keeps anything
};
void DMG_CPU::Reset() {
    m_cycle_count = 0;
    m_curr_opcode = nullptr;
    // m_curr_opcode_length = 0;
    m_curr_opcode_mcycle = 0;
    m_abort = false;
};


Opcode* DMG_CPU::parseOpcode(uint8_t opcode) {
    switch (opcode) {
        case 0x00: return &Opcode_x00_NOP;
        case 0x01: return &Opcode_x01_LD_BC_N16;
        case 0x02: return &Opcode_x02_LD_BC_A;
        case 0x03: return &Opcode_x03_INC_BC;
        case 0x04: return &Opcode_x04_INC_B;
        case 0x05: return &Opcode_x05_DEC_B;

        case 0x0A: return &Opcode_x0A_LD_BC_A;
        case 0x0B: return &Opcode_x0B_DEC_BC;
        case 0x0C: return &Opcode_x0C_INC_C;
        case 0x0D: return &Opcode_x0D_DEC_C;

        case 0x10: return &Opcode_x10_STOP;
        case 0x11: return &Opcode_x11_LD_DE_N16;
        case 0x12: return &Opcode_x12_LD_DE_A;
        case 0x13: return &Opcode_x13_INC_DE;
        case 0x14: return &Opcode_x14_INC_D;
        case 0x15: return &Opcode_x15_DEC_D;

        case 0x18: return &Opcode_x18_JR;

        case 0x1A: return &Opcode_x1A_LD_DE_A;
        case 0x1B: return &Opcode_x1B_DEC_DE;
        case 0x1C: return &Opcode_x1C_INC_E;
        case 0x1D: return &Opcode_x1D_DEC_E;

        case 0x20: return &Opcode_x20_JR_NZ;
        case 0x21: return &Opcode_x21_LD_HL_N16;
        case 0x22: return &Opcode_x22_LD_HLI_A;
        case 0x23: return &Opcode_x23_INC_HL;
        case 0x24: return &Opcode_x24_INC_H;
        case 0x25: return &Opcode_x25_DEC_HL;

        case 0x28: return &Opcode_x28_JR_Z;

        case 0x2A: return &Opcode_x2A_LD_HLI_A;
        case 0x2B: return &Opcode_x2B_DEC_HL;
        case 0x2C: return &Opcode_x2C_INC_L;
        case 0x2D: return &Opcode_x2D_DEC_L;

        case 0x30: return &Opcode_x30_JR_NC;
        case 0x31: return &Opcode_x31_LD_SP_N16;
        case 0x32: return &Opcode_x32_LD_HLD_A;
        case 0x33: return &Opcode_x33_INC_SP;
        case 0x34: return &Opcode_x34_INC_HL;
        case 0x35: return &Opcode_x35_DEC_H;

        case 0x38: return &Opcode_x38_JR_C;

        case 0x3A: return &Opcode_x3A_LD_HLD_A;
        case 0x3B: return &Opcode_x3B_DEC_SP;
        case 0x3C: return &Opcode_x3C_INC_A;
        case 0x3D: return &Opcode_x3D_DEC_A;

        case 0x40: return &Opcode_x40_LD_B_B;
        case 0x41: return &Opcode_x41_LD_B_C;
        case 0x42: return &Opcode_x42_LD_B_D;
        case 0x43: return &Opcode_x43_LD_B_E;
        case 0x44: return &Opcode_x44_LD_B_H;
        case 0x45: return &Opcode_x45_LD_B_L;
        case 0x46: return &Opcode_x46_LD_B_HL;
        case 0x47: return &Opcode_x47_LD_B_A;
        case 0x48: return &Opcode_x48_LD_C_B;
        case 0x49: return &Opcode_x49_LD_C_C;
        case 0x4A: return &Opcode_x4A_LD_C_D;
        case 0x4B: return &Opcode_x4B_LD_C_E;
        case 0x4C: return &Opcode_x4C_LD_C_H;
        case 0x4D: return &Opcode_x4D_LD_C_L;
        case 0x4E: return &Opcode_x4E_LD_C_HL;
        case 0x4F: return &Opcode_x4F_LD_C_A;
        case 0x50: return &Opcode_x50_LD_D_B;
        case 0x51: return &Opcode_x51_LD_D_C;
        case 0x52: return &Opcode_x52_LD_D_D;
        case 0x53: return &Opcode_x53_LD_D_E;
        case 0x54: return &Opcode_x54_LD_D_H;
        case 0x55: return &Opcode_x55_LD_D_L;
        case 0x56: return &Opcode_x56_LD_D_HL;
        case 0x57: return &Opcode_x57_LD_D_A;
        case 0x58: return &Opcode_x58_LD_E_B;
        case 0x59: return &Opcode_x59_LD_E_C;
        case 0x5A: return &Opcode_x5A_LD_E_D;
        case 0x5B: return &Opcode_x5B_LD_E_E;
        case 0x5C: return &Opcode_x5C_LD_E_H;
        case 0x5D: return &Opcode_x5D_LD_E_L;
        case 0x5E: return &Opcode_x5E_LD_E_HL;
        case 0x5F: return &Opcode_x5F_LD_E_A;
        case 0x60: return &Opcode_x60_LD_H_B;
        case 0x61: return &Opcode_x61_LD_H_C;
        case 0x62: return &Opcode_x62_LD_H_D;
        case 0x63: return &Opcode_x63_LD_H_E;
        case 0x64: return &Opcode_x64_LD_H_H;
        case 0x65: return &Opcode_x65_LD_H_L;
        case 0x66: return &Opcode_x66_LD_H_HL;
        case 0x67: return &Opcode_x67_LD_H_A;
        case 0x68: return &Opcode_x68_LD_L_B;
        case 0x69: return &Opcode_x69_LD_L_C;
        case 0x6A: return &Opcode_x6A_LD_L_D;
        case 0x6B: return &Opcode_x6B_LD_L_E;
        case 0x6C: return &Opcode_x6C_LD_L_H;
        case 0x6D: return &Opcode_x6D_LD_L_L;
        case 0x6E: return &Opcode_x6E_LD_L_HL;
        case 0x6F: return &Opcode_x6F_LD_L_A;
        case 0x70: return &Opcode_x70_LD_HL_B;
        case 0x71: return &Opcode_x71_LD_HL_C;
        case 0x72: return &Opcode_x72_LD_HL_D;
        case 0x73: return &Opcode_x73_LD_HL_E;
        case 0x74: return &Opcode_x74_LD_HL_H;
        case 0x75: return &Opcode_x75_LD_HL_L;
        case 0x76: return &Opcode_x76_HALT;
        case 0x77: return &Opcode_x77_LD_HL_A;
        case 0x78: return &Opcode_x78_LD_A_B;
        case 0x79: return &Opcode_x79_LD_A_C;
        case 0x7A: return &Opcode_x7A_LD_A_D;
        case 0x7B: return &Opcode_x7B_LD_A_E;
        case 0x7C: return &Opcode_x7C_LD_A_H;
        case 0x7D: return &Opcode_x7D_LD_A_L;
        case 0x7E: return &Opcode_x7E_LD_A_HL;
        case 0x7F: return &Opcode_x7F_LD_A_A;
        case 0x80: return &Opcode_x80_ADD_A_B;
        case 0x81: return &Opcode_x81_ADD_A_C;
        case 0x82: return &Opcode_x82_ADD_A_D;
        case 0x83: return &Opcode_x83_ADD_A_E;
        case 0x84: return &Opcode_x84_ADD_A_H;
        case 0x85: return &Opcode_x85_ADD_A_L;
        case 0x86: return &Opcode_x86_ADD_A_HL;
        case 0x87: return &Opcode_x87_ADD_A_A;
        case 0x88: return &Opcode_x88_ADC_A_B;
        case 0x89: return &Opcode_x89_ADC_A_C;
        case 0x8A: return &Opcode_x8A_ADC_A_D;
        case 0x8B: return &Opcode_x8B_ADC_A_E;
        case 0x8C: return &Opcode_x8C_ADC_A_H;
        case 0x8D: return &Opcode_x8D_ADC_A_L;
        case 0x8E: return &Opcode_x8E_ADC_A_HL;
        case 0x8F: return &Opcode_x8F_ADC_A_A;
        case 0x90: return &Opcode_x90_SUB_A_B;
        case 0x91: return &Opcode_x91_SUB_A_C;
        case 0x92: return &Opcode_x92_SUB_A_D;
        case 0x93: return &Opcode_x93_SUB_A_E;
        case 0x94: return &Opcode_x94_SUB_A_H;
        case 0x95: return &Opcode_x95_SUB_A_L;
        case 0x96: return &Opcode_x96_SUB_A_HL;
        case 0x97: return &Opcode_x97_SUB_A_A;
        case 0x98: return &Opcode_x98_SBC_A_B;
        case 0x99: return &Opcode_x99_SBC_A_C;
        case 0x9A: return &Opcode_x9A_SBC_A_D;
        case 0x9B: return &Opcode_x9B_SBC_A_E;
        case 0x9C: return &Opcode_x9C_SBC_A_H;
        case 0x9D: return &Opcode_x9D_SBC_A_L;
        case 0x9E: return &Opcode_x9E_SBC_A_HL;
        case 0x9F: return &Opcode_x9F_SBC_A_A;
        case 0xA0: return &Opcode_xA0_AND_A_B;
        case 0xA1: return &Opcode_xA1_AND_A_C;
        case 0xA2: return &Opcode_xA2_AND_A_D;
        case 0xA3: return &Opcode_xA3_AND_A_E;
        case 0xA4: return &Opcode_xA4_AND_A_H;
        case 0xA5: return &Opcode_xA5_AND_A_L;
        case 0xA6: return &Opcode_xA6_AND_A_HL;
        case 0xA7: return &Opcode_xA7_AND_A_A;
        case 0xA8: return &Opcode_xA8_XOR_A_B;
        case 0xA9: return &Opcode_xA9_XOR_A_C;
        case 0xAA: return &Opcode_xAA_XOR_A_D;
        case 0xAB: return &Opcode_xAB_XOR_A_E;
        case 0xAC: return &Opcode_xAC_XOR_A_H;
        case 0xAD: return &Opcode_xAD_XOR_A_L;
        case 0xAE: return &Opcode_xAE_XOR_A_HL;
        case 0xAF: return &Opcode_xAF_XOR_A_A;
        case 0xB0: return &Opcode_xB0_OR_A_B;
        case 0xB1: return &Opcode_xB1_OR_A_C;
        case 0xB2: return &Opcode_xB2_OR_A_D;
        case 0xB3: return &Opcode_xB3_OR_A_E;
        case 0xB4: return &Opcode_xB4_OR_A_H;
        case 0xB5: return &Opcode_xB5_OR_A_L;
        case 0xB6: return &Opcode_xB6_OR_A_HL;
        case 0xB7: return &Opcode_xB7_OR_A_A;
        case 0xB8: return &Opcode_xB8_CP_A_B;
        case 0xB9: return &Opcode_xB9_CP_A_C;
        case 0xBA: return &Opcode_xBA_CP_A_D;
        case 0xBB: return &Opcode_xBB_CP_A_E;
        case 0xBC: return &Opcode_xBC_CP_A_H;
        case 0xBD: return &Opcode_xBD_CP_A_L;
        case 0xBE: return &Opcode_xBE_CP_A_HL;
        case 0xBF: return &Opcode_xBF_CP_A_A;
        case 0xC0: return &Opcode_xC0_RET_NZ;
        case 0xC1: return &Opcode_xC1_PUSH_BC;
        case 0xC2: return &Opcode_xC2_JP_NZ;
        case 0xC3: return &Opcode_xC3_JP;
        case 0xC4: return &Opcode_xC4_CALL_NZ;
        case 0xC5: return &Opcode_xC5_POP_BC;

        case 0xC8: return &Opcode_xC8_RET_Z;
        case 0xC9: return &Opcode_xC9_RET;
        case 0xCA: return &Opcode_xCA_JP_Z;

        case 0xCC: return &Opcode_xCC_CALL_Z;
        case 0xCD: return &Opcode_xCD_CALL;

        case 0xD0: return &Opcode_xD0_RET_NC;
        case 0xD1: return &Opcode_xD1_PUSH_DE;
        case 0xD2: return &Opcode_xD2_JP_NC;
        case 0xD3: return &Opcode_xD3_INVALID;
        case 0xD4: return &Opcode_xD4_CALL_NC;
        case 0xD5: return &Opcode_xD5_POP_DE;

        case 0xD8: return &Opcode_xD8_RET_C;
        case 0xD9: return &Opcode_xD9_RETI;
        case 0xDA: return &Opcode_xDA_JP_C;
        case 0xDB: return &Opcode_xDB_INVALID;
        case 0xDC: return &Opcode_xDC_CALL_C;
        case 0xDD: return &Opcode_xDD_INVALID;

        case 0xE0: return &Opcode_xE0_LDH_A8_A;
        case 0xE1: return &Opcode_xE1_PUSH_HL;
        case 0xE2: return &Opcode_xE2_LDH_C_A;
        case 0xE3: return &Opcode_xE3_INVALID;
        case 0xE4: return &Opcode_xE4_INVALID;

        case 0xE5: return &Opcode_xE5_POP_HL;

        case 0xEB: return &Opcode_xEB_INVALID;
        case 0xEC: return &Opcode_xEC_INVALID;
        case 0xED: return &Opcode_xED_INVALID;

        case 0xF0: return &Opcode_xF0_LDH_A_A8;
        case 0xF1: return &Opcode_xF1_PUSH_AF;
        case 0xF2: return &Opcode_xF2_LDH_A_C;
        case 0xF3: return &Opcode_xF3_DI;
        case 0xF4: return &Opcode_xF4_INVALID;
        case 0xF5: return &Opcode_xF5_POP_AF;

        case 0xFB: return &Opcode_xFB_EI;
        case 0xFC: return &Opcode_xFC_INVALID;
        case 0xFD: return &Opcode_xFD_INVALID;

        default: return &Opcode_xZZ_UNIMPLEMENTED;
    }
};
void DMG_CPU::clearOpcode() {
    m_curr_opcode = nullptr;
};
void DMG_CPU::runMCycle() {
    // Interrupts
    m_Memory.handleIME();

    if (!m_stopped && !m_halted) {
        if (!m_curr_opcode || m_curr_opcode_mcycle >= m_curr_opcode->length) {
            uint8_t opcode = PC_Eat_Byte();
            if (m_halt_bug) { // Causes double fetch
                m_regs.set(Reg_u16::PC, m_regs.get(Reg_u16::PC) - 1);
                m_halt_bug = false;
            }
            m_curr_opcode = parseOpcode(opcode);
            // m_curr_opcode_length = m_curr_opcode->length; // For the variable length opcodes
            m_curr_opcode_mcycle = 0;
        }

        (m_curr_opcode->mcycles[m_curr_opcode_mcycle])(*this);
        m_curr_opcode_mcycle += 1;

        m_Memory.freeBus();
    }
    // Timers
    // PPU
    // APU
    m_cycle_count += 4;
};
void DMG_CPU::runTCycle() {
    // Optional later expansion
};

void DMG_CPU::callAbort() {
    m_abort = true;
};

// void DMG_CPU::consumeCycles(int cycles) { // Flip the system and splice the opcodes by cycle (or T-cycle accurate?)
//     m_cycle_count += cycles;

//     // pollEvents(); // Not so often
//     // m_Screen.runCycles(cycles, m_cycle_count); // Possibly not so often
//     // m_Sound.runCycles(cycles, m_cycle_count);

//     // if m_cycle_count > c_ONE_FRAME: m_Screen.presentRenderer(), m_cycle_count -= c_ONE_FRAME

//     // now = timeNS // Possibly avoid sleeping
//     // gap = last+c_4_CYCLES - now
//     // if gap > 0: delay(gap)
//     // last += c_4_CYCLES
// };


uint8_t DMG_CPU::PC_Eat_Byte() {
    m_Memory.latchBus(m_regs.get(Reg_u16::PC));
    uint8_t ret = m_Memory.Read(); // Read from <PC>
    m_regs.set(Reg_u16::PC, m_regs.get(Reg_u16::PC) + 1); // Increment PC
    return ret;
};
uint8_t DMG_CPU::ALU_B8_ADDER(uint8_t num1, uint8_t num2, uint8_t carry_bit) {
    uint8_t half = (num1 & 0xf) + (num2 & 0xf) + carry_bit; // For half carry
    uint16_t unmasked = static_cast<uint16_t>(num1) + static_cast<uint16_t>(num2) + static_cast<uint16_t>(carry_bit); // For carry
    uint8_t masked = static_cast<uint8_t>(unmasked & 0xff);
    m_regs.set(Reg_flag::Z, (masked == 0));
    m_regs.set(Reg_flag::N, false);
    m_regs.set(Reg_flag::H, ((half >> 4) & 0b1 == 1));
    m_regs.set(Reg_flag::C, ((unmasked >> 8) & 0b1 == 1));
    return masked;
};
uint8_t DMG_CPU::ALU_B8_SUBBER(uint8_t num1, uint8_t num2, uint8_t carry_bit) {
    // uint8_t half = (num1 & 0xf) - (num2 & 0xf) - carry_bit; // For half carry
    uint16_t unmasked = static_cast<uint16_t>(num1) - static_cast<uint16_t>(num2) - static_cast<uint16_t>(carry_bit); // For carry
    uint8_t masked = static_cast<uint8_t>(unmasked & 0xff);
    m_regs.set(Reg_flag::Z, (masked == 0));
    m_regs.set(Reg_flag::N, true);
    m_regs.set(Reg_flag::H, (num1 & 0xf) < ((num2 & 0xf) + carry_bit));
    m_regs.set(Reg_flag::C, num1 < (num2 + carry_bit));
    return masked;
};
void DMG_CPU::STOP() {
    m_stopped = true;
};
void DMG_CPU::HALT() {
    m_halted = true;
};
void DMG_CPU::HALT_Bug() {
    m_halt_bug = true;
};
bool DMG_CPU::WAKE() {
    return false; // Returns true if any buttons are pressed
};
// void DMG_CPU::PUSH_B16(Reg_u8 reg_H, Reg_u8 reg_L) {
//     m_regs.set(Reg_u16::SP, m_regs.get(Reg_u16::SP) - 1); // Decrement SP
//     m_Memory.Write(m_regs.get(Reg_u16::SP), m_regs.get(reg_H)); // Write High
//     m_regs.set(Reg_u16::SP, m_regs.get(Reg_u16::SP) - 1); // Decrement SP
//     m_Memory.Write(m_regs.get(Reg_u16::SP), m_regs.get(reg_L)); // Write Low
// };
// void DMG_CPU::POP_B16(Reg_u8 reg_H, Reg_u8 reg_L) {
//     m_regs.set(Reg_u16::SP, m_regs.get(Reg_u16::SP) + 1); // Increment SP
//     m_regs.set(reg_L, m_Memory.Read(m_regs.get(Reg_u16::SP))); // Read Low
//     m_regs.set(Reg_u16::SP, m_regs.get(Reg_u16::SP) + 1); // Increment SP
//     m_regs.set(reg_H, m_Memory.Read(m_regs.get(Reg_u16::SP))); // Read High
// };
