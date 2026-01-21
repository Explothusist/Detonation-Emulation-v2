#ifndef DMG_CPU_OPCODES_
#define DMG_CPU_OPCODES_

#include <cstdint>

class DMG_CPU;

using MCycleFn = void(*)(DMG_CPU&); // return()(params)

struct Opcode {
    MCycleFn mcycles[6];
    uint8_t length;
};

extern Opcode Opcode_x00_NOP;
extern Opcode Opcode_x01_STOP;
extern Opcode Opcode_x02_JR_NZ;
extern Opcode Opcode_x03_JR_NC;
extern Opcode Opcode_x04_LD_B_B;
extern Opcode Opcode_x05_LD_D_B;
extern Opcode Opcode_x06_LD_H_B;
extern Opcode Opcode_x07_LD_HL_B;
extern Opcode Opcode_xZZ_UNIMPLEMENTED;

#endif