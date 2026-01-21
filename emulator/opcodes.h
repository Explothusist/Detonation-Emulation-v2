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

// ----------------- Relative Jumps -----------------
extern Opcode Opcode_x18_JR;
extern Opcode Opcode_x20_JR_NZ;
extern Opcode Opcode_x28_JR_Z;
extern Opcode Opcode_x30_JR_NC;
extern Opcode Opcode_x38_JR_C;

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

extern Opcode Opcode_xZZ_UNIMPLEMENTED;

#endif