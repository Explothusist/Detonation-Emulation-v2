
#include "opcodes.h"
#include "memory.h"
#include "cpu.h"

void NOP_MCycle_1(DMG_CPU &m_cpu) {
    // Do Nothing
};
Opcode Opcode_x00_NOP = {
    { NOP_MCycle_1 },
    1
};

void UNIMPLEMENTED_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED = {
    { UNIMPLEMENTED_MCycle_1 },
    1
};
