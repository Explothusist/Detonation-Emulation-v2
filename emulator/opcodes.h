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

// ----------------- Relative Jumps -----------------
extern Opcode Opcode_x18_JR;
extern Opcode Opcode_x20_JR_NZ;
extern Opcode Opcode_x28_JR_Z;
extern Opcode Opcode_x30_JR_NC;
extern Opcode Opcode_x38_JR_C;

extern Opcode Opcode_x40_LD_B_B;
extern Opcode Opcode_x50_LD_D_B;
extern Opcode Opcode_x60_LD_H_B;
extern Opcode Opcode_x70_LD_HL_B;
extern Opcode Opcode_xZZ_UNIMPLEMENTED;

#endif