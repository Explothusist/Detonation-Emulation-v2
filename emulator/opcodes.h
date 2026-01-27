#ifndef DMG_CPU_OPCODES_
#define DMG_CPU_OPCODES_

#include <cstdint>

class DMG_CPU;

using MCycleFn = void(*)(DMG_CPU&); // return type(function pointer)(param types)

struct Opcode {
    MCycleFn mcycles[6];
    uint8_t length;
    // uint8_t param1;
    // uint8_t param2;
};

// ----------------- Special -----------------
extern Opcode Opcode_x00_NOP;
extern Opcode Opcode_x10_STOP;
extern Opcode Opcode_x76_HALT;
extern Opcode Opcode_xF3_DI;
extern Opcode Opcode_xFB_EI;
extern Opcode Opcode_x27_DAA;
extern Opcode Opcode_x2F_CPL;
extern Opcode Opcode_x37_SCF;
extern Opcode Opcode_x3F_CCF;

// ----------------- Stack Special -----------------
extern Opcode Opcode_x08_LD_NN_SP;
extern Opcode Opcode_xE8_ADD_SP_N;
extern Opcode Opcode_xF8_LD_HL_SP_N;
extern Opcode Opcode_xF9_LD_SP_HL;

// ----------------- Push and Pop b16 -----------------
extern Opcode Opcode_xC1_PUSH_BC;
extern Opcode Opcode_xC5_POP_BC;
extern Opcode Opcode_xD1_PUSH_DE;
extern Opcode Opcode_xD5_POP_DE;
extern Opcode Opcode_xE1_PUSH_HL;
extern Opcode Opcode_xE5_POP_HL;
extern Opcode Opcode_xF1_PUSH_AF;
extern Opcode Opcode_xF5_POP_AF;

// ----------------- Relative Jumps -----------------
extern Opcode Opcode_x18_JR;
extern Opcode Opcode_x20_JR_NZ;
extern Opcode Opcode_x28_JR_Z;
extern Opcode Opcode_x30_JR_NC;
extern Opcode Opcode_x38_JR_C;

// ----------------- Absolute Jumps -----------------
extern Opcode Opcode_xC3_JP;
extern Opcode Opcode_xC2_JP_NZ;
extern Opcode Opcode_xCA_JP_Z;
extern Opcode Opcode_xD2_JP_NC;
extern Opcode Opcode_xDA_JP_C;
extern Opcode Opcode_xE9_JP_HL;

// ----------------- Calls -----------------
extern Opcode Opcode_xCD_CALL;
extern Opcode Opcode_xC4_CALL_NZ;
extern Opcode Opcode_xCC_CALL_Z;
extern Opcode Opcode_xD4_CALL_NC;
extern Opcode Opcode_xDC_CALL_C;

// ----------------- Resets -----------------
extern Opcode Opcode_xC7_RST_00;
extern Opcode Opcode_xCF_RST_08;
extern Opcode Opcode_xD7_RST_10;
extern Opcode Opcode_xDF_RST_18;
extern Opcode Opcode_xE7_RST_20;
extern Opcode Opcode_xEF_RST_28;
extern Opcode Opcode_xF7_RST_30;
extern Opcode Opcode_xFF_RST_38;

// ----------------- Returns -----------------
extern Opcode Opcode_xC9_RET;
extern Opcode Opcode_xC0_RET_NZ;
extern Opcode Opcode_xC8_RET_Z;
extern Opcode Opcode_xD0_RET_NC;
extern Opcode Opcode_xD8_RET_C;
extern Opcode Opcode_xD9_RETI;

// ----------------- 8 bit Loads -----------------
extern Opcode Opcode_x40_LD_B_B;
extern Opcode Opcode_x41_LD_B_C;
extern Opcode Opcode_x42_LD_B_D;
extern Opcode Opcode_x43_LD_B_E;
extern Opcode Opcode_x44_LD_B_H;
extern Opcode Opcode_x45_LD_B_L;
extern Opcode Opcode_x46_LD_B_HL;
extern Opcode Opcode_x47_LD_B_A;

