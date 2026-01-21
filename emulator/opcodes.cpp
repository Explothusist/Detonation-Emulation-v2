
#include "opcodes.h"
#include "memory.h"
#include "cpu.h"

// ----------------- Special -----------------
void NOP_MCycle_1(DMG_CPU &m_cpu) {
    // Do Nothing
};
Opcode Opcode_x00_NOP = {
    { NOP_MCycle_1 },
    1
};

void STOP_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode STOP");
    m_cpu.callAbort();
};
Opcode Opcode_x10_STOP = {
    { STOP_MCycle_1 },
    1
};


// ----------------- Relative Jumps -----------------
void JR_MCycle_1(DMG_CPU &m_cpu) { // WORKING HERE
    // Lower byte of PC ends up in WZ after bus shenanigans
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.m_regs.get(Reg_u8::PC_L)); // Incidental, not functional
};
void JR_MCycle_2(DMG_CPU &m_cpu) {
    // Add PC to WZ
    int8_t offset = static_cast<int8_t>(m_cpu.m_Memory.PC_Eat_Byte(m_cpu.m_regs));
    m_cpu.m_regs.set(Reg_u16::WZ, m_cpu.m_regs.get(Reg_u16::PC) + offset);
};
void JR_MCycle_3(DMG_CPU &m_cpu) {
    // Jump by Offset (temp)
    m_cpu.m_regs.set(Reg_u16::PC, m_cpu.m_regs.get(Reg_u16::WZ));
};
void JR_F_MCycle_1(DMG_CPU &m_cpu, Reg_flag flag) {
    JR_MCycle_1(m_cpu);
    m_cpu.m_regs.latchFlags();
};
void JR_F_MCycle_2(DMG_CPU &m_cpu, Reg_flag flag) {
    JR_MCycle_2(m_cpu);
    if (!m_cpu.m_regs.getLatched(flag)) {
        m_cpu.clearOpcode(); // End the opcode earlier
    }
};
void JR_F_MCycle_3(DMG_CPU &m_cpu, Reg_flag flag) {
    JR_MCycle_3(m_cpu);
};

void JR_NZ_MCycle_1(DMG_CPU &m_cpu) {
    JR_F_MCycle_1(m_cpu, Reg_flag::NZ);
};
void JR_NZ_MCycle_2(DMG_CPU &m_cpu) {
    JR_F_MCycle_2(m_cpu, Reg_flag::NZ);
};
void JR_NZ_MCycle_3(DMG_CPU &m_cpu) {
    JR_F_MCycle_3(m_cpu, Reg_flag::NZ);
};
Opcode Opcode_x20_JR_NZ = {
    { JR_NZ_MCycle_1, JR_NZ_MCycle_2, JR_NZ_MCycle_3 },
    3
};

void JR_NC_MCycle_1(DMG_CPU &m_cpu) {
    JR_F_MCycle_1(m_cpu, Reg_flag::NC);
};
void JR_NC_MCycle_2(DMG_CPU &m_cpu) {
    JR_F_MCycle_2(m_cpu, Reg_flag::NC);
};
void JR_NC_MCycle_3(DMG_CPU &m_cpu) {
    JR_F_MCycle_3(m_cpu, Reg_flag::NC);
};
Opcode Opcode_x30_JR_NC = {
    { JR_NC_MCycle_1, JR_NC_MCycle_2, JR_NC_MCycle_3 },
    3
};

Opcode Opcode_x18_JR = {
    { JR_MCycle_1, JR_MCycle_2, JR_MCycle_3 },
    3
};

void JR_Z_MCycle_1(DMG_CPU &m_cpu) {
    JR_F_MCycle_1(m_cpu, Reg_flag::Z);
};
void JR_Z_MCycle_2(DMG_CPU &m_cpu) {
    JR_F_MCycle_2(m_cpu, Reg_flag::Z);
};
void JR_Z_MCycle_3(DMG_CPU &m_cpu) {
    JR_F_MCycle_3(m_cpu, Reg_flag::Z);
};
Opcode Opcode_x28_JR_Z = {
    { JR_Z_MCycle_1, JR_Z_MCycle_2, JR_Z_MCycle_3 },
    3
};

void JR_C_MCycle_1(DMG_CPU &m_cpu) {
    JR_F_MCycle_1(m_cpu, Reg_flag::C);
};
void JR_C_MCycle_2(DMG_CPU &m_cpu) {
    JR_F_MCycle_2(m_cpu, Reg_flag::C);
};
void JR_C_MCycle_3(DMG_CPU &m_cpu) {
    JR_F_MCycle_3(m_cpu, Reg_flag::C);
};
Opcode Opcode_x38_JR_C = {
    { JR_C_MCycle_1, JR_C_MCycle_2, JR_C_MCycle_3 },
    3
};


void UNIMPLEMENTED_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED = {
    { UNIMPLEMENTED_MCycle_1 },
    1
};
