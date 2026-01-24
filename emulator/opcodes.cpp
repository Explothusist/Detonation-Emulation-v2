
#include "opcodes.h"
#include "memory.h"
#include "cpu.h"

#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4100)  // Unused parameter warning (MSVC specific)
#endif

// ----------------- Special -----------------
void NOP_MCycle_1(DMG_CPU &m_cpu) {
    // Do Nothing
};
Opcode Opcode_x00_NOP = { { NOP_MCycle_1 }, 1 };

void STOP_MCycle_1(DMG_CPU &m_cpu) {
    if (m_cpu.m_Memory._InterruptsPending()) {
        m_cpu.clearOpcode();
    }
};
void STOP_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.PC_Eat_Byte(); // Byte is discarded
    if (!m_cpu.WAKE()) {
        m_cpu.STOP();
    }
};
Opcode Opcode_x10_STOP = { { STOP_MCycle_1, STOP_MCycle_2 }, 2 };

void HALT_MCycle_1(DMG_CPU &m_cpu) {
    if (m_cpu.m_Memory._InterruptsEnabled()) {
        if (m_cpu.m_Memory._InterruptsPending()) {
            m_cpu.clearOpcode(); // Executes semi-normally
        }else {
            m_cpu.HALT();
        }
    }else {
        if (m_cpu.m_Memory._InterruptsPending()) {
            // Halt Bug
            m_cpu.HALT_Bug();
            m_cpu.clearOpcode();
        }else {
            m_cpu.HALT();
            // Fake NOP Executes
        }
    }
};
void HALT_MCycle_2(DMG_CPU &m_cpu) {
    // This is the fake NOP cycle that may or may not happen
};
Opcode Opcode_x76_HALT = { { HALT_MCycle_1, HALT_MCycle_2 }, 2 };

void DI_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_Memory._Set_IME(false);
};
void EI_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_Memory._Set_IME_Delayed(true);
};
Opcode Opcode_xF3_DI = { { DI_MCycle_1 }, 1 };
Opcode Opcode_xFB_EI = { { EI_MCycle_1 }, 1 };


// ----------------- Push and Pop b16 -----------------
template<Reg_u8 REG_H, Reg_u8 REG_L>
void PUSH_NN_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) - 1);
};
template<Reg_u8 REG_H, Reg_u8 REG_L>
void PUSH_NN_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(REG_H));
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) - 1);
};
template<Reg_u8 REG_H, Reg_u8 REG_L>
void PUSH_NN_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(REG_L));
};
template<Reg_u8 REG_H, Reg_u8 REG_L>
void PUSH_NN_MCycle_4(DMG_CPU &m_cpu) {
    // Fetch next opcode
};

Opcode Opcode_xC1_PUSH_BC = { { PUSH_NN_MCycle_1<Reg_u8::B, Reg_u8::C>, PUSH_NN_MCycle_2<Reg_u8::B, Reg_u8::C>, PUSH_NN_MCycle_3<Reg_u8::B, Reg_u8::C>, PUSH_NN_MCycle_4<Reg_u8::B, Reg_u8::C> }, 4 };
Opcode Opcode_xD1_PUSH_DE = { { PUSH_NN_MCycle_1<Reg_u8::D, Reg_u8::E>, PUSH_NN_MCycle_2<Reg_u8::D, Reg_u8::E>, PUSH_NN_MCycle_3<Reg_u8::D, Reg_u8::E>, PUSH_NN_MCycle_4<Reg_u8::D, Reg_u8::E> }, 4 };
Opcode Opcode_xE1_PUSH_HL = { { PUSH_NN_MCycle_1<Reg_u8::H, Reg_u8::L>, PUSH_NN_MCycle_2<Reg_u8::H, Reg_u8::L>, PUSH_NN_MCycle_3<Reg_u8::H, Reg_u8::L>, PUSH_NN_MCycle_4<Reg_u8::H, Reg_u8::L> }, 4 };
Opcode Opcode_xF1_PUSH_AF = { { PUSH_NN_MCycle_1<Reg_u8::A, Reg_u8::F>, PUSH_NN_MCycle_2<Reg_u8::A, Reg_u8::F>, PUSH_NN_MCycle_3<Reg_u8::A, Reg_u8::F>, PUSH_NN_MCycle_4<Reg_u8::A, Reg_u8::F> }, 4 };