extern Opcode Opcode_x48_LD_C_B;
extern Opcode Opcode_x49_LD_C_C;
extern Opcode Opcode_x4A_LD_C_D;
extern Opcode Opcode_x4B_LD_C_E;
extern Opcode Opcode_x4C_LD_C_H;
extern Opcode Opcode_x4D_LD_C_L;
extern Opcode Opcode_x4E_LD_C_HL;
extern Opcode Opcode_x4F_LD_C_A;

extern Opcode Opcode_x50_LD_D_B;
extern Opcode Opcode_x51_LD_D_C;
extern Opcode Opcode_x52_LD_D_D;
extern Opcode Opcode_x53_LD_D_E;
extern Opcode Opcode_x54_LD_D_H;
extern Opcode Opcode_x55_LD_D_L;
extern Opcode Opcode_x56_LD_D_HL;
extern Opcode Opcode_x57_LD_D_A;

extern Opcode Opcode_x58_LD_E_B;
extern Opcode Opcode_x59_LD_E_C;
extern Opcode Opcode_x5A_LD_E_D;
extern Opcode Opcode_x5B_LD_E_E;
extern Opcode Opcode_x5C_LD_E_H;
extern Opcode Opcode_x5D_LD_E_L;
extern Opcode Opcode_x5E_LD_E_HL;
extern Opcode Opcode_x5F_LD_E_A;

extern Opcode Opcode_x60_LD_H_B;
extern Opcode Opcode_x61_LD_H_C;
extern Opcode Opcode_x62_LD_H_D;
extern Opcode Opcode_x63_LD_H_E;
extern Opcode Opcode_x64_LD_H_H;
extern Opcode Opcode_x65_LD_H_L;
extern Opcode Opcode_x66_LD_H_HL;
extern Opcode Opcode_x67_LD_H_A;

extern Opcode Opcode_x68_LD_L_B;
extern Opcode Opcode_x69_LD_L_C;
extern Opcode Opcode_x6A_LD_L_D;
extern Opcode Opcode_x6B_LD_L_E;
extern Opcode Opcode_x6C_LD_L_H;
extern Opcode Opcode_x6D_LD_L_L;
extern Opcode Opcode_x6E_LD_L_HL;
extern Opcode Opcode_x6F_LD_L_A;

extern Opcode Opcode_x70_LD_HL_B;
extern Opcode Opcode_x71_LD_HL_C;
extern Opcode Opcode_x72_LD_HL_D;
extern Opcode Opcode_x73_LD_HL_E;
extern Opcode Opcode_x74_LD_HL_H;
extern Opcode Opcode_x75_LD_HL_L;
// (Halt)
extern Opcode Opcode_x77_LD_HL_A;

extern Opcode Opcode_x78_LD_A_B;
extern Opcode Opcode_x79_LD_A_C;
extern Opcode Opcode_x7A_LD_A_D;
extern Opcode Opcode_x7B_LD_A_E;
extern Opcode Opcode_x7C_LD_A_H;
extern Opcode Opcode_x7D_LD_A_L;
extern Opcode Opcode_x7E_LD_A_HL;
extern Opcode Opcode_x7F_LD_A_A;

// ----------------- Special 8 bit Loads -----------------
extern Opcode Opcode_x06_LD_B_N8;
extern Opcode Opcode_x0E_LD_C_N8;
extern Opcode Opcode_x16_LD_D_N8;
extern Opcode Opcode_x1E_LD_E_N8;
extern Opcode Opcode_x26_LD_H_N8;
extern Opcode Opcode_x2E_LD_L_N8;
extern Opcode Opcode_x36_LD_HL_N8;
extern Opcode Opcode_x3E_LD_A_N8;

extern Opcode Opcode_xE0_LDH_A8_A;
extern Opcode Opcode_xF0_LDH_A_A8;

extern Opcode Opcode_xE2_LDH_C_A;
extern Opcode Opcode_xF2_LDH_A_C;

extern Opcode Opcode_x02_LD_BC_A;
extern Opcode Opcode_x12_LD_DE_A;
extern Opcode Opcode_x22_LD_HLI_A;
extern Opcode Opcode_x32_LD_HLD_A;

