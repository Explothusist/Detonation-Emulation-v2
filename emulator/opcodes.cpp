
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

void HALT_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode HALT");
    m_cpu.callAbort();
};
Opcode Opcode_x76_HALT = {
    { HALT_MCycle_1 },
    1
};


// ----------------- Relative Jumps -----------------
void JR_MCycle_1(DMG_CPU &m_cpu) {
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
template <Reg_flag FLAG>
void JR_F_MCycle_1(DMG_CPU &m_cpu) {
    JR_MCycle_1(m_cpu);
    m_cpu.m_regs.latchFlags();
};
template <Reg_flag FLAG>
void JR_F_MCycle_2(DMG_CPU &m_cpu) {
    JR_MCycle_2(m_cpu);
    if (!m_cpu.m_regs.getLatched(FLAG)) {
        m_cpu.clearOpcode(); // End the opcode earlier
    }
};
template <Reg_flag FLAG>
void JR_F_MCycle_3(DMG_CPU &m_cpu) {
    JR_MCycle_3(m_cpu);
};

Opcode Opcode_x18_JR = {
    { JR_MCycle_1, JR_MCycle_2, JR_MCycle_3 },
    3
};

Opcode Opcode_x20_JR_NZ = {
    { JR_F_MCycle_1<Reg_flag::NZ>, JR_F_MCycle_2<Reg_flag::NZ>, JR_F_MCycle_3<Reg_flag::NZ> },
    3
};
Opcode Opcode_x28_JR_Z = {
    { JR_F_MCycle_1<Reg_flag::Z>, JR_F_MCycle_2<Reg_flag::Z>, JR_F_MCycle_3<Reg_flag::Z> },
    3
};
Opcode Opcode_x30_JR_NC = {
    { JR_F_MCycle_1<Reg_flag::NC>, JR_F_MCycle_2<Reg_flag::NC>, JR_F_MCycle_3<Reg_flag::NC> },
    3
};
Opcode Opcode_x38_JR_C = {
    { JR_F_MCycle_1<Reg_flag::C>, JR_F_MCycle_2<Reg_flag::C>, JR_F_MCycle_3<Reg_flag::C> },
    3
};


// ----------------- 8 bit Loads -----------------
template <Reg_u8 REG1, Reg_u8 REG2> // Loads REG2 into REG1 (REG1 = REG2)
void LD_N_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(REG1, m_cpu.m_regs.get(REG2));
};
template <Reg_u8 REG1>
void LD_N_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
template <Reg_u8 REG1>
void LD_N_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(REG1, m_cpu.m_Memory.Read(m_cpu.m_regs.get(Reg_u16::HL)));
};
template <Reg_u8 REG2>
void LD_HL_N_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to write
};
template <Reg_u8 REG2>
void LD_HL_N_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u16::HL), m_cpu.m_regs.get(REG2));
};

Opcode Opcode_x40_LD_B_B = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::B> }, 1 };
Opcode Opcode_x41_LD_B_C = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::C> }, 1 };
Opcode Opcode_x42_LD_B_D = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::D> }, 1 };
Opcode Opcode_x43_LD_B_E = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::E> }, 1 };
Opcode Opcode_x44_LD_B_H = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::H> }, 1 };
Opcode Opcode_x45_LD_B_L = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::L> }, 1 };
Opcode Opcode_x46_LD_B_HL = { { LD_N_HL_MCycle_1<Reg_u8::B>, LD_N_HL_MCycle_2<Reg_u8::B> }, 2 };
Opcode Opcode_x47_LD_B_A = { { LD_N_N_MCycle_1<Reg_u8::B, Reg_u8::A> }, 1 };

Opcode Opcode_x48_LD_C_B = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::B> }, 1 };
Opcode Opcode_x49_LD_C_C = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::C> }, 1 };
Opcode Opcode_x4A_LD_C_D = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::D> }, 1 };
Opcode Opcode_x4B_LD_C_E = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::E> }, 1 };
Opcode Opcode_x4C_LD_C_H = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::H> }, 1 };
Opcode Opcode_x4D_LD_C_L = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::L> }, 1 };
Opcode Opcode_x4E_LD_C_HL = { { LD_N_HL_MCycle_1<Reg_u8::C>, LD_N_HL_MCycle_2<Reg_u8::C> }, 2 };
Opcode Opcode_x4F_LD_C_A = { { LD_N_N_MCycle_1<Reg_u8::C, Reg_u8::A> }, 1 };

Opcode Opcode_x50_LD_D_B = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::B> }, 1 };
Opcode Opcode_x51_LD_D_C = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::C> }, 1 };
Opcode Opcode_x52_LD_D_D = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::D> }, 1 };
Opcode Opcode_x53_LD_D_E = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::E> }, 1 };
Opcode Opcode_x54_LD_D_H = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::H> }, 1 };
Opcode Opcode_x55_LD_D_L = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::L> }, 1 };
Opcode Opcode_x56_LD_D_HL = { { LD_N_HL_MCycle_1<Reg_u8::D>, LD_N_HL_MCycle_2<Reg_u8::D> }, 2 };
Opcode Opcode_x57_LD_D_A = { { LD_N_N_MCycle_1<Reg_u8::D, Reg_u8::A> }, 1 };