template<Reg_u8 REG_H, Reg_u8 REG_L>
void POP_NN_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be free
};
template<Reg_u8 REG_H, Reg_u8 REG_L>
void POP_NN_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_regs.set(REG_L, m_cpu.m_Memory.Read());
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) + 1);
};
template<Reg_u8 REG_H, Reg_u8 REG_L>
void POP_NN_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_regs.set(REG_H, m_cpu.m_Memory.Read());
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) + 1);
};

Opcode Opcode_xC5_POP_BC = { { POP_NN_MCycle_1<Reg_u8::B, Reg_u8::C>, POP_NN_MCycle_2<Reg_u8::B, Reg_u8::C>, POP_NN_MCycle_3<Reg_u8::B, Reg_u8::C> }, 3 };
Opcode Opcode_xD5_POP_DE = { { POP_NN_MCycle_1<Reg_u8::D, Reg_u8::E>, POP_NN_MCycle_2<Reg_u8::D, Reg_u8::E>, POP_NN_MCycle_3<Reg_u8::D, Reg_u8::E> }, 3 };
Opcode Opcode_xE5_POP_HL = { { POP_NN_MCycle_1<Reg_u8::H, Reg_u8::L>, POP_NN_MCycle_2<Reg_u8::H, Reg_u8::L>, POP_NN_MCycle_3<Reg_u8::H, Reg_u8::L> }, 3 };
Opcode Opcode_xF5_POP_AF = { { POP_NN_MCycle_1<Reg_u8::A, Reg_u8::F>, POP_NN_MCycle_2<Reg_u8::A, Reg_u8::F>, POP_NN_MCycle_3<Reg_u8::A, Reg_u8::F> }, 3 };


// ----------------- Relative Jumps -----------------
void JR_MCycle_1(DMG_CPU &m_cpu) {
    // Lower byte of PC ends up in WZ after bus shenanigans
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.m_regs.get(Reg_u8::PC_L)); // Incidental, not functional
};
void JR_MCycle_2(DMG_CPU &m_cpu) {
    // Add PC to WZ
    int8_t offset = static_cast<int8_t>(m_cpu.PC_Eat_Byte());
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

Opcode Opcode_x18_JR = { { JR_MCycle_1, JR_MCycle_2, JR_MCycle_3 }, 3 };
Opcode Opcode_x20_JR_NZ = { { JR_F_MCycle_1<Reg_flag::NZ>, JR_F_MCycle_2<Reg_flag::NZ>, JR_F_MCycle_3<Reg_flag::NZ> }, 3 };
Opcode Opcode_x28_JR_Z = { { JR_F_MCycle_1<Reg_flag::Z>, JR_F_MCycle_2<Reg_flag::Z>, JR_F_MCycle_3<Reg_flag::Z> }, 3 };
Opcode Opcode_x30_JR_NC = { { JR_F_MCycle_1<Reg_flag::NC>, JR_F_MCycle_2<Reg_flag::NC>, JR_F_MCycle_3<Reg_flag::NC> }, 3 };
Opcode Opcode_x38_JR_C = { { JR_F_MCycle_1<Reg_flag::C>, JR_F_MCycle_2<Reg_flag::C>, JR_F_MCycle_3<Reg_flag::C> }, 3 };


// ----------------- Absolute Jumps -----------------
void JP_MCycle_1(DMG_CPU &m_cpu) {
    // Lower byte of PC ends up in WZ after bus shenanigans
    // m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.m_regs.get(Reg_u8::PC_L)); // Incidental, not functional
};
void JP_MCycle_2(DMG_CPU &m_cpu) {
    // Read into WZ
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.PC_Eat_Byte());
};
void JP_MCycle_3(DMG_CPU &m_cpu) {
    // Read into WZ
    m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.PC_Eat_Byte());
};
void JP_MCycle_4(DMG_CPU &m_cpu) {
    // Jump!
    m_cpu.m_regs.set(Reg_u16::PC, m_cpu.m_regs.get(Reg_u16::WZ));
};
template <Reg_flag FLAG>
void JP_F_MCycle_1(DMG_CPU &m_cpu) {
    JP_MCycle_1(m_cpu);
    m_cpu.m_regs.latchFlags();
};
template <Reg_flag FLAG>
void JP_F_MCycle_3(DMG_CPU &m_cpu) {
    JP_MCycle_3(m_cpu);
    if (!m_cpu.m_regs.getLatched(FLAG)) {
        m_cpu.clearOpcode(); // End the opcode earlier
    }
};

