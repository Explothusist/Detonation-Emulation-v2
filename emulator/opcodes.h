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

extern Opcode Opcode_x88_ADC_A_B;
extern Opcode Opcode_x89_ADC_A_C;
extern Opcode Opcode_x8A_ADC_A_D;
extern Opcode Opcode_x8B_ADC_A_E;
extern Opcode Opcode_x8C_ADC_A_H;
extern Opcode Opcode_x8D_ADC_A_L;
extern Opcode Opcode_x8E_ADC_A_HL;
extern Opcode Opcode_x8F_ADC_A_A;

// ----------------- 8 bit Subtraction -----------------
extern Opcode Opcode_x90_SUB_A_B;
extern Opcode Opcode_x91_SUB_A_C;
extern Opcode Opcode_x92_SUB_A_D;
extern Opcode Opcode_x93_SUB_A_E;
extern Opcode Opcode_x94_SUB_A_H;
extern Opcode Opcode_x95_SUB_A_L;
extern Opcode Opcode_x96_SUB_A_HL;
extern Opcode Opcode_x97_SUB_A_A;

extern Opcode Opcode_x98_SBC_A_B;
extern Opcode Opcode_x99_SBC_A_C;
extern Opcode Opcode_x9A_SBC_A_D;
extern Opcode Opcode_x9B_SBC_A_E;
extern Opcode Opcode_x9C_SBC_A_H;
extern Opcode Opcode_x9D_SBC_A_L;
extern Opcode Opcode_x9E_SBC_A_HL;
extern Opcode Opcode_x9F_SBC_A_A;

// ----------------- 8 bit Bit Operations -----------------
extern Opcode Opcode_xA0_AND_A_B;
extern Opcode Opcode_xA1_AND_A_C;
extern Opcode Opcode_xA2_AND_A_D;
extern Opcode Opcode_xA3_AND_A_E;
extern Opcode Opcode_xA4_AND_A_H;
extern Opcode Opcode_xA5_AND_A_L;
extern Opcode Opcode_xA6_AND_A_HL;
extern Opcode Opcode_xA7_AND_A_A;

extern Opcode Opcode_xA8_XOR_A_B;
extern Opcode Opcode_xA9_XOR_A_C;
extern Opcode Opcode_xAA_XOR_A_D;
extern Opcode Opcode_xAB_XOR_A_E;
extern Opcode Opcode_xAC_XOR_A_H;
extern Opcode Opcode_xAD_XOR_A_L;
extern Opcode Opcode_xAE_XOR_A_HL;
extern Opcode Opcode_xAF_XOR_A_A;

extern Opcode Opcode_xB0_OR_A_B;
extern Opcode Opcode_xB1_OR_A_C;
extern Opcode Opcode_xB2_OR_A_D;
extern Opcode Opcode_xB3_OR_A_E;
extern Opcode Opcode_xB4_OR_A_H;
extern Opcode Opcode_xB5_OR_A_L;
extern Opcode Opcode_xB6_OR_A_HL;
extern Opcode Opcode_xB7_OR_A_A;

extern Opcode Opcode_xB8_CP_A_B;
extern Opcode Opcode_xB9_CP_A_C;
extern Opcode Opcode_xBA_CP_A_D;
extern Opcode Opcode_xBB_CP_A_E;
extern Opcode Opcode_xBC_CP_A_H;
extern Opcode Opcode_xBD_CP_A_L;
extern Opcode Opcode_xBE_CP_A_HL;
extern Opcode Opcode_xBF_CP_A_A;


extern Opcode Opcode_xZZ_UNIMPLEMENTED;

#endif