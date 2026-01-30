
#include "cpu.h"

#include "../enable_logging.h"
#include "emulator_log.h"


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

Cart_Details DMG_CPU::Trigger_InitializeAll(std::vector<uint8_t>* rom, Emulator_Options* options) {
    Cart_Details cart_details = m_Memory.Initialize(rom, options->m_run_boot_rom);
    Initialize(options->m_run_boot_rom, options->m_log_length, options->m_log_enable);
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

void DMG_CPU::Initialize(bool use_boot_rom, int log_length, bool log_enable) {
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
    m_logfile.setOptions(log_length, log_enable);
    m_log_enable = log_enable;
    m_logfile.println("Emulator Initialized");
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
    m_logfile.clearLog();
};
void DMG_CPU::UpdateSettings(Emulator_Options* options) {
    m_log_enable = options->m_log_enable;
    m_logfile.setOptions(options->m_log_length, options->m_log_enable);
};


Opcode* DMG_CPU::parseOpcode(uint8_t opcode) {
    // if (opcode == 0xFF) {
    //     callAbort();
    // }
    switch (opcode) {
        case 0x00: return &Opcode_x00_NOP;
        case 0x01: return &Opcode_x01_LD_BC_N16;
        case 0x02: return &Opcode_x02_LD_BC_A;
        case 0x03: return &Opcode_x03_INC_BC;
        case 0x04: return &Opcode_x04_INC_B;
        case 0x05: return &Opcode_x05_DEC_B;
        case 0x06: return &Opcode_x06_LD_B_N8;
        case 0x07: return &Opcode_x07_RLCA;
        case 0x08: return &Opcode_x08_LD_NN_SP;
        case 0x09: return &Opcode_x09_ADD_HL_BC;
        case 0x0A: return &Opcode_x0A_LD_A_BC;
        case 0x0B: return &Opcode_x0B_DEC_BC;
        case 0x0C: return &Opcode_x0C_INC_C;
        case 0x0D: return &Opcode_x0D_DEC_C;
        case 0x0E: return &Opcode_x0E_LD_C_N8;
        case 0x0F: return &Opcode_x0F_RRCA;
        case 0x10: return &Opcode_x10_STOP;
        case 0x11: return &Opcode_x11_LD_DE_N16;
        case 0x12: return &Opcode_x12_LD_DE_A;
        case 0x13: return &Opcode_x13_INC_DE;
        case 0x14: return &Opcode_x14_INC_D;
        case 0x15: return &Opcode_x15_DEC_D;
        case 0x16: return &Opcode_x16_LD_D_N8;
        case 0x17: return &Opcode_x17_RLA;
        case 0x18: return &Opcode_x18_JR;
        case 0x19: return &Opcode_x19_ADD_HL_DE;
        case 0x1A: return &Opcode_x1A_LD_A_DE;
        case 0x1B: return &Opcode_x1B_DEC_DE;
        case 0x1C: return &Opcode_x1C_INC_E;
        case 0x1D: return &Opcode_x1D_DEC_E;
        case 0x1E: return &Opcode_x1E_LD_E_N8;
        case 0x1F: return &Opcode_x1F_RRA;
        case 0x20: return &Opcode_x20_JR_NZ;
        case 0x21: return &Opcode_x21_LD_HL_N16;
        case 0x22: return &Opcode_x22_LD_HLI_A;
        case 0x23: return &Opcode_x23_INC_HL;
        case 0x24: return &Opcode_x24_INC_H;
        case 0x25: return &Opcode_x25_DEC_H;
        case 0x26: return &Opcode_x26_LD_H_N8;
        case 0x27: return &Opcode_x27_DAA;
        case 0x28: return &Opcode_x28_JR_Z;
        case 0x29: return &Opcode_x29_ADD_HL_HL;
        case 0x2A: return &Opcode_x2A_LD_A_HLI;
        case 0x2B: return &Opcode_x2B_DEC_HL;
        case 0x2C: return &Opcode_x2C_INC_L;
        case 0x2D: return &Opcode_x2D_DEC_L;
        case 0x2E: return &Opcode_x2E_LD_L_N8;
        case 0x2F: return &Opcode_x2F_CPL;
        case 0x30: return &Opcode_x30_JR_NC;
        case 0x31: return &Opcode_x31_LD_SP_N16;
        case 0x32: return &Opcode_x32_LD_HLD_A;
        case 0x33: return &Opcode_x33_INC_SP;
        case 0x34: return &Opcode_x34_INC_HL;
        case 0x35: return &Opcode_x35_DEC_HL;
        case 0x36: return &Opcode_x36_LD_HL_N8;
        case 0x37: return &Opcode_x37_SCF;
        case 0x38: return &Opcode_x38_JR_C;
        case 0x39: return &Opcode_x39_ADD_HL_SP;
        case 0x3A: return &Opcode_x3A_LD_A_HLD;
        case 0x3B: return &Opcode_x3B_DEC_SP;
        case 0x3C: return &Opcode_x3C_INC_A;
        case 0x3D: return &Opcode_x3D_DEC_A;
        case 0x3E: return &Opcode_x3E_LD_A_N8;
        case 0x3F: return &Opcode_x3F_CCF;
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
        case 0xC1: return &Opcode_xC1_POP_BC;
        case 0xC2: return &Opcode_xC2_JP_NZ;
        case 0xC3: return &Opcode_xC3_JP;
        case 0xC4: return &Opcode_xC4_CALL_NZ;
        case 0xC5: return &Opcode_xC5_PUSH_BC;
        case 0xC6: return &Opcode_xC6_ADD_A_N8;
        case 0xC7: return &Opcode_xC7_RST_00;
        case 0xC8: return &Opcode_xC8_RET_Z;
        case 0xC9: return &Opcode_xC9_RET;
        case 0xCA: return &Opcode_xCA_JP_Z;
        case 0xCB: return &Opcode_xCB_CB_PREFIX;
        case 0xCC: return &Opcode_xCC_CALL_Z;
        case 0xCD: return &Opcode_xCD_CALL;
        case 0xCE: return &Opcode_xCE_ADC_A_N8;
        case 0xCF: return &Opcode_xCF_RST_08;
        case 0xD0: return &Opcode_xD0_RET_NC;
        case 0xD1: return &Opcode_xD1_POP_DE;
        case 0xD2: return &Opcode_xD2_JP_NC;
        case 0xD3: return &Opcode_xD3_INVALID;
        case 0xD4: return &Opcode_xD4_CALL_NC;
        case 0xD5: return &Opcode_xD5_PUSH_DE;
        case 0xD6: return &Opcode_xD6_SUB_A_N8;
        case 0xD7: return &Opcode_xD7_RST_10;
        case 0xD8: return &Opcode_xD8_RET_C;
        case 0xD9: return &Opcode_xD9_RETI;
        case 0xDA: return &Opcode_xDA_JP_C;
        case 0xDB: return &Opcode_xDB_INVALID;
        case 0xDC: return &Opcode_xDC_CALL_C;
        case 0xDD: return &Opcode_xDD_INVALID;
        case 0xDE: return &Opcode_xDE_SBC_A_N8;
        case 0xDF: return &Opcode_xDF_RST_18;
        case 0xE0: return &Opcode_xE0_LDH_A8_A;
        case 0xE1: return &Opcode_xE1_POP_HL;
        case 0xE2: return &Opcode_xE2_LDH_C_A;
        case 0xE3: return &Opcode_xE3_INVALID;
        case 0xE4: return &Opcode_xE4_INVALID;
        case 0xE5: return &Opcode_xE5_PUSH_HL;
        case 0xE6: return &Opcode_xE6_AND_A_N8;
        case 0xE7: return &Opcode_xE7_RST_20;
        case 0xE8: return &Opcode_xE8_ADD_SP_N;
        case 0xE9: return &Opcode_xE9_JP_HL;
        case 0xEA: return &Opcode_xEA_LD_N16_A;
        case 0xEB: return &Opcode_xEB_INVALID;
        case 0xEC: return &Opcode_xEC_INVALID;
        case 0xED: return &Opcode_xED_INVALID;
        case 0xEE: return &Opcode_xEE_XOR_A_N8;
        case 0xEF: return &Opcode_xEF_RST_28;
        case 0xF0: return &Opcode_xF0_LDH_A_A8;
        case 0xF1: return &Opcode_xF1_POP_AF;
        case 0xF2: return &Opcode_xF2_LDH_A_C;
        case 0xF3: return &Opcode_xF3_DI;
        case 0xF4: return &Opcode_xF4_INVALID;
        case 0xF5: return &Opcode_xF5_PUSH_AF;
        case 0xF6: return &Opcode_xF6_OR_A_N8;
        case 0xF7: return &Opcode_xF7_RST_30;
        case 0xF8: return &Opcode_xF8_LD_HL_SP_N;
        case 0xF9: return &Opcode_xF9_LD_SP_HL;
        case 0xFA: return &Opcode_xFA_LD_A_N16;
        case 0xFB: return &Opcode_xFB_EI;
        case 0xFC: return &Opcode_xFC_INVALID;
        case 0xFD: return &Opcode_xFD_INVALID;
        case 0xFE: return &Opcode_xFE_CP_A_N8;
        case 0xFF: return &Opcode_xFF_RST_38;
        default: return &Opcode_xZZ_UNIMPLEMENTED;
    }
};
Opcode* DMG_CPU::parseCBOpcode(uint8_t opcode) {
    switch (opcode) {
        case 0x00: return &Opcode_CB_x00_RLC_B;
        case 0x01: return &Opcode_CB_x01_RLC_C;
        case 0x02: return &Opcode_CB_x02_RLC_D;
        case 0x03: return &Opcode_CB_x03_RLC_E;
        case 0x04: return &Opcode_CB_x04_RLC_H;
        case 0x05: return &Opcode_CB_x05_RLC_L;
        case 0x06: return &Opcode_CB_x06_RLC_HL;
        case 0x07: return &Opcode_CB_x07_RLC_A;
        case 0x08: return &Opcode_CB_x08_RRC_B;
        case 0x09: return &Opcode_CB_x09_RRC_C;
        case 0x0A: return &Opcode_CB_x0A_RRC_D;
        case 0x0B: return &Opcode_CB_x0B_RRC_E;
        case 0x0C: return &Opcode_CB_x0C_RRC_H;
        case 0x0D: return &Opcode_CB_x0D_RRC_L;
        case 0x0E: return &Opcode_CB_x0E_RRC_HL;
        case 0x0F: return &Opcode_CB_x0F_RRC_A;
        case 0x10: return &Opcode_CB_x10_RL_B;
        case 0x11: return &Opcode_CB_x11_RL_C;
        case 0x12: return &Opcode_CB_x12_RL_D;
        case 0x13: return &Opcode_CB_x13_RL_E;
        case 0x14: return &Opcode_CB_x14_RL_H;
        case 0x15: return &Opcode_CB_x15_RL_L;
        case 0x16: return &Opcode_CB_x16_RL_HL;
        case 0x17: return &Opcode_CB_x17_RL_A;
        case 0x18: return &Opcode_CB_x18_RR_B;
        case 0x19: return &Opcode_CB_x19_RR_C;
        case 0x1A: return &Opcode_CB_x1A_RR_D;
        case 0x1B: return &Opcode_CB_x1B_RR_E;
        case 0x1C: return &Opcode_CB_x1C_RR_H;
        case 0x1D: return &Opcode_CB_x1D_RR_L;
        case 0x1E: return &Opcode_CB_x1E_RR_HL;
        case 0x1F: return &Opcode_CB_x1F_RR_A;
        case 0x20: return &Opcode_CB_x20_SLA_B;
        case 0x21: return &Opcode_CB_x21_SLA_C;
        case 0x22: return &Opcode_CB_x22_SLA_D;
        case 0x23: return &Opcode_CB_x23_SLA_E;
        case 0x24: return &Opcode_CB_x24_SLA_H;
        case 0x25: return &Opcode_CB_x25_SLA_L;
        case 0x26: return &Opcode_CB_x26_SLA_HL;
        case 0x27: return &Opcode_CB_x27_SLA_A;
        case 0x28: return &Opcode_CB_x28_SRA_B;
        case 0x29: return &Opcode_CB_x29_SRA_C;
        case 0x2A: return &Opcode_CB_x2A_SRA_D;
        case 0x2B: return &Opcode_CB_x2B_SRA_E;
        case 0x2C: return &Opcode_CB_x2C_SRA_H;
        case 0x2D: return &Opcode_CB_x2D_SRA_L;
        case 0x2E: return &Opcode_CB_x2E_SRA_HL;
        case 0x2F: return &Opcode_CB_x2F_SRA_A;
        case 0x30: return &Opcode_CB_x30_SWAP_B;
        case 0x31: return &Opcode_CB_x31_SWAP_C;
        case 0x32: return &Opcode_CB_x32_SWAP_D;
        case 0x33: return &Opcode_CB_x33_SWAP_E;
        case 0x34: return &Opcode_CB_x34_SWAP_H;
        case 0x35: return &Opcode_CB_x35_SWAP_L;
        case 0x36: return &Opcode_CB_x36_SWAP_HL;
        case 0x37: return &Opcode_CB_x37_SWAP_A;
        case 0x38: return &Opcode_CB_x38_SRL_B;
        case 0x39: return &Opcode_CB_x39_SRL_C;
        case 0x3A: return &Opcode_CB_x3A_SRL_D;
        case 0x3B: return &Opcode_CB_x3B_SRL_E;
        case 0x3C: return &Opcode_CB_x3C_SRL_H;
        case 0x3D: return &Opcode_CB_x3D_SRL_L;
        case 0x3E: return &Opcode_CB_x3E_SRL_HL;
        case 0x3F: return &Opcode_CB_x3F_SRL_A;
        case 0x40: return &Opcode_CB_x40_BIT_0_B;
        case 0x41: return &Opcode_CB_x41_BIT_0_C;
        case 0x42: return &Opcode_CB_x42_BIT_0_D;
        case 0x43: return &Opcode_CB_x43_BIT_0_E;
        case 0x44: return &Opcode_CB_x44_BIT_0_H;
        case 0x45: return &Opcode_CB_x45_BIT_0_L;
        case 0x46: return &Opcode_CB_x46_BIT_0_HL;
        case 0x47: return &Opcode_CB_x47_BIT_0_A;
        case 0x48: return &Opcode_CB_x48_BIT_1_B;
        case 0x49: return &Opcode_CB_x49_BIT_1_C;
        case 0x4A: return &Opcode_CB_x4A_BIT_1_D;
        case 0x4B: return &Opcode_CB_x4B_BIT_1_E;
        case 0x4C: return &Opcode_CB_x4C_BIT_1_H;
        case 0x4D: return &Opcode_CB_x4D_BIT_1_L;
        case 0x4E: return &Opcode_CB_x4E_BIT_1_HL;
        case 0x4F: return &Opcode_CB_x4F_BIT_1_A;
        case 0x50: return &Opcode_CB_x50_BIT_2_B;
        case 0x51: return &Opcode_CB_x51_BIT_2_C;
        case 0x52: return &Opcode_CB_x52_BIT_2_D;
        case 0x53: return &Opcode_CB_x53_BIT_2_E;
        case 0x54: return &Opcode_CB_x54_BIT_2_H;
        case 0x55: return &Opcode_CB_x55_BIT_2_L;
        case 0x56: return &Opcode_CB_x56_BIT_2_HL;
        case 0x57: return &Opcode_CB_x57_BIT_2_A;
        case 0x58: return &Opcode_CB_x58_BIT_3_B;
        case 0x59: return &Opcode_CB_x59_BIT_3_C;
        case 0x5A: return &Opcode_CB_x5A_BIT_3_D;
        case 0x5B: return &Opcode_CB_x5B_BIT_3_E;
        case 0x5C: return &Opcode_CB_x5C_BIT_3_H;
        case 0x5D: return &Opcode_CB_x5D_BIT_3_L;
        case 0x5E: return &Opcode_CB_x5E_BIT_3_HL;
        case 0x5F: return &Opcode_CB_x5F_BIT_3_A;
        case 0x60: return &Opcode_CB_x60_BIT_4_B;
        case 0x61: return &Opcode_CB_x61_BIT_4_C;
        case 0x62: return &Opcode_CB_x62_BIT_4_D;
        case 0x63: return &Opcode_CB_x63_BIT_4_E;
        case 0x64: return &Opcode_CB_x64_BIT_4_H;
        case 0x65: return &Opcode_CB_x65_BIT_4_L;
        case 0x66: return &Opcode_CB_x66_BIT_4_HL;
        case 0x67: return &Opcode_CB_x67_BIT_4_A;
        case 0x68: return &Opcode_CB_x68_BIT_5_B;
        case 0x69: return &Opcode_CB_x69_BIT_5_C;
        case 0x6A: return &Opcode_CB_x6A_BIT_5_D;
        case 0x6B: return &Opcode_CB_x6B_BIT_5_E;
        case 0x6C: return &Opcode_CB_x6C_BIT_5_H;
        case 0x6D: return &Opcode_CB_x6D_BIT_5_L;
        case 0x6E: return &Opcode_CB_x6E_BIT_5_HL;
        case 0x6F: return &Opcode_CB_x6F_BIT_5_A;
        case 0x70: return &Opcode_CB_x70_BIT_6_B;
        case 0x71: return &Opcode_CB_x71_BIT_6_C;
        case 0x72: return &Opcode_CB_x72_BIT_6_D;
        case 0x73: return &Opcode_CB_x73_BIT_6_E;
        case 0x74: return &Opcode_CB_x74_BIT_6_H;
        case 0x75: return &Opcode_CB_x75_BIT_6_L;
        case 0x76: return &Opcode_CB_x76_BIT_6_HL;
        case 0x77: return &Opcode_CB_x77_BIT_6_A;
        case 0x78: return &Opcode_CB_x78_BIT_7_B;
        case 0x79: return &Opcode_CB_x79_BIT_7_C;
        case 0x7A: return &Opcode_CB_x7A_BIT_7_D;
        case 0x7B: return &Opcode_CB_x7B_BIT_7_E;
        case 0x7C: return &Opcode_CB_x7C_BIT_7_H;
        case 0x7D: return &Opcode_CB_x7D_BIT_7_L;
        case 0x7E: return &Opcode_CB_x7E_BIT_7_HL;
        case 0x7F: return &Opcode_CB_x7F_BIT_7_A;
        case 0x80: return &Opcode_CB_x80_RES_0_B;
        case 0x81: return &Opcode_CB_x81_RES_0_C;
        case 0x82: return &Opcode_CB_x82_RES_0_D;
        case 0x83: return &Opcode_CB_x83_RES_0_E;
        case 0x84: return &Opcode_CB_x84_RES_0_H;
        case 0x85: return &Opcode_CB_x85_RES_0_L;
        case 0x86: return &Opcode_CB_x86_RES_0_HL;
        case 0x87: return &Opcode_CB_x87_RES_0_A;
        case 0x88: return &Opcode_CB_x88_RES_1_B;
        case 0x89: return &Opcode_CB_x89_RES_1_C;
        case 0x8A: return &Opcode_CB_x8A_RES_1_D;
        case 0x8B: return &Opcode_CB_x8B_RES_1_E;
        case 0x8C: return &Opcode_CB_x8C_RES_1_H;
        case 0x8D: return &Opcode_CB_x8D_RES_1_L;
        case 0x8E: return &Opcode_CB_x8E_RES_1_HL;
        case 0x8F: return &Opcode_CB_x8F_RES_1_A;
        case 0x90: return &Opcode_CB_x90_RES_2_B;
        case 0x91: return &Opcode_CB_x91_RES_2_C;
        case 0x92: return &Opcode_CB_x92_RES_2_D;
        case 0x93: return &Opcode_CB_x93_RES_2_E;
        case 0x94: return &Opcode_CB_x94_RES_2_H;
        case 0x95: return &Opcode_CB_x95_RES_2_L;
        case 0x96: return &Opcode_CB_x96_RES_2_HL;
        case 0x97: return &Opcode_CB_x97_RES_2_A;
        case 0x98: return &Opcode_CB_x98_RES_3_B;
        case 0x99: return &Opcode_CB_x99_RES_3_C;
        case 0x9A: return &Opcode_CB_x9A_RES_3_D;
        case 0x9B: return &Opcode_CB_x9B_RES_3_E;
        case 0x9C: return &Opcode_CB_x9C_RES_3_H;
        case 0x9D: return &Opcode_CB_x9D_RES_3_L;
        case 0x9E: return &Opcode_CB_x9E_RES_3_HL;
        case 0x9F: return &Opcode_CB_x9F_RES_3_A;
        case 0xA0: return &Opcode_CB_xA0_RES_4_B;
        case 0xA1: return &Opcode_CB_xA1_RES_4_C;
        case 0xA2: return &Opcode_CB_xA2_RES_4_D;
        case 0xA3: return &Opcode_CB_xA3_RES_4_E;
        case 0xA4: return &Opcode_CB_xA4_RES_4_H;
        case 0xA5: return &Opcode_CB_xA5_RES_4_L;
        case 0xA6: return &Opcode_CB_xA6_RES_4_HL;
        case 0xA7: return &Opcode_CB_xA7_RES_4_A;
        case 0xA8: return &Opcode_CB_xA8_RES_5_B;
        case 0xA9: return &Opcode_CB_xA9_RES_5_C;
        case 0xAA: return &Opcode_CB_xAA_RES_5_D;
        case 0xAB: return &Opcode_CB_xAB_RES_5_E;
        case 0xAC: return &Opcode_CB_xAC_RES_5_H;
        case 0xAD: return &Opcode_CB_xAD_RES_5_L;
        case 0xAE: return &Opcode_CB_xAE_RES_5_HL;
        case 0xAF: return &Opcode_CB_xAF_RES_5_A;
        case 0xB0: return &Opcode_CB_xB0_RES_6_B;
        case 0xB1: return &Opcode_CB_xB1_RES_6_C;
        case 0xB2: return &Opcode_CB_xB2_RES_6_D;
        case 0xB3: return &Opcode_CB_xB3_RES_6_E;
        case 0xB4: return &Opcode_CB_xB4_RES_6_H;
        case 0xB5: return &Opcode_CB_xB5_RES_6_L;
        case 0xB6: return &Opcode_CB_xB6_RES_6_HL;
        case 0xB7: return &Opcode_CB_xB7_RES_6_A;
        case 0xB8: return &Opcode_CB_xB8_RES_7_B;
        case 0xB9: return &Opcode_CB_xB9_RES_7_C;
        case 0xBA: return &Opcode_CB_xBA_RES_7_D;
        case 0xBB: return &Opcode_CB_xBB_RES_7_E;
        case 0xBC: return &Opcode_CB_xBC_RES_7_H;
        case 0xBD: return &Opcode_CB_xBD_RES_7_L;
        case 0xBE: return &Opcode_CB_xBE_RES_7_HL;
        case 0xBF: return &Opcode_CB_xBF_RES_7_A;
        case 0xC0: return &Opcode_CB_xC0_SET_0_B;
        case 0xC1: return &Opcode_CB_xC1_SET_0_C;
        case 0xC2: return &Opcode_CB_xC2_SET_0_D;
        case 0xC3: return &Opcode_CB_xC3_SET_0_E;
        case 0xC4: return &Opcode_CB_xC4_SET_0_H;
        case 0xC5: return &Opcode_CB_xC5_SET_0_L;
        case 0xC6: return &Opcode_CB_xC6_SET_0_HL;
        case 0xC7: return &Opcode_CB_xC7_SET_0_A;
        case 0xC8: return &Opcode_CB_xC8_SET_1_B;
        case 0xC9: return &Opcode_CB_xC9_SET_1_C;
        case 0xCA: return &Opcode_CB_xCA_SET_1_D;
        case 0xCB: return &Opcode_CB_xCB_SET_1_E;
        case 0xCC: return &Opcode_CB_xCC_SET_1_H;
        case 0xCD: return &Opcode_CB_xCD_SET_1_L;
        case 0xCE: return &Opcode_CB_xCE_SET_1_HL;
        case 0xCF: return &Opcode_CB_xCF_SET_1_A;
        case 0xD0: return &Opcode_CB_xD0_SET_2_B;
        case 0xD1: return &Opcode_CB_xD1_SET_2_C;
        case 0xD2: return &Opcode_CB_xD2_SET_2_D;
        case 0xD3: return &Opcode_CB_xD3_SET_2_E;
        case 0xD4: return &Opcode_CB_xD4_SET_2_H;
        case 0xD5: return &Opcode_CB_xD5_SET_2_L;
        case 0xD6: return &Opcode_CB_xD6_SET_2_HL;
        case 0xD7: return &Opcode_CB_xD7_SET_2_A;
        case 0xD8: return &Opcode_CB_xD8_SET_3_B;
        case 0xD9: return &Opcode_CB_xD9_SET_3_C;
        case 0xDA: return &Opcode_CB_xDA_SET_3_D;
        case 0xDB: return &Opcode_CB_xDB_SET_3_E;
        case 0xDC: return &Opcode_CB_xDC_SET_3_H;
        case 0xDD: return &Opcode_CB_xDD_SET_3_L;
        case 0xDE: return &Opcode_CB_xDE_SET_3_HL;
        case 0xDF: return &Opcode_CB_xDF_SET_3_A;
        case 0xE0: return &Opcode_CB_xE0_SET_4_B;
        case 0xE1: return &Opcode_CB_xE1_SET_4_C;
        case 0xE2: return &Opcode_CB_xE2_SET_4_D;
        case 0xE3: return &Opcode_CB_xE3_SET_4_E;
        case 0xE4: return &Opcode_CB_xE4_SET_4_H;
        case 0xE5: return &Opcode_CB_xE5_SET_4_L;
        case 0xE6: return &Opcode_CB_xE6_SET_4_HL;
        case 0xE7: return &Opcode_CB_xE7_SET_4_A;
        case 0xE8: return &Opcode_CB_xE8_SET_5_B;
        case 0xE9: return &Opcode_CB_xE9_SET_5_C;
        case 0xEA: return &Opcode_CB_xEA_SET_5_D;
        case 0xEB: return &Opcode_CB_xEB_SET_5_E;
        case 0xEC: return &Opcode_CB_xEC_SET_5_H;
        case 0xED: return &Opcode_CB_xED_SET_5_L;
        case 0xEE: return &Opcode_CB_xEE_SET_5_HL;
        case 0xEF: return &Opcode_CB_xEF_SET_5_A;
        case 0xF0: return &Opcode_CB_xF0_SET_6_B;
        case 0xF1: return &Opcode_CB_xF1_SET_6_C;
        case 0xF2: return &Opcode_CB_xF2_SET_6_D;
        case 0xF3: return &Opcode_CB_xF3_SET_6_E;
        case 0xF4: return &Opcode_CB_xF4_SET_6_H;
        case 0xF5: return &Opcode_CB_xF5_SET_6_L;
        case 0xF6: return &Opcode_CB_xF6_SET_6_HL;
        case 0xF7: return &Opcode_CB_xF7_SET_6_A;
        case 0xF8: return &Opcode_CB_xF8_SET_7_B;
        case 0xF9: return &Opcode_CB_xF9_SET_7_C;
        case 0xFA: return &Opcode_CB_xFA_SET_7_D;
        case 0xFB: return &Opcode_CB_xFB_SET_7_E;
        case 0xFC: return &Opcode_CB_xFC_SET_7_H;
        case 0xFD: return &Opcode_CB_xFD_SET_7_L;
        case 0xFE: return &Opcode_CB_xFE_SET_7_HL;
        case 0xFF: return &Opcode_CB_xFF_SET_7_A;
        default: return &Opcode_xZZ_UNIMPLEMENTED_CB;
    }
};
void DMG_CPU::clearOpcode() {
    m_curr_opcode = nullptr;
    m_curr_opcode_mcycle = 0;
};
void DMG_CPU::loadCBOpcode() {
    // clearOpcode();
    // uint8_t opcode = PC_Eat_Byte();
    // m_curr_opcode = parseCBOpcode(opcode);
    m_read_cb_opcode = true;
};
void DMG_CPU::runMCycle() {
    if (!m_abort) {
        // Interrupts
        m_Memory.handleIME();

        if (!m_stopped && !m_halted) {
            if (!m_curr_opcode || m_curr_opcode_mcycle >= m_curr_opcode->length) {
#ifdef DEBUG_LOGGING
                if (m_log_enable) {
                    m_logfile.dumpLine(); // Print the log for the previous instruction
                }
#endif

                uint8_t opcode = PC_Eat_Byte();
#ifdef DEBUG_LOGGING
//                 printf("Opcode Run: PC: %s, Opcode: %s\n", to_b16_out(m_regs.get(Reg_u16::PC)).c_str(), to_b8_out(opcode).c_str());
                if (m_log_enable) {
                    m_logfile.print(std::string("Opcode: ")+(m_read_cb_opcode ? "0xCB " : "")+"0x"+to_b8_out(opcode)+", "
                            +(!m_read_cb_opcode ? getOpcodeName(opcode) : getCBOpcodeName(opcode))+"; ");
                    m_logfile.print(m_regs.dumpState() + "; ");
                }
#endif
                if (m_halt_bug) { // Causes double fetch
                    m_regs.set(Reg_u16::PC, m_regs.get(Reg_u16::PC) - 1);
                    m_halt_bug = false;
                }
                if (!m_read_cb_opcode) {
                    m_curr_opcode = parseOpcode(opcode);
                }else {
                    m_curr_opcode = parseCBOpcode(opcode);
                    m_read_cb_opcode = false;
                }
                // m_curr_opcode_length = m_curr_opcode->length; // For the variable length opcodes
                m_curr_opcode_mcycle = 0;
            }

            (m_curr_opcode->mcycles[m_curr_opcode_mcycle])(*this);
            m_curr_opcode_mcycle += 1;

            m_Memory.freeBus();
#ifdef DEBUG_LOGGING
            if (m_log_enable) {
                m_logfile.print(std::string("($M-Cycle) "));
            }
#endif
        }
        // Timers
        // PPU
        // APU
    }
};
void DMG_CPU::runTCycle() {
    // Optional later expansion
};

void DMG_CPU::runFrame() {
    m_cycle_count = 0;
    while (m_cycle_count < k_Cycle_Per_Frame) {
        runMCycle();
        m_cycle_count += 4;
    }
    // m_ppu.PresentScreen();
};

void DMG_CPU::callAbort() {
    m_abort = true;
};


uint8_t DMG_CPU::PC_Eat_Byte() {
#ifdef DEBUG_LOGGING
    bool old_log_flag = m_log_enable;
    m_log_enable = false;
#endif

#ifdef DEBUG_LOGGING
    uint16_t addr = m_regs.get(Reg_u16::PC);
    m_Memory.latchBus(addr);
#else
    m_Memory.latchBus(m_regs.get(Reg_u16::PC));
#endif
    uint8_t ret = m_Memory.Read(); // Read from <PC>
// #ifdef DEBUG_LOGGING
//     printf("PC Eat: %s, Value: %s\n", padLeft(to_string_base(m_regs.get(Reg_u16::PC), 16), 4, '0').c_str(), padLeft(to_string_base(ret, 16), 2, '0').c_str());
// #endif
    m_regs.set(Reg_u16::PC, m_regs.get(Reg_u16::PC) + 1); // Increment PC
    
#ifdef DEBUG_LOGGING
    m_log_enable = old_log_flag;
    if (m_log_enable) {
        m_logfile.print("PC Eat Byte addr "+to_b16_out(addr)+" value "+to_b8_out(ret)+", ");
    }
#endif
    return ret;
};
uint8_t DMG_CPU::ALU_B8_ADDER(uint8_t num1, uint8_t num2, uint8_t carry_bit) {
    uint8_t half = (num1 & 0xf) + (num2 & 0xf) + carry_bit; // For half carry
    uint16_t unmasked = static_cast<uint16_t>(num1) + static_cast<uint16_t>(num2) + static_cast<uint16_t>(carry_bit); // For carry
    uint8_t masked = static_cast<uint8_t>(unmasked & 0xff);
    m_regs.set(Reg_flag::Z, (masked == 0));
    m_regs.set(Reg_flag::N, false);
    m_regs.set(Reg_flag::H, (((half >> 4) & 0b1) == 1));
    m_regs.set(Reg_flag::C, (((unmasked >> 8) & 0b1) == 1));
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Performed ALU "+to_b8_out(num1)+" + "+to_b8_out(num2)
                +(carry_bit != 0 ? " + "+to_b8_out(carry_bit) : "")
                +" = "+to_b8_out(masked)+", ");
        }
    #endif
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
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Performed ALU "+to_b8_out(num1)+" - "+to_b8_out(num2)
                +(carry_bit != 0 ? " - "+to_b8_out(carry_bit) : "")
                +" = "+to_b8_out(masked)+", ");
        }
    #endif
    return masked;
};
void DMG_CPU::STOP() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Set STOP, ");
        }
    #endif
    m_stopped = true;
};
void DMG_CPU::HALT() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Set HALT, ");
        }
    #endif
    m_halted = true;
};
void DMG_CPU::HALT_Bug() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Triggered Halt Bug, ");
        }
    #endif
    m_halt_bug = true;
};
bool DMG_CPU::WAKE() {
    #ifdef DEBUG_LOGGING
        if (m_log_enable) {
            m_logfile.print("Check WAKE (joysticks), ");
        }
    #endif
    return false; // Returns true if any buttons are pressed
};