extern Opcode Opcode_x0A_LD_BC_A;
extern Opcode Opcode_x1A_LD_DE_A;
extern Opcode Opcode_x2A_LD_HLI_A;
extern Opcode Opcode_x3A_LD_HLD_A;

extern Opcode Opcode_xEA_LD_N16_A;
extern Opcode Opcode_xFA_LD_A_N16;

// ----------------- 16 bit Loads -----------------
extern Opcode Opcode_x01_LD_BC_N16;
extern Opcode Opcode_x11_LD_DE_N16;
extern Opcode Opcode_x21_LD_HL_N16;
extern Opcode Opcode_x31_LD_SP_N16;

// ----------------- 8 bit Addition -----------------
extern Opcode Opcode_x80_ADD_A_B;
extern Opcode Opcode_x81_ADD_A_C;
extern Opcode Opcode_x82_ADD_A_D;
extern Opcode Opcode_x83_ADD_A_E;
extern Opcode Opcode_x84_ADD_A_H;
extern Opcode Opcode_x85_ADD_A_L;
extern Opcode Opcode_x86_ADD_A_HL;
extern Opcode Opcode_x87_ADD_A_A;
extern Opcode Opcode_xC6_ADD_A_N8;

extern Opcode Opcode_x88_ADC_A_B;
extern Opcode Opcode_x89_ADC_A_C;
extern Opcode Opcode_x8A_ADC_A_D;
extern Opcode Opcode_x8B_ADC_A_E;
extern Opcode Opcode_x8C_ADC_A_H;
extern Opcode Opcode_x8D_ADC_A_L;
extern Opcode Opcode_x8E_ADC_A_HL;
extern Opcode Opcode_x8F_ADC_A_A;
extern Opcode Opcode_xCE_ADC_A_N8;

extern Opcode Opcode_x04_INC_B;
extern Opcode Opcode_x0C_INC_C;
extern Opcode Opcode_x14_INC_D;
extern Opcode Opcode_x1C_INC_E;
extern Opcode Opcode_x24_INC_H;
extern Opcode Opcode_x2C_INC_L;
extern Opcode Opcode_x34_INC_HL;
extern Opcode Opcode_x3C_INC_A;

extern Opcode Opcode_x05_DEC_B;
extern Opcode Opcode_x0D_DEC_C;
extern Opcode Opcode_x15_DEC_D;
extern Opcode Opcode_x1D_DEC_E;
extern Opcode Opcode_x25_DEC_HL;
extern Opcode Opcode_x2D_DEC_L;
extern Opcode Opcode_x35_DEC_H;
extern Opcode Opcode_x3D_DEC_A;

// ----------------- 8 bit Subtraction -----------------
extern Opcode Opcode_x90_SUB_A_B;
extern Opcode Opcode_x91_SUB_A_C;
extern Opcode Opcode_x92_SUB_A_D;
extern Opcode Opcode_x93_SUB_A_E;
extern Opcode Opcode_x94_SUB_A_H;
extern Opcode Opcode_x95_SUB_A_L;
extern Opcode Opcode_x96_SUB_A_HL;
extern Opcode Opcode_x97_SUB_A_A;
extern Opcode Opcode_xD6_SUB_A_N8;

extern Opcode Opcode_x98_SBC_A_B;
extern Opcode Opcode_x99_SBC_A_C;
extern Opcode Opcode_x9A_SBC_A_D;
extern Opcode Opcode_x9B_SBC_A_E;
extern Opcode Opcode_x9C_SBC_A_H;
extern Opcode Opcode_x9D_SBC_A_L;
extern Opcode Opcode_x9E_SBC_A_HL;
extern Opcode Opcode_x9F_SBC_A_A;
extern Opcode Opcode_xDE_SBC_A_N8;