Opcode Opcode_xC3_JP = { { JP_MCycle_1, JP_MCycle_2, JP_MCycle_3, JP_MCycle_4 }, 4 };
Opcode Opcode_xC2_JP_NZ = { { JP_F_MCycle_1<Reg_flag::NZ>, JP_MCycle_2, JP_F_MCycle_3<Reg_flag::NZ>, JP_MCycle_4 }, 4 };
Opcode Opcode_xCA_JP_Z = { { JP_F_MCycle_1<Reg_flag::Z>, JP_MCycle_2, JP_F_MCycle_3<Reg_flag::Z>, JP_MCycle_4 }, 4 };
Opcode Opcode_xD2_JP_NC = { { JP_F_MCycle_1<Reg_flag::NC>, JP_MCycle_2, JP_F_MCycle_3<Reg_flag::NC>, JP_MCycle_4 }, 4 };
Opcode Opcode_xDA_JP_C = { { JP_F_MCycle_1<Reg_flag::C>, JP_MCycle_2, JP_F_MCycle_3<Reg_flag::C>, JP_MCycle_4 }, 4 };


// ----------------- Calls -----------------
void CALL_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus...
};
void CALL_MCycle_2(DMG_CPU &m_cpu) {
    // Read into WZ
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.PC_Eat_Byte());
};
void CALL_MCycle_3(DMG_CPU &m_cpu) {
    // Read into WZ
    m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.PC_Eat_Byte());
};
void CALL_MCycle_4(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) - 1);
};
void CALL_MCycle_5(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::PC_H));
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) - 1);
};
void CALL_MCycle_6(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::PC_L));
    m_cpu.m_regs.set(Reg_u16::PC, m_cpu.m_regs.get(Reg_u16::WZ));
};
template <Reg_flag FLAG>
void CALL_F_MCycle_1(DMG_CPU &m_cpu) {
    CALL_MCycle_1(m_cpu);
    m_cpu.m_regs.latchFlags();
};
template <Reg_flag FLAG>
void CALL_F_MCycle_3(DMG_CPU &m_cpu) {
    CALL_MCycle_3(m_cpu);
    if (!m_cpu.m_regs.getLatched(FLAG)) {
        m_cpu.clearOpcode(); // End the opcode earlier
    }
};
Opcode Opcode_xCD_CALL = { { CALL_MCycle_1, CALL_MCycle_2, CALL_MCycle_3, CALL_MCycle_4, CALL_MCycle_5, CALL_MCycle_6 }, 6 };
Opcode Opcode_xC4_CALL_NZ = { { CALL_F_MCycle_1<Reg_flag::NZ>, CALL_MCycle_2, CALL_F_MCycle_3<Reg_flag::NZ>, CALL_MCycle_4, CALL_MCycle_5, CALL_MCycle_6 }, 6 };
Opcode Opcode_xCC_CALL_Z = { { CALL_F_MCycle_1<Reg_flag::Z>, CALL_MCycle_2, CALL_F_MCycle_3<Reg_flag::Z>, CALL_MCycle_4, CALL_MCycle_5, CALL_MCycle_6 }, 6 };
Opcode Opcode_xD4_CALL_NC = { { CALL_F_MCycle_1<Reg_flag::NC>, CALL_MCycle_2, CALL_F_MCycle_3<Reg_flag::NC>, CALL_MCycle_4, CALL_MCycle_5, CALL_MCycle_6 }, 6 };
Opcode Opcode_xDC_CALL_C = { { CALL_F_MCycle_1<Reg_flag::C>, CALL_MCycle_2, CALL_F_MCycle_3<Reg_flag::C>, CALL_MCycle_4, CALL_MCycle_5, CALL_MCycle_6 }, 6 };


