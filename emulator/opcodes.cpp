
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


// ----------------- 8 bit Addition -----------------
uint8_t ALU_B8_ADDER(DMG_CPU &m_cpu, uint8_t num1, uint8_t num2, uint8_t carry_bit = 0) {
    uint8_t half = (num1 & 0xf) + (num2 & 0xf) + carry_bit; // For half carry
    uint16_t unmasked = static_cast<uint16_t>(num1) + static_cast<uint16_t>(num2) + static_cast<uint16_t>(carry_bit); // For carry
    uint8_t masked = static_cast<uint8_t>(unmasked & 0xff);
    m_cpu.m_regs.set(Reg_flag::Z, (masked == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, ((half >> 4) & 0b1 == 1));
    m_cpu.m_regs.set(Reg_flag::C, ((unmasked >> 8) & 0b1 == 1));
    return masked;
};

template<Reg_u8 REG>
void ADD_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_ADDER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG)));
};
void ADD_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read(m_cpu.m_regs.get(Reg_u16::HL)));
};
void ADD_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_ADDER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(Reg_u8::temp_L)));
};

template<Reg_u8 REG>
void ADC_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_ADDER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};
void ADC_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read(m_cpu.m_regs.get(Reg_u16::HL)));
};
void ADC_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_ADDER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(Reg_u8::temp_L), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};

Opcode Opcode_x80_ADD_A_B = { { ADD_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x81_ADD_A_C = { { ADD_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x82_ADD_A_D = { { ADD_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x83_ADD_A_E = { { ADD_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x84_ADD_A_H = { { ADD_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x85_ADD_A_L = { { ADD_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x86_ADD_A_HL = { { ADD_A_HL_MCycle_1, ADD_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x87_ADD_A_A = { { ADD_A_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_x88_ADC_A_B = { { ADC_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x89_ADC_A_C = { { ADC_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x8A_ADC_A_D = { { ADC_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x8B_ADC_A_E = { { ADC_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x8C_ADC_A_H = { { ADC_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x8D_ADC_A_L = { { ADC_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x8E_ADC_A_HL = { { ADC_A_HL_MCycle_1, ADC_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x8F_ADC_A_A = { { ADC_A_N_MCycle_1<Reg_u8::A> }, 1 };


// ----------------- 8 bit Subtraction -----------------
uint8_t ALU_B8_SUBBER(DMG_CPU &m_cpu, uint8_t num1, uint8_t num2, uint8_t carry_bit = 0) {
    // uint8_t half = (num1 & 0xf) - (num2 & 0xf) - carry_bit; // For half carry
    uint16_t unmasked = static_cast<uint16_t>(num1) - static_cast<uint16_t>(num2) - static_cast<uint16_t>(carry_bit); // For carry
    uint8_t masked = static_cast<uint8_t>(unmasked & 0xff);
    m_cpu.m_regs.set(Reg_flag::Z, (masked == 0));
    m_cpu.m_regs.set(Reg_flag::N, true);
    m_cpu.m_regs.set(Reg_flag::H, (num1 & 0xf) < ((num2 & 0xf) + carry_bit));
    m_cpu.m_regs.set(Reg_flag::C, num1 < (num2 + carry_bit));
    return masked;
};

template<Reg_u8 REG>
void SUB_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_SUBBER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG)));
};
void SUB_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read(m_cpu.m_regs.get(Reg_u16::HL)));
};
void SUB_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_SUBBER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(Reg_u8::temp_L)));
};

template<Reg_u8 REG>
void SBC_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_SUBBER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};
void SBC_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read(m_cpu.m_regs.get(Reg_u16::HL)));
};
void SBC_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ALU_B8_SUBBER(m_cpu, m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(Reg_u8::temp_L), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};

Opcode Opcode_x90_SUB_A_B = { { SUB_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x91_SUB_A_C = { { SUB_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x92_SUB_A_D = { { SUB_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x93_SUB_A_E = { { SUB_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x94_SUB_A_H = { { SUB_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x95_SUB_A_L = { { SUB_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x96_SUB_A_HL = { { SUB_A_HL_MCycle_1, SUB_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x97_SUB_A_A = { { SUB_A_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_x98_SBC_A_B = { { SBC_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x99_SBC_A_C = { { SBC_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x9A_SBC_A_D = { { SBC_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x9B_SBC_A_E = { { SBC_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x9C_SBC_A_H = { { SBC_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x9D_SBC_A_L = { { SBC_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x9E_SBC_A_HL = { { SBC_A_HL_MCycle_1, SBC_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x9F_SBC_A_A = { { SBC_A_N_MCycle_1<Reg_u8::A> }, 1 };


// ----------------- Unimplemented -----------------
void UNIMPLEMENTED_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED = {
    { UNIMPLEMENTED_MCycle_1 },
    1
};