// ----------------- 8 bit Bit Operations -----------------
extern Opcode Opcode_xA0_AND_A_B;
extern Opcode Opcode_xA1_AND_A_C;
extern Opcode Opcode_xA2_AND_A_D;
extern Opcode Opcode_xA3_AND_A_E;
extern Opcode Opcode_xA4_AND_A_H;
extern Opcode Opcode_xA5_AND_A_L;
extern Opcode Opcode_xA6_AND_A_HL;
extern Opcode Opcode_xA7_AND_A_A;
extern Opcode Opcode_xE6_AND_A_N8;

extern Opcode Opcode_xA8_XOR_A_B;
extern Opcode Opcode_xA9_XOR_A_C;
extern Opcode Opcode_xAA_XOR_A_D;
extern Opcode Opcode_xAB_XOR_A_E;
extern Opcode Opcode_xAC_XOR_A_H;
extern Opcode Opcode_xAD_XOR_A_L;
extern Opcode Opcode_xAE_XOR_A_HL;
extern Opcode Opcode_xAF_XOR_A_A;
extern Opcode Opcode_xEE_XOR_A_N8;

extern Opcode Opcode_xB0_OR_A_B;
extern Opcode Opcode_xB1_OR_A_C;
extern Opcode Opcode_xB2_OR_A_D;
extern Opcode Opcode_xB3_OR_A_E;
extern Opcode Opcode_xB4_OR_A_H;
extern Opcode Opcode_xB5_OR_A_L;
extern Opcode Opcode_xB6_OR_A_HL;
extern Opcode Opcode_xB7_OR_A_A;
extern Opcode Opcode_xF6_OR_A_N8;

extern Opcode Opcode_xB8_CP_A_B;
extern Opcode Opcode_xB9_CP_A_C;
extern Opcode Opcode_xBA_CP_A_D;
extern Opcode Opcode_xBB_CP_A_E;
extern Opcode Opcode_xBC_CP_A_H;
extern Opcode Opcode_xBD_CP_A_L;
extern Opcode Opcode_xBE_CP_A_HL;
extern Opcode Opcode_xBF_CP_A_A;
extern Opcode Opcode_xFE_CP_A_N8;

extern Opcode Opcode_x07_RLCA;
extern Opcode Opcode_x0F_RRCA;
extern Opcode Opcode_x17_RLA;
extern Opcode Opcode_x1F_RRA;

// ----------------- 16 bit Addition -----------------
extern Opcode Opcode_x03_INC_BC;
extern Opcode Opcode_x13_INC_DE;
extern Opcode Opcode_x23_INC_HL;
extern Opcode Opcode_x33_INC_SP;

extern Opcode Opcode_x0B_DEC_BC;
extern Opcode Opcode_x1B_DEC_DE;
extern Opcode Opcode_x2B_DEC_HL;
extern Opcode Opcode_x3B_DEC_SP;

extern Opcode Opcode_x09_ADD_HL_BC;
extern Opcode Opcode_x19_ADD_HL_DE;
extern Opcode Opcode_x29_ADD_HL_HL;
extern Opcode Opcode_x39_ADD_HL_SP;

// ----------------- Invalid -----------------
extern Opcode Opcode_xD3_INVALID;
extern Opcode Opcode_xDB_INVALID;
extern Opcode Opcode_xDD_INVALID;
extern Opcode Opcode_xE3_INVALID;
extern Opcode Opcode_xE4_INVALID;
extern Opcode Opcode_xEB_INVALID;
extern Opcode Opcode_xEC_INVALID;
extern Opcode Opcode_xED_INVALID;
extern Opcode Opcode_xF4_INVALID;
extern Opcode Opcode_xFC_INVALID;
extern Opcode Opcode_xFD_INVALID;

// ----------------- CB Opcodes -----------------
extern Opcode Opcode_CB_x00_RLC_B;
extern Opcode Opcode_CB_x01_RLC_C;
extern Opcode Opcode_CB_x02_RLC_D;
extern Opcode Opcode_CB_x03_RLC_E;
extern Opcode Opcode_CB_x04_RLC_H;
extern Opcode Opcode_CB_x05_RLC_L;
extern Opcode Opcode_CB_x06_RLC_HL;
extern Opcode Opcode_CB_x07_RLC_A;