// ----------------- Returns -----------------
void RET_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be free
};
void RET_MCycle_2(DMG_CPU &m_cpu) {
    // Start loading new PC
    POP_NN_MCycle_2<Reg_u8::WZ_H, Reg_u8::WZ_L>(m_cpu);
};
void RET_MCycle_3(DMG_CPU &m_cpu) {
    // Finish loading new PC
    POP_NN_MCycle_3<Reg_u8::WZ_H, Reg_u8::WZ_L>(m_cpu);
};
void RET_MCycle_4(DMG_CPU &m_cpu) {
    // Jump to location
    m_cpu.m_regs.set(Reg_u16::PC, m_cpu.m_regs.get(Reg_u16::WZ));
};
template <Reg_flag FLAG>
void RET_F_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.latchFlags(); // Does nothing but check the condition
};
template <Reg_flag FLAG>
void RET_F_MCycle_2(DMG_CPU &m_cpu) {
    if (!m_cpu.m_regs.getLatched(FLAG)) {
        m_cpu.clearOpcode(); // Ends early with only 2 cycles
    } // Else continue with normal RET
};
void RETI_MCycle_4(DMG_CPU &m_cpu) {
    RET_MCycle_4(m_cpu);
    m_cpu.m_Memory._Set_IME(true);
};

Opcode Opcode_xC9_RET = { { RET_MCycle_1, RET_MCycle_2, RET_MCycle_3, RET_MCycle_4 }, 4 };
Opcode Opcode_xC0_RET_NZ = { { RET_F_MCycle_1<Reg_flag::NZ>, RET_F_MCycle_2<Reg_flag::NZ>, RET_MCycle_2, RET_MCycle_3, RET_MCycle_4 }, 5 };
Opcode Opcode_xC8_RET_Z = { { RET_F_MCycle_1<Reg_flag::Z>, RET_F_MCycle_2<Reg_flag::Z>, RET_MCycle_2, RET_MCycle_3, RET_MCycle_4 }, 5 };
Opcode Opcode_xD0_RET_NC = { { RET_F_MCycle_1<Reg_flag::NC>, RET_F_MCycle_2<Reg_flag::NC>, RET_MCycle_2, RET_MCycle_3, RET_MCycle_4 }, 5 };
Opcode Opcode_xD8_RET_C = { { RET_F_MCycle_1<Reg_flag::C>, RET_F_MCycle_2<Reg_flag::C>, RET_MCycle_2, RET_MCycle_3, RET_MCycle_4 }, 5 };
Opcode Opcode_xD9_RETI = { { RET_MCycle_1, RET_MCycle_2, RET_MCycle_3, RETI_MCycle_4 }, 4 };


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
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(REG1, m_cpu.m_Memory.Read());
};
template <Reg_u8 REG2>
void LD_HL_N_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to write
};
template <Reg_u8 REG2>
void LD_HL_N_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(REG2));
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


// ----------------- Special 8 bit Loads -----------------
void LDH_A8_A_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
void LDH_A8_A_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.PC_Eat_Byte());
};
void LDH_A8_A_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(0xff00 + m_cpu.m_regs.get(Reg_u8::temp_L));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::A));
};
void LDH_A_A8_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
void LDH_A_A8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.PC_Eat_Byte());
};
void LDH_A_A8_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(0xff00 + m_cpu.m_regs.get(Reg_u8::temp_L));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.m_Memory.Read());
};