Opcode Opcode_x58_LD_E_B = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::B> }, 1 };
Opcode Opcode_x59_LD_E_C = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::C> }, 1 };
Opcode Opcode_x5A_LD_E_D = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::D> }, 1 };
Opcode Opcode_x5B_LD_E_E = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::E> }, 1 };
Opcode Opcode_x5C_LD_E_H = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::H> }, 1 };
Opcode Opcode_x5D_LD_E_L = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::L> }, 1 };
Opcode Opcode_x5E_LD_E_HL = { { LD_N_HL_MCycle_1<Reg_u8::E>, LD_N_HL_MCycle_2<Reg_u8::E> }, 2 };
Opcode Opcode_x5F_LD_E_A = { { LD_N_N_MCycle_1<Reg_u8::E, Reg_u8::A> }, 1 };

Opcode Opcode_x60_LD_H_B = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::B> }, 1 };
Opcode Opcode_x61_LD_H_C = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::C> }, 1 };
Opcode Opcode_x62_LD_H_D = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::D> }, 1 };
Opcode Opcode_x63_LD_H_E = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::E> }, 1 };
Opcode Opcode_x64_LD_H_H = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::H> }, 1 };
Opcode Opcode_x65_LD_H_L = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::L> }, 1 };
Opcode Opcode_x66_LD_H_HL = { { LD_N_HL_MCycle_1<Reg_u8::H>, LD_N_HL_MCycle_2<Reg_u8::H> }, 2 };
Opcode Opcode_x67_LD_H_A = { { LD_N_N_MCycle_1<Reg_u8::H, Reg_u8::A> }, 1 };

Opcode Opcode_x68_LD_L_B = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::B> }, 1 };
Opcode Opcode_x69_LD_L_C = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::C> }, 1 };
Opcode Opcode_x6A_LD_L_D = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::D> }, 1 };
Opcode Opcode_x6B_LD_L_E = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::E> }, 1 };
Opcode Opcode_x6C_LD_L_H = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::H> }, 1 };
Opcode Opcode_x6D_LD_L_L = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::L> }, 1 };
Opcode Opcode_x6E_LD_L_HL = { { LD_N_HL_MCycle_1<Reg_u8::L>, LD_N_HL_MCycle_2<Reg_u8::L> }, 2 };
Opcode Opcode_x6F_LD_L_A = { { LD_N_N_MCycle_1<Reg_u8::L, Reg_u8::A> }, 1 };

Opcode Opcode_x70_LD_HL_B = { { LD_HL_N_MCycle_1<Reg_u8::B>, LD_HL_N_MCycle_2<Reg_u8::B> }, 2 };
Opcode Opcode_x71_LD_HL_C = { { LD_HL_N_MCycle_1<Reg_u8::C>, LD_HL_N_MCycle_2<Reg_u8::C> }, 2 };
Opcode Opcode_x72_LD_HL_D = { { LD_HL_N_MCycle_1<Reg_u8::D>, LD_HL_N_MCycle_2<Reg_u8::D> }, 2 };
Opcode Opcode_x73_LD_HL_E = { { LD_HL_N_MCycle_1<Reg_u8::E>, LD_HL_N_MCycle_2<Reg_u8::E> }, 2 };
Opcode Opcode_x74_LD_HL_H = { { LD_HL_N_MCycle_1<Reg_u8::H>, LD_HL_N_MCycle_2<Reg_u8::H> }, 2 };
Opcode Opcode_x75_LD_HL_L = { { LD_HL_N_MCycle_1<Reg_u8::L>, LD_HL_N_MCycle_2<Reg_u8::L> }, 2 };
// (Halt)
Opcode Opcode_x77_LD_HL_A = { { LD_HL_N_MCycle_1<Reg_u8::A>, LD_HL_N_MCycle_2<Reg_u8::A> }, 2 };

Opcode Opcode_x78_LD_A_B = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::B> }, 1 };
Opcode Opcode_x79_LD_A_C = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::C> }, 1 };
Opcode Opcode_x7A_LD_A_D = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::D> }, 1 };
Opcode Opcode_x7B_LD_A_E = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::E> }, 1 };
Opcode Opcode_x7C_LD_A_H = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::H> }, 1 };
Opcode Opcode_x7D_LD_A_L = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::L> }, 1 };
Opcode Opcode_x7E_LD_A_HL = { { LD_N_HL_MCycle_1<Reg_u8::A>, LD_N_HL_MCycle_2<Reg_u8::A> }, 2 };
Opcode Opcode_x7F_LD_A_A = { { LD_N_N_MCycle_1<Reg_u8::A, Reg_u8::A> }, 1 };


// ----------------- Unimplemented -----------------
void UNIMPLEMENTED_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED = {
    { UNIMPLEMENTED_MCycle_1 },
    1
};