extern Opcode Opcode_CB_x08_RRC_B;
extern Opcode Opcode_CB_x09_RRC_C;
extern Opcode Opcode_CB_x0A_RRC_D;
extern Opcode Opcode_CB_x0B_RRC_E;
extern Opcode Opcode_CB_x0C_RRC_H;
extern Opcode Opcode_CB_x0D_RRC_L;
extern Opcode Opcode_CB_x0E_RRC_HL;
extern Opcode Opcode_CB_x0F_RRC_A;

extern Opcode Opcode_CB_x10_RL_B;
extern Opcode Opcode_CB_x11_RL_C;
extern Opcode Opcode_CB_x12_RL_D;
extern Opcode Opcode_CB_x13_RL_E;
extern Opcode Opcode_CB_x14_RL_H;
extern Opcode Opcode_CB_x15_RL_L;
extern Opcode Opcode_CB_x16_RL_HL;
extern Opcode Opcode_CB_x17_RL_A;

extern Opcode Opcode_CB_x18_RR_B;
extern Opcode Opcode_CB_x19_RR_C;
extern Opcode Opcode_CB_x1A_RR_D;
extern Opcode Opcode_CB_x1B_RR_E;
extern Opcode Opcode_CB_x1C_RR_H;
extern Opcode Opcode_CB_x1D_RR_L;
extern Opcode Opcode_CB_x1E_RR_HL;
extern Opcode Opcode_CB_x1F_RR_A;

extern Opcode Opcode_CB_x20_SLA_B;
extern Opcode Opcode_CB_x21_SLA_C;
extern Opcode Opcode_CB_x22_SLA_D;
extern Opcode Opcode_CB_x23_SLA_E;
extern Opcode Opcode_CB_x24_SLA_H;
extern Opcode Opcode_CB_x25_SLA_L;
extern Opcode Opcode_CB_x26_SLA_HL;
extern Opcode Opcode_CB_x27_SLA_A;

extern Opcode Opcode_CB_x28_SRA_B;
extern Opcode Opcode_CB_x29_SRA_C;
extern Opcode Opcode_CB_x2A_SRA_D;
extern Opcode Opcode_CB_x2B_SRA_E;
extern Opcode Opcode_CB_x2C_SRA_H;
extern Opcode Opcode_CB_x2D_SRA_L;
extern Opcode Opcode_CB_x2E_SRA_HL;
extern Opcode Opcode_CB_x2F_SRA_A;

extern Opcode Opcode_CB_x30_SWAP_B;
extern Opcode Opcode_CB_x31_SWAP_C;
extern Opcode Opcode_CB_x32_SWAP_D;
extern Opcode Opcode_CB_x33_SWAP_E;
extern Opcode Opcode_CB_x34_SWAP_H;
extern Opcode Opcode_CB_x35_SWAP_L;
extern Opcode Opcode_CB_x36_SWAP_HL;
extern Opcode Opcode_CB_x37_SWAP_A;

extern Opcode Opcode_CB_x38_SRL_B;
extern Opcode Opcode_CB_x39_SRL_C;
extern Opcode Opcode_CB_x3A_SRL_D;
extern Opcode Opcode_CB_x3B_SRL_E;
extern Opcode Opcode_CB_x3C_SRL_H;
extern Opcode Opcode_CB_x3D_SRL_L;
extern Opcode Opcode_CB_x3E_SRL_HL;
extern Opcode Opcode_CB_x3F_SRL_A;

extern Opcode Opcode_CB_x40_BIT_0_B;
extern Opcode Opcode_CB_x41_BIT_0_C;
extern Opcode Opcode_CB_x42_BIT_0_D;
extern Opcode Opcode_CB_x43_BIT_0_E;
extern Opcode Opcode_CB_x44_BIT_0_H;
extern Opcode Opcode_CB_x45_BIT_0_L;
extern Opcode Opcode_CB_x46_BIT_0_HL;
extern Opcode Opcode_CB_x47_BIT_0_A;

extern Opcode Opcode_CB_x48_BIT_1_B;
extern Opcode Opcode_CB_x49_BIT_1_C;
extern Opcode Opcode_CB_x4A_BIT_1_D;
extern Opcode Opcode_CB_x4B_BIT_1_E;
extern Opcode Opcode_CB_x4C_BIT_1_H;
extern Opcode Opcode_CB_x4D_BIT_1_L;
extern Opcode Opcode_CB_x4E_BIT_1_HL;
extern Opcode Opcode_CB_x4F_BIT_1_A;

extern Opcode Opcode_CB_x50_BIT_2_B;
extern Opcode Opcode_CB_x51_BIT_2_C;
extern Opcode Opcode_CB_x52_BIT_2_D;
extern Opcode Opcode_CB_x53_BIT_2_E;
extern Opcode Opcode_CB_x54_BIT_2_H;
extern Opcode Opcode_CB_x55_BIT_2_L;
extern Opcode Opcode_CB_x56_BIT_2_HL;
extern Opcode Opcode_CB_x57_BIT_2_A;

extern Opcode Opcode_CB_x58_BIT_3_B;
extern Opcode Opcode_CB_x59_BIT_3_C;
extern Opcode Opcode_CB_x5A_BIT_3_D;
extern Opcode Opcode_CB_x5B_BIT_3_E;
extern Opcode Opcode_CB_x5C_BIT_3_H;
extern Opcode Opcode_CB_x5D_BIT_3_L;
extern Opcode Opcode_CB_x5E_BIT_3_HL;
extern Opcode Opcode_CB_x5F_BIT_3_A;

extern Opcode Opcode_CB_x60_BIT_4_B;
extern Opcode Opcode_CB_x61_BIT_4_C;
extern Opcode Opcode_CB_x62_BIT_4_D;
extern Opcode Opcode_CB_x63_BIT_4_E;
extern Opcode Opcode_CB_x64_BIT_4_H;
extern Opcode Opcode_CB_x65_BIT_4_L;
extern Opcode Opcode_CB_x66_BIT_4_HL;
extern Opcode Opcode_CB_x67_BIT_4_A;

extern Opcode Opcode_CB_x68_BIT_5_B;
extern Opcode Opcode_CB_x69_BIT_5_C;
extern Opcode Opcode_CB_x6A_BIT_5_D;
extern Opcode Opcode_CB_x6B_BIT_5_E;
extern Opcode Opcode_CB_x6C_BIT_5_H;
extern Opcode Opcode_CB_x6D_BIT_5_L;
extern Opcode Opcode_CB_x6E_BIT_5_HL;
extern Opcode Opcode_CB_x6F_BIT_5_A;

extern Opcode Opcode_CB_x70_BIT_6_B;
extern Opcode Opcode_CB_x71_BIT_6_C;
extern Opcode Opcode_CB_x72_BIT_6_D;
extern Opcode Opcode_CB_x73_BIT_6_E;
extern Opcode Opcode_CB_x74_BIT_6_H;
extern Opcode Opcode_CB_x75_BIT_6_L;
extern Opcode Opcode_CB_x76_BIT_6_HL;
extern Opcode Opcode_CB_x77_BIT_6_A;

extern Opcode Opcode_CB_x78_BIT_7_B;
extern Opcode Opcode_CB_x79_BIT_7_C;
extern Opcode Opcode_CB_x7A_BIT_7_D;
extern Opcode Opcode_CB_x7B_BIT_7_E;
extern Opcode Opcode_CB_x7C_BIT_7_H;
extern Opcode Opcode_CB_x7D_BIT_7_L;
extern Opcode Opcode_CB_x7E_BIT_7_HL;
extern Opcode Opcode_CB_x7F_BIT_7_A;

extern Opcode Opcode_CB_x80_RES_0_B;
extern Opcode Opcode_CB_x81_RES_0_C;
extern Opcode Opcode_CB_x82_RES_0_D;
extern Opcode Opcode_CB_x83_RES_0_E;
extern Opcode Opcode_CB_x84_RES_0_H;
extern Opcode Opcode_CB_x85_RES_0_L;
extern Opcode Opcode_CB_x86_RES_0_HL;
extern Opcode Opcode_CB_x87_RES_0_A;

extern Opcode Opcode_CB_x88_RES_1_B;
extern Opcode Opcode_CB_x89_RES_1_C;
extern Opcode Opcode_CB_x8A_RES_1_D;
extern Opcode Opcode_CB_x8B_RES_1_E;
extern Opcode Opcode_CB_x8C_RES_1_H;
extern Opcode Opcode_CB_x8D_RES_1_L;
extern Opcode Opcode_CB_x8E_RES_1_HL;
extern Opcode Opcode_CB_x8F_RES_1_A;

extern Opcode Opcode_CB_x90_RES_2_B;
extern Opcode Opcode_CB_x91_RES_2_C;
extern Opcode Opcode_CB_x92_RES_2_D;
extern Opcode Opcode_CB_x93_RES_2_E;
extern Opcode Opcode_CB_x94_RES_2_H;
extern Opcode Opcode_CB_x95_RES_2_L;
extern Opcode Opcode_CB_x96_RES_2_HL;
extern Opcode Opcode_CB_x97_RES_2_A;

extern Opcode Opcode_CB_x98_RES_3_B;
extern Opcode Opcode_CB_x99_RES_3_C;
extern Opcode Opcode_CB_x9A_RES_3_D;
extern Opcode Opcode_CB_x9B_RES_3_E;
extern Opcode Opcode_CB_x9C_RES_3_H;
extern Opcode Opcode_CB_x9D_RES_3_L;
extern Opcode Opcode_CB_x9E_RES_3_HL;
extern Opcode Opcode_CB_x9F_RES_3_A;

extern Opcode Opcode_CB_xA0_RES_4_B;
extern Opcode Opcode_CB_xA1_RES_4_C;
extern Opcode Opcode_CB_xA2_RES_4_D;
extern Opcode Opcode_CB_xA3_RES_4_E;
extern Opcode Opcode_CB_xA4_RES_4_H;
extern Opcode Opcode_CB_xA5_RES_4_L;
extern Opcode Opcode_CB_xA6_RES_4_HL;
extern Opcode Opcode_CB_xA7_RES_4_A;

extern Opcode Opcode_CB_xA8_RES_5_B;
extern Opcode Opcode_CB_xA9_RES_5_C;
extern Opcode Opcode_CB_xAA_RES_5_D;
extern Opcode Opcode_CB_xAB_RES_5_E;
extern Opcode Opcode_CB_xAC_RES_5_H;
extern Opcode Opcode_CB_xAD_RES_5_L;
extern Opcode Opcode_CB_xAE_RES_5_HL;
extern Opcode Opcode_CB_xAF_RES_5_A;

extern Opcode Opcode_CB_xB0_RES_6_B;
extern Opcode Opcode_CB_xB1_RES_6_C;
extern Opcode Opcode_CB_xB2_RES_6_D;
extern Opcode Opcode_CB_xB3_RES_6_E;
extern Opcode Opcode_CB_xB4_RES_6_H;
extern Opcode Opcode_CB_xB5_RES_6_L;
extern Opcode Opcode_CB_xB6_RES_6_HL;
extern Opcode Opcode_CB_xB7_RES_6_A;

extern Opcode Opcode_CB_xB8_RES_7_B;
extern Opcode Opcode_CB_xB9_RES_7_C;
extern Opcode Opcode_CB_xBA_RES_7_D;
extern Opcode Opcode_CB_xBB_RES_7_E;
extern Opcode Opcode_CB_xBC_RES_7_H;
extern Opcode Opcode_CB_xBD_RES_7_L;
extern Opcode Opcode_CB_xBE_RES_7_HL;
extern Opcode Opcode_CB_xBF_RES_7_A;

extern Opcode Opcode_CB_xC0_SET_0_B;
extern Opcode Opcode_CB_xC1_SET_0_C;
extern Opcode Opcode_CB_xC2_SET_0_D;
extern Opcode Opcode_CB_xC3_SET_0_E;
extern Opcode Opcode_CB_xC4_SET_0_H;
extern Opcode Opcode_CB_xC5_SET_0_L;
extern Opcode Opcode_CB_xC6_SET_0_HL;
extern Opcode Opcode_CB_xC7_SET_0_A;