Opcode Opcode_xE0_LDH_A8_A = { { LDH_A8_A_MCycle_1, LDH_A8_A_MCycle_2, LDH_A8_A_MCycle_3 }, 3 };
Opcode Opcode_xF0_LDH_A_A8 = { { LDH_A_A8_MCycle_1, LDH_A_A8_MCycle_2, LDH_A_A8_MCycle_3 }, 3 };

void LDH_C_A_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to write
};
void LDH_C_A_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(0xff00 + m_cpu.m_regs.get(Reg_u8::C));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::A));
};
void LDH_A_C_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
void LDH_A_C_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(0xff00 + m_cpu.m_regs.get(Reg_u8::C));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.m_Memory.Read());
};

Opcode Opcode_xE2_LDH_C_A = { { LDH_C_A_MCycle_1, LDH_C_A_MCycle_2 }, 2 };
Opcode Opcode_xF2_LDH_A_C = { { LDH_A_C_MCycle_1, LDH_A_C_MCycle_2 }, 2 };

template<Reg_u16 REG>
void LD_NN_A_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
template<Reg_u16 REG>
void LD_NN_A_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(REG));
    m_cpu.set(Reg_u8::A, m_cpu.m_Memory.Read());
};
template<Reg_u16 REG, int INC>
void LD_NNI_A_MCycle_2(DMG_CPU &m_cpu) {
    LD_NN_A_MCycle_2<REG>(m_cpu);
    m_cpu.m_regs.set(REG, m_cpu.m_regs.get(REG) + INC);
};

Opcode Opcode_x0A_LD_BC_A = { { LD_NN_A_MCycle_1<Reg_u16::BC>, LD_NN_A_MCycle_2<Reg_u16::BC> }, 2 };
Opcode Opcode_x1A_LD_DE_A = { { LD_NN_A_MCycle_1<Reg_u16::DE>, LD_NN_A_MCycle_2<Reg_u16::DE> }, 2 };
Opcode Opcode_x2A_LD_HLI_A = { { LD_NN_A_MCycle_1<Reg_u16::HL>, LD_NNI_A_MCycle_2<Reg_u16::HL, 1> }, 2 };
Opcode Opcode_x3A_LD_HLD_A = { { LD_NN_A_MCycle_1<Reg_u16::HL>, LD_NNI_A_MCycle_2<Reg_u16::HL, -1> }, 2 };

template<Reg_u16 REG>
void LD_A_NN_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to write
};
template<Reg_u16 REG>
void LD_A_NN_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(REG));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::A));
};
template<Reg_u16 REG, int INC>
void LD_A_NNI_MCycle_2(DMG_CPU &m_cpu) {
    LD_A_NN_MCycle_2<REG>(m_cpu);
    m_cpu.m_regs.set(REG, m_cpu.m_regs.get(REG) + INC);
};

Opcode Opcode_x02_LD_BC_A = { { LD_A_NN_MCycle_1<Reg_u16::BC>, LD_A_NN_MCycle_2<Reg_u16::BC> }, 2 };
Opcode Opcode_x12_LD_DE_A = { { LD_A_NN_MCycle_1<Reg_u16::DE>, LD_A_NN_MCycle_2<Reg_u16::DE> }, 2 };
Opcode Opcode_x22_LD_HLI_A = { { LD_A_NN_MCycle_1<Reg_u16::HL>, LD_NNI_A_MCycle_2<Reg_u16::HL, 1> }, 2 };
Opcode Opcode_x32_LD_HLD_A = { { LD_A_NN_MCycle_1<Reg_u16::HL>, LD_NNI_A_MCycle_2<Reg_u16::HL, -1> }, 2 };


// ----------------- 16 bit Loads -----------------
template <Reg_u8 REG_H, Reg_u8 REG_L>
void LD_NN_N16_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
template <Reg_u8 REG_H, Reg_u8 REG_L>
void LD_NN_N16_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(REG_L, m_cpu.PC_Eat_Byte());
};
template <Reg_u8 REG_H, Reg_u8 REG_L>
void LD_NN_N16_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(REG_H, m_cpu.PC_Eat_Byte());
};

