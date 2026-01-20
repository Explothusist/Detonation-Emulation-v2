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
extern Opcode Opcode_xZZ_UNIMPLEMENTED;

#endif