extern Opcode Opcode_CB_xC8_SET_1_B;
extern Opcode Opcode_CB_xC9_SET_1_C;
extern Opcode Opcode_CB_xCA_SET_1_D;
extern Opcode Opcode_CB_xCB_SET_1_E;
extern Opcode Opcode_CB_xCC_SET_1_H;
extern Opcode Opcode_CB_xCD_SET_1_L;
extern Opcode Opcode_CB_xCE_SET_1_HL;
extern Opcode Opcode_CB_xCF_SET_1_A;

extern Opcode Opcode_CB_xD0_SET_2_B;
extern Opcode Opcode_CB_xD1_SET_2_C;
extern Opcode Opcode_CB_xD2_SET_2_D;
extern Opcode Opcode_CB_xD3_SET_2_E;
extern Opcode Opcode_CB_xD4_SET_2_H;
extern Opcode Opcode_CB_xD5_SET_2_L;
extern Opcode Opcode_CB_xD6_SET_2_HL;
extern Opcode Opcode_CB_xD7_SET_2_A;

extern Opcode Opcode_CB_xD8_SET_3_B;
extern Opcode Opcode_CB_xD9_SET_3_C;
extern Opcode Opcode_CB_xDA_SET_3_D;
extern Opcode Opcode_CB_xDB_SET_3_E;
extern Opcode Opcode_CB_xDC_SET_3_H;
extern Opcode Opcode_CB_xDD_SET_3_L;
extern Opcode Opcode_CB_xDE_SET_3_HL;
extern Opcode Opcode_CB_xDF_SET_3_A;

extern Opcode Opcode_CB_xE0_SET_4_B;
extern Opcode Opcode_CB_xE1_SET_4_C;
extern Opcode Opcode_CB_xE2_SET_4_D;
extern Opcode Opcode_CB_xE3_SET_4_E;
extern Opcode Opcode_CB_xE4_SET_4_H;
extern Opcode Opcode_CB_xE5_SET_4_L;
extern Opcode Opcode_CB_xE6_SET_4_HL;
extern Opcode Opcode_CB_xE7_SET_4_A;

extern Opcode Opcode_CB_xE8_SET_5_B;
extern Opcode Opcode_CB_xE9_SET_5_C;
extern Opcode Opcode_CB_xEA_SET_5_D;
extern Opcode Opcode_CB_xEB_SET_5_E;
extern Opcode Opcode_CB_xEC_SET_5_H;
extern Opcode Opcode_CB_xED_SET_5_L;
extern Opcode Opcode_CB_xEE_SET_5_HL;
extern Opcode Opcode_CB_xEF_SET_5_A;

extern Opcode Opcode_CB_xF0_SET_6_B;
extern Opcode Opcode_CB_xF1_SET_6_C;
extern Opcode Opcode_CB_xF2_SET_6_D;
extern Opcode Opcode_CB_xF3_SET_6_E;
extern Opcode Opcode_CB_xF4_SET_6_H;
extern Opcode Opcode_CB_xF5_SET_6_L;
extern Opcode Opcode_CB_xF6_SET_6_HL;
extern Opcode Opcode_CB_xF7_SET_6_A;

extern Opcode Opcode_CB_xF8_SET_7_B;
extern Opcode Opcode_CB_xF9_SET_7_C;
extern Opcode Opcode_CB_xFA_SET_7_D;
extern Opcode Opcode_CB_xFB_SET_7_E;
extern Opcode Opcode_CB_xFC_SET_7_H;
extern Opcode Opcode_CB_xFD_SET_7_L;
extern Opcode Opcode_CB_xFE_SET_7_HL;
extern Opcode Opcode_CB_xFF_SET_7_A;

// ----------------- Unimplemented -----------------
extern Opcode Opcode_xZZ_UNIMPLEMENTED;
extern Opcode Opcode_xZZ_UNIMPLEMENTED_CB;

#endif