Opcode Opcode_x01_LD_BC_N16 = { { LD_NN_N16_MCycle_1<Reg_u8::B, Reg_u8::C>, LD_NN_N16_MCycle_2<Reg_u8::B, Reg_u8::C>, LD_NN_N16_MCycle_3<Reg_u8::B, Reg_u8::C> }, 3 };
Opcode Opcode_x11_LD_DE_N16 = { { LD_NN_N16_MCycle_1<Reg_u8::D, Reg_u8::E>, LD_NN_N16_MCycle_2<Reg_u8::D, Reg_u8::E>, LD_NN_N16_MCycle_3<Reg_u8::D, Reg_u8::E> }, 3 };
Opcode Opcode_x21_LD_HL_N16 = { { LD_NN_N16_MCycle_1<Reg_u8::H, Reg_u8::L>, LD_NN_N16_MCycle_2<Reg_u8::H, Reg_u8::L>, LD_NN_N16_MCycle_3<Reg_u8::H, Reg_u8::L> }, 3 };
Opcode Opcode_x31_LD_SP_N16 = { { LD_NN_N16_MCycle_1<Reg_u8::SP_H, Reg_u8::SP_L>, LD_NN_N16_MCycle_2<Reg_u8::SP_H, Reg_u8::SP_L>, LD_NN_N16_MCycle_3<Reg_u8::SP_H, Reg_u8::SP_L> }, 3 };


// ----------------- 8 bit Addition -----------------
template<Reg_u8 REG>
void ADD_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG)));
};
void ADD_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Wait fo bus to be freed
};
void ADD_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_Memory.Read()));
};

template<Reg_u8 REG>
void ADC_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};
void ADC_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be free
};
void ADC_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_Memory.Read(), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
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

template<Reg_u8 REG>
void INC_N_MCycle_1(DMG_CPU &m_cpu) {
    bool c_flag = m_cpu.m_regs.get(Reg_flag::C);
    m_cpu.m_regs.set(REG, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(REG), 1));
    m_cpu.m_regs.set(Reg_flag::C, c_flag); // Preserve the C flag
};
void INC_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Wait fo bus to be freed
};
void INC_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read());
};
void INC_HL_MCycle_3(DMG_CPU &m_cpu) {
    bool c_flag = m_cpu.m_regs.get(Reg_flag::C);
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_Memory.Write(m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::temp_L), 1));
    m_cpu.m_regs.set(Reg_flag::C, c_flag); // Preserve the C flag
};

Opcode Opcode_x04_INC_B = { { INC_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x0C_INC_C = { { INC_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x14_INC_D = { { INC_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x1C_INC_E = { { INC_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x24_INC_H = { { INC_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x2C_INC_L = { { INC_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x34_INC_HL = { { INC_HL_MCycle_1, INC_HL_MCycle_2, INC_HL_MCycle_3 }, 3 };
Opcode Opcode_x3C_INC_A = { { INC_N_MCycle_1<Reg_u8::A> }, 1 };

template<Reg_u8 REG>
void DEC_N_MCycle_1(DMG_CPU &m_cpu) {
    bool c_flag = m_cpu.m_regs.get(Reg_flag::C);
    m_cpu.m_regs.set(REG, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(REG), 1));
    m_cpu.m_regs.set(Reg_flag::C, c_flag); // Preserve the C flag
};
void DEC_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Wait fo bus to be freed
};
void DEC_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read());
};
void DEC_HL_MCycle_3(DMG_CPU &m_cpu) {
    bool c_flag = m_cpu.m_regs.get(Reg_flag::C);
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_Memory.Write(m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::temp_L), 1));
    m_cpu.m_regs.set(Reg_flag::C, c_flag); // Preserve the C flag
};

Opcode Opcode_x05_DEC_B = { { DEC_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x0D_DEC_C = { { DEC_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x15_DEC_D = { { DEC_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x1D_DEC_E = { { DEC_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x25_DEC_H = { { DEC_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x2D_DEC_L = { { DEC_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x35_DEC_HL = { { DEC_HL_MCycle_1, DEC_HL_MCycle_2, DEC_HL_MCycle_3 }, 3 };
Opcode Opcode_x3D_DEC_A = { { DEC_N_MCycle_1<Reg_u8::A> }, 1 };


// ----------------- 8 bit Subtraction -----------------
template<Reg_u8 REG>
void SUB_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG)));
};
void SUB_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be freed
};
void SUB_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_Memory.Read()));
};

template<Reg_u8 REG>
void SBC_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};
void SBC_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be free
};
void SBC_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_Memory.Read(), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
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


// ----------------- 8 bit Bit Operations -----------------
template<Reg_u8 REG>
void AND_A_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) & m_cpu.m_regs.get(REG);
    m_cpu.m_regs.set(Reg_u8::A, result);
    m_cpu.m_regs.set(Reg_flag::Z, (result == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, true);
    m_cpu.m_regs.set(Reg_flag::C, false);
};
void AND_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void AND_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) & m_cpu.m_Memory.Read();
    m_cpu.m_regs.set(Reg_u8::A, result);
    m_cpu.m_regs.set(Reg_flag::Z, (result == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, true);
    m_cpu.m_regs.set(Reg_flag::C, false);
};

Opcode Opcode_xA0_AND_A_B = { { AND_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_xA1_AND_A_C = { { AND_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_xA2_AND_A_D = { { AND_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_xA3_AND_A_E = { { AND_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_xA4_AND_A_H = { { AND_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_xA5_AND_A_L = { { AND_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_xA6_AND_A_HL = { { AND_A_HL_MCycle_1, AND_A_HL_MCycle_2 }, 2 };
Opcode Opcode_xA7_AND_A_A = { { AND_A_N_MCycle_1<Reg_u8::A> }, 1 };

template<Reg_u8 REG>
void XOR_A_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) ^ m_cpu.m_regs.get(REG);
    m_cpu.m_regs.set(Reg_u8::A, result);
    m_cpu.m_regs.set(Reg_flag::Z, (result == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, false);
};
void XOR_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void XOR_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) ^ m_cpu.m_Memory.Read();
    m_cpu.m_regs.set(Reg_u8::A, result);
    m_cpu.m_regs.set(Reg_flag::Z, (result == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, false);
};

Opcode Opcode_xA8_XOR_A_B = { { XOR_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_xA9_XOR_A_C = { { XOR_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_xAA_XOR_A_D = { { XOR_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_xAB_XOR_A_E = { { XOR_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_xAC_XOR_A_H = { { XOR_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_xAD_XOR_A_L = { { XOR_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_xAE_XOR_A_HL = { { XOR_A_HL_MCycle_1, XOR_A_HL_MCycle_2 }, 2 };
Opcode Opcode_xAF_XOR_A_A = { { XOR_A_N_MCycle_1<Reg_u8::A> }, 1 };

template<Reg_u8 REG>
void OR_A_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) | m_cpu.m_regs.get(REG);
    m_cpu.m_regs.set(Reg_u8::A, result);
    m_cpu.m_regs.set(Reg_flag::Z, (result == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, false);
};
void OR_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void OR_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) | m_cpu.m_Memory.Read();
    m_cpu.m_regs.set(Reg_u8::A, result);
    m_cpu.m_regs.set(Reg_flag::Z, (result == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, false);
};

Opcode Opcode_xB0_OR_A_B = { { OR_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_xB1_OR_A_C = { { OR_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_xB2_OR_A_D = { { OR_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_xB3_OR_A_E = { { OR_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_xB4_OR_A_H = { { OR_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_xB5_OR_A_L = { { OR_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_xB6_OR_A_HL = { { OR_A_HL_MCycle_1, OR_A_HL_MCycle_2 }, 2 };
Opcode Opcode_xB7_OR_A_A = { { OR_A_N_MCycle_1<Reg_u8::A> }, 1 };

template<Reg_u8 REG>
void CP_A_N_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_regs.get(REG)); // Performs to set flags, then discards the result
};
void CP_A_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void CP_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_Memory.Read()); // Performs to set flags, then discards the result
};

Opcode Opcode_xB8_CP_A_B = { { CP_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_xB9_CP_A_C = { { CP_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_xBA_CP_A_D = { { CP_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_xBB_CP_A_E = { { CP_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_xBC_CP_A_H = { { CP_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_xBD_CP_A_L = { { CP_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_xBE_CP_A_HL = { { CP_A_HL_MCycle_1, CP_A_HL_MCycle_2 }, 2 };
Opcode Opcode_xBF_CP_A_A = { { CP_A_N_MCycle_1<Reg_u8::A> }, 1 };


// ----------------- 16 bit Addition -----------------
template<Reg_u16 REG, int INC>
void INC_NN_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be freed (because it uses post-increment)
};
template<Reg_u16 REG, int INC>
void INC_NN_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(REG, m_cpu.m_regs.get(REG) + INC);
};

Opcode Opcode_x03_INC_BC = { { INC_NN_MCycle_1<Reg_u16::BC, 1>, INC_NN_MCycle_2<Reg_u16::BC, 1> }, 2 };
Opcode Opcode_x13_INC_DE = { { INC_NN_MCycle_1<Reg_u16::DE, 1>, INC_NN_MCycle_2<Reg_u16::DE, 1> }, 2 };
Opcode Opcode_x23_INC_HL = { { INC_NN_MCycle_1<Reg_u16::HL, 1>, INC_NN_MCycle_2<Reg_u16::HL, 1> }, 2 };
Opcode Opcode_x33_INC_SP = { { INC_NN_MCycle_1<Reg_u16::SP, 1>, INC_NN_MCycle_2<Reg_u16::SP, 1> }, 2 };

Opcode Opcode_x0B_DEC_BC = { { INC_NN_MCycle_1<Reg_u16::BC, 1>, INC_NN_MCycle_2<Reg_u16::BC, -1> }, 2 };
Opcode Opcode_x1B_DEC_DE = { { INC_NN_MCycle_1<Reg_u16::DE, 1>, INC_NN_MCycle_2<Reg_u16::DE, -1> }, 2 };
Opcode Opcode_x2B_DEC_HL = { { INC_NN_MCycle_1<Reg_u16::HL, 1>, INC_NN_MCycle_2<Reg_u16::HL, -1> }, 2 };
Opcode Opcode_x3B_DEC_SP = { { INC_NN_MCycle_1<Reg_u16::SP, 1>, INC_NN_MCycle_2<Reg_u16::SP, -1> }, 2 };


// ----------------- Invalid -----------------
template<uint8_t OPCODE>
void INVALID_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Invalid Opcode: '%i'", OPCODE);
    m_cpu.callAbort();
};

Opcode Opcode_xD3_INVALID = { { INVALID_MCycle_1<0xD3> }, 1 };
Opcode Opcode_xDB_INVALID = { { INVALID_MCycle_1<0xDB> }, 1 };
Opcode Opcode_xDD_INVALID = { { INVALID_MCycle_1<0xDD> }, 1 };
Opcode Opcode_xE3_INVALID = { { INVALID_MCycle_1<0xE3> }, 1 };
Opcode Opcode_xE4_INVALID = { { INVALID_MCycle_1<0xE4> }, 1 };
Opcode Opcode_xEB_INVALID = { { INVALID_MCycle_1<0xEB> }, 1 };
Opcode Opcode_xEC_INVALID = { { INVALID_MCycle_1<0xEC> }, 1 };
Opcode Opcode_xED_INVALID = { { INVALID_MCycle_1<0xED> }, 1 };
Opcode Opcode_xF4_INVALID = { { INVALID_MCycle_1<0xF4> }, 1 };
Opcode Opcode_xFC_INVALID = { { INVALID_MCycle_1<0xFC> }, 1 };
Opcode Opcode_xFD_INVALID = { { INVALID_MCycle_1<0xFD> }, 1 };


// ----------------- Unimplemented -----------------
void UNIMPLEMENTED_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED = {
    { UNIMPLEMENTED_MCycle_1 },
    1
};

#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif