
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

void DAA_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t new_a = 0;
    bool new_c = false;
    if (m_cpu.m_regs.get(Reg_flag::N)) {
        uint8_t offset = (m_cpu.m_regs.get(Reg_flag::H) ? 0x06 : 0);
        if (m_cpu.m_regs.get(Reg_flag::C)) {
            offset += 0x60;
            new_c = true;
        }
        new_a = m_cpu.m_regs.get(Reg_u8::A) - offset;
        m_cpu.m_regs.set(Reg_u8::A, new_a);
    }else {
        uint8_t old_a = m_cpu.m_regs.get(Reg_u8::A);
        uint8_t offset = ((m_cpu.m_regs.get(Reg_flag::H) || (old_a & 0xf) > 0x9) ? 0x06 : 0);
        if (m_cpu.m_regs.get(Reg_flag::C) || old_a > 0x99) {
            offset += 0x60;
            new_c = true;
        }
        new_a = old_a + offset;
        m_cpu.m_regs.set(Reg_u8::A, new_a);
    }
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, new_c);
};
Opcode Opcode_x27_DAA = { { DAA_MCycle_1 }, 1 };

void CPL_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, ~m_cpu.m_regs.get(Reg_u8::A));
    m_cpu.m_regs.set(Reg_flag::N, true);
    m_cpu.m_regs.set(Reg_flag::H, true);
};
Opcode Opcode_x2F_CPL = { { CPL_MCycle_1 }, 1 };

void SCF_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, true);
};
void CCF_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, !m_cpu.m_regs.get(Reg_flag::C));
};
Opcode Opcode_x37_SCF = { { SCF_MCycle_1 }, 1 };
Opcode Opcode_x3F_CCF = { { CCF_MCycle_1 }, 1 };

void CB_Prefix_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.loadCBOpcode();
};
Opcode Opcode_xCB_CB_PREFIX = { { CB_Prefix_MCycle_1 }, 1 };


// ----------------- Stack Special -----------------
void LD_NN_SP_MCycle_1(DMG_CPU &m_cpu) {
    // Wait until bus is free...
};
void LD_NN_SP_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.PC_Eat_Byte());
};
void LD_NN_SP_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.PC_Eat_Byte());
};
void LD_NN_SP_MCycle_4(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::WZ));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::SP_L));
};
void LD_NN_SP_MCycle_5(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::WZ) + 1);
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::SP_H));
};
Opcode Opcode_x08_LD_NN_SP = { { LD_NN_SP_MCycle_1, LD_NN_SP_MCycle_2, LD_NN_SP_MCycle_3, LD_NN_SP_MCycle_4, LD_NN_SP_MCycle_5 }, 5 };

void ADD_SP_N_MCycle_1(DMG_CPU &m_cpu) {
    // Wait until bus is free...
};
void ADD_SP_N_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.PC_Eat_Byte());
};
void ADD_SP_N_MCycle_3(DMG_CPU &m_cpu) {
    // int8_t offset = static_cast<int8_t>(m_cpu.m_regs.get(Reg_u8::WZ_L));
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::WZ_L), m_cpu.m_regs.get(Reg_u8::SP_L)));
};
void ADD_SP_N_MCycle_4(DMG_CPU &m_cpu) {
    int8_t sign_bit = (m_cpu.m_regs.get(Reg_u8::WZ_L) >> 7);
    // m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.m_regs.get(Reg_u8::SP_H) + ((m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0) * (sign_bit == 1 ? -1 : 1)));
    m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.m_regs.get(Reg_u8::SP_H) +(m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::WZ));
    m_cpu.m_regs.set(Reg_flag::Z, false);
    m_cpu.m_regs.set(Reg_flag::N, false);
};
Opcode Opcode_xE8_ADD_SP_N = { { ADD_SP_N_MCycle_1, ADD_SP_N_MCycle_2, ADD_SP_N_MCycle_3, ADD_SP_N_MCycle_4 }, 4 };

void LD_HL_SP_N_MCycle_1(DMG_CPU &m_cpu) {
    // Wait until bus is free...
};
void LD_HL_SP_N_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::L, m_cpu.ALU_B8_ADDER(m_cpu.PC_Eat_Byte(), m_cpu.m_regs.get(Reg_u8::SP_L)));
};
void LD_HL_SP_N_MCycle_3(DMG_CPU &m_cpu) {
    // int8_t offset = static_cast<int8_t>(m_cpu.m_regs.get(Reg_u8::WZ_L));
    int8_t sign_bit = (m_cpu.m_regs.get(Reg_u8::WZ_L) >> 7);
    // m_cpu.m_regs.set(Reg_u8::H, m_cpu.m_regs.get(Reg_u8::SP_H) + ((m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0) * (sign_bit == 1 ? -1 : 1)));
    m_cpu.m_regs.set(Reg_u8::H, m_cpu.m_regs.get(Reg_u8::SP_H) + (m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
    m_cpu.m_regs.set(Reg_flag::Z, false);
    m_cpu.m_regs.set(Reg_flag::N, false);
};
Opcode Opcode_xF8_LD_HL_SP_N = { { LD_HL_SP_N_MCycle_1, LD_HL_SP_N_MCycle_2, LD_HL_SP_N_MCycle_3 }, 3 };

void LD_SP_HL_MCycle_1(DMG_CPU &m_cpu) {;
    m_cpu.m_regs.set(Reg_u8::SP_L, m_cpu.m_regs.get(Reg_u8::L));
};
void LD_SP_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::SP_H, m_cpu.m_regs.get(Reg_u8::H));
};
Opcode Opcode_xF9_LD_SP_HL = { { LD_SP_HL_MCycle_1, LD_SP_HL_MCycle_2 }, 2 };


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

void JP_HL_MCycle_1(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u16::PC, m_cpu.m_regs.get(Reg_u16::HL));
};
Opcode Opcode_xE9_JP_HL = { { JP_HL_MCycle_1 }, 1 };


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


// ----------------- Resets -----------------
template <uint16_t ADDR>
void RST_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus...
};
template <uint16_t ADDR>
void RST_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) - 1);
};
template <uint16_t ADDR>
void RST_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::PC_H));
    m_cpu.m_regs.set(Reg_u16::SP, m_cpu.m_regs.get(Reg_u16::SP) - 1);
};
template <uint16_t ADDR>
void RST_MCycle_4(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::SP));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::PC_L));
    m_cpu.m_regs.set(Reg_u16::PC, ADDR);
};

Opcode Opcode_xC7_RST_00 = { { RST_MCycle_1<0x0000>, RST_MCycle_2<0x0000>, RST_MCycle_3<0x0000>, RST_MCycle_4<0x0000> }, 4 };
Opcode Opcode_xCF_RST_08 = { { RST_MCycle_1<0x0008>, RST_MCycle_2<0x0008>, RST_MCycle_3<0x0008>, RST_MCycle_4<0x0008> }, 4 };
Opcode Opcode_xD7_RST_10 = { { RST_MCycle_1<0x0010>, RST_MCycle_2<0x0010>, RST_MCycle_3<0x0010>, RST_MCycle_4<0x0010> }, 4 };
Opcode Opcode_xDF_RST_18 = { { RST_MCycle_1<0x0018>, RST_MCycle_2<0x0018>, RST_MCycle_3<0x0018>, RST_MCycle_4<0x0018> }, 4 };
Opcode Opcode_xE7_RST_20 = { { RST_MCycle_1<0x0020>, RST_MCycle_2<0x0020>, RST_MCycle_3<0x0020>, RST_MCycle_4<0x0020> }, 4 };
Opcode Opcode_xEF_RST_28 = { { RST_MCycle_1<0x0028>, RST_MCycle_2<0x0028>, RST_MCycle_3<0x0028>, RST_MCycle_4<0x0028> }, 4 };
Opcode Opcode_xF7_RST_30 = { { RST_MCycle_1<0x0030>, RST_MCycle_2<0x0030>, RST_MCycle_3<0x0030>, RST_MCycle_4<0x0030> }, 4 };
Opcode Opcode_xFF_RST_38 = { { RST_MCycle_1<0x0038>, RST_MCycle_2<0x0038>, RST_MCycle_3<0x0038>, RST_MCycle_4<0x0038> }, 4 };


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
template <Reg_u8 REG>
void LD_N_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to read
};
template <Reg_u8 REG>
void LD_N_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(REG, m_cpu.PC_Eat_Byte());
};
void LD_HL_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Waits to be able to write
};
void LD_HL_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.PC_Eat_Byte());
};
void LD_HL_N8_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::temp_L));
};

Opcode Opcode_x06_LD_B_N8 = { { LD_N_N8_MCycle_1<Reg_u8::B>, LD_N_N8_MCycle_2<Reg_u8::B> }, 2 };
Opcode Opcode_x0E_LD_C_N8 = { { LD_N_N8_MCycle_1<Reg_u8::C>, LD_N_N8_MCycle_2<Reg_u8::C> }, 2 };
Opcode Opcode_x16_LD_D_N8 = { { LD_N_N8_MCycle_1<Reg_u8::D>, LD_N_N8_MCycle_2<Reg_u8::D> }, 2 };
Opcode Opcode_x1E_LD_E_N8 = { { LD_N_N8_MCycle_1<Reg_u8::E>, LD_N_N8_MCycle_2<Reg_u8::E> }, 2 };
Opcode Opcode_x26_LD_H_N8 = { { LD_N_N8_MCycle_1<Reg_u8::H>, LD_N_N8_MCycle_2<Reg_u8::H> }, 2 };
Opcode Opcode_x2E_LD_L_N8 = { { LD_N_N8_MCycle_1<Reg_u8::L>, LD_N_N8_MCycle_2<Reg_u8::L> }, 2 };
Opcode Opcode_x36_LD_HL_N8 = { { LD_HL_N8_MCycle_1, LD_HL_N8_MCycle_2, LD_HL_N8_MCycle_3 }, 3 };
Opcode Opcode_x3E_LD_A_N8 = { { LD_N_N8_MCycle_1<Reg_u8::A>, LD_N_N8_MCycle_2<Reg_u8::A> }, 2 };

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

void LD_N16_A_MCycle_1(DMG_CPU &m_cpu) {
    // Waits for bus to be free
};
void LD_N16_A_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.PC_Eat_Byte());
};
void LD_N16_A_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.PC_Eat_Byte());
};
void LD_N16_A_MCycle_4(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::WZ));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::A));
};
void LD_A_N16_MCycle_1(DMG_CPU &m_cpu) {
    // Waits for bus to be free
};
void LD_A_N16_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_L, m_cpu.PC_Eat_Byte());
};
void LD_A_N16_MCycle_3(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::WZ_H, m_cpu.PC_Eat_Byte());
};
void LD_A_N16_MCycle_4(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::WZ));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.m_Memory.Read());
};

Opcode Opcode_xEA_LD_N16_A = { { LD_N16_A_MCycle_1, LD_N16_A_MCycle_2, LD_N16_A_MCycle_3, LD_N16_A_MCycle_4 }, 4 };
Opcode Opcode_xFA_LD_A_N16 = { { LD_A_N16_MCycle_1, LD_A_N16_MCycle_2, LD_A_N16_MCycle_3, LD_A_N16_MCycle_4 }, 4 };


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
    // Wait for bus to be freed
};
void ADD_A_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.m_Memory.Read()));
};
void ADD_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be freed
};
void ADD_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.PC_Eat_Byte()));
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
void ADC_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be free
};
void ADC_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.PC_Eat_Byte(), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};

Opcode Opcode_x80_ADD_A_B = { { ADD_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x81_ADD_A_C = { { ADD_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x82_ADD_A_D = { { ADD_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x83_ADD_A_E = { { ADD_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x84_ADD_A_H = { { ADD_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x85_ADD_A_L = { { ADD_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x86_ADD_A_HL = { { ADD_A_HL_MCycle_1, ADD_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x87_ADD_A_A = { { ADD_A_N_MCycle_1<Reg_u8::A> }, 1 };
Opcode Opcode_xC6_ADD_A_N8 = { { ADD_A_N8_MCycle_1, ADD_A_N8_MCycle_2 }, 2 };

Opcode Opcode_x88_ADC_A_B = { { ADC_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x89_ADC_A_C = { { ADC_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x8A_ADC_A_D = { { ADC_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x8B_ADC_A_E = { { ADC_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x8C_ADC_A_H = { { ADC_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x8D_ADC_A_L = { { ADC_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x8E_ADC_A_HL = { { ADC_A_HL_MCycle_1, ADC_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x8F_ADC_A_A = { { ADC_A_N_MCycle_1<Reg_u8::A> }, 1 };
Opcode Opcode_xCE_ADC_A_N8 = { { ADC_A_N8_MCycle_1, ADC_A_N8_MCycle_2 }, 2 };

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
void SUB_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be freed
};
void SUB_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.PC_Eat_Byte()));
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
void SBC_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Wait for bus to be free
};
void SBC_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_regs.set(Reg_u8::A, m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.PC_Eat_Byte(), m_cpu.m_regs.get(Reg_flag::C) ? 1 : 0));
};

Opcode Opcode_x90_SUB_A_B = { { SUB_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x91_SUB_A_C = { { SUB_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x92_SUB_A_D = { { SUB_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x93_SUB_A_E = { { SUB_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x94_SUB_A_H = { { SUB_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x95_SUB_A_L = { { SUB_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x96_SUB_A_HL = { { SUB_A_HL_MCycle_1, SUB_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x97_SUB_A_A = { { SUB_A_N_MCycle_1<Reg_u8::A> }, 1 };
Opcode Opcode_xD6_SUB_A_N8 = { { SUB_A_N8_MCycle_1, SUB_A_N8_MCycle_2 }, 2 };

Opcode Opcode_x98_SBC_A_B = { { SBC_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_x99_SBC_A_C = { { SBC_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_x9A_SBC_A_D = { { SBC_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_x9B_SBC_A_E = { { SBC_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_x9C_SBC_A_H = { { SBC_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_x9D_SBC_A_L = { { SBC_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_x9E_SBC_A_HL = { { SBC_A_HL_MCycle_1, SBC_A_HL_MCycle_2 }, 2 };
Opcode Opcode_x9F_SBC_A_A = { { SBC_A_N_MCycle_1<Reg_u8::A> }, 1 };
Opcode Opcode_xDE_SBC_A_N8 = { { SBC_A_N8_MCycle_1, SBC_A_N8_MCycle_2 }, 2 };


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

template<Reg_u8 REG_H, Reg_u8 REG_L>
void ADD_HL_NN_MCycle_1(DMG_CPU &m_cpu) {
    bool z_flag = m_cpu.m_regs.get(Reg_flag::Z);
    m_cpu.m_regs.set(Reg_u8::L, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::L), m_cpu.m_regs.get(REG_L)));
    m_cpu.m_regs.set(Reg_flag::Z, z_flag);
};
template<Reg_u8 REG_H, Reg_u8 REG_L>
void ADD_HL_NN_MCycle_2(DMG_CPU &m_cpu) {
    bool z_flag = m_cpu.m_regs.get(Reg_flag::Z);
    m_cpu.m_regs.set(Reg_u8::H, m_cpu.ALU_B8_ADDER(m_cpu.m_regs.get(Reg_u8::H), m_cpu.m_regs.get(REG_H), m_cpu.m_regs.get(Reg_flag::C)));
    m_cpu.m_regs.set(Reg_flag::Z, z_flag);
};

Opcode Opcode_x09_ADD_HL_BC = { { ADD_HL_NN_MCycle_1<Reg_u8::B, Reg_u8::C>, ADD_HL_NN_MCycle_2<Reg_u8::B, Reg_u8::C> }, 2 };
Opcode Opcode_x19_ADD_HL_DE = { { ADD_HL_NN_MCycle_1<Reg_u8::D, Reg_u8::E>, ADD_HL_NN_MCycle_2<Reg_u8::D, Reg_u8::E> }, 2 };
Opcode Opcode_x29_ADD_HL_HL = { { ADD_HL_NN_MCycle_1<Reg_u8::H, Reg_u8::L>, ADD_HL_NN_MCycle_2<Reg_u8::H, Reg_u8::L> }, 2 };
Opcode Opcode_x39_ADD_HL_SP = { { ADD_HL_NN_MCycle_1<Reg_u8::SP_H, Reg_u8::SP_L>, ADD_HL_NN_MCycle_2<Reg_u8::SP_H, Reg_u8::SP_L> }, 2 };


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
void AND_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void AND_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) & m_cpu.PC_Eat_Byte();
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
Opcode Opcode_xE6_AND_A_N8 = { { AND_A_N8_MCycle_1, AND_A_N8_MCycle_2 }, 2 };

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
void XOR_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void XOR_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) ^ m_cpu.PC_Eat_Byte();
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
Opcode Opcode_xEE_XOR_A_N8 = { { XOR_A_N8_MCycle_1, XOR_A_N8_MCycle_2 }, 2 };

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
void OR_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void OR_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    uint8_t result = m_cpu.m_regs.get(Reg_u8::A) | m_cpu.PC_Eat_Byte();
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
Opcode Opcode_xF6_OR_A_N8 = { { OR_A_N8_MCycle_1, OR_A_N8_MCycle_2 }, 2 };

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
void CP_A_N8_MCycle_1(DMG_CPU &m_cpu) {
    // Waiting for bus to be free
};
void CP_A_N8_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.ALU_B8_SUBBER(m_cpu.m_regs.get(Reg_u8::A), m_cpu.PC_Eat_Byte()); // Performs to set flags, then discards the result
};

Opcode Opcode_xB8_CP_A_B = { { CP_A_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_xB9_CP_A_C = { { CP_A_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_xBA_CP_A_D = { { CP_A_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_xBB_CP_A_E = { { CP_A_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_xBC_CP_A_H = { { CP_A_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_xBD_CP_A_L = { { CP_A_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_xBE_CP_A_HL = { { CP_A_HL_MCycle_1, CP_A_HL_MCycle_2 }, 2 };
Opcode Opcode_xBF_CP_A_A = { { CP_A_N_MCycle_1<Reg_u8::A> }, 1 };
Opcode Opcode_xFE_CP_A_N8 = { { CP_A_N8_MCycle_1, CP_A_N8_MCycle_2 }, 2 };

template<Reg_u8 REG>
void RLC_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_7 = (old_a >> 7) & 1;
    uint8_t new_a = (old_a << 1) | bit_7;
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_7 == 1));
};
template<Reg_u8 REG>
void RL_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_7 = (old_a >> 7) & 1;
    uint8_t bit_8 = m_cpu.m_regs.get(Reg_flag::C);
    uint8_t new_a = (old_a << 1) | bit_8;
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_7 == 1));
};
template<Reg_u8 REG>
void RRC_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_0 = old_a & 1;
    uint8_t new_a = (old_a >> 1) | (bit_0 << 7);
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_0 == 1));
};
template<Reg_u8 REG>
void RR_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_0 = old_a & 1;
    uint8_t bit_8 = m_cpu.m_regs.get(Reg_flag::C);
    uint8_t new_a = (old_a >> 1) | (bit_8 << 7);
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_0 == 1));
};

void RLCA_MCycle_1(DMG_CPU &m_cpu) {
    RLC_N_MCycle_1<Reg_u8::A>(m_cpu);
    m_cpu.m_regs.set(Reg_flag::Z, false);
};
void RLA_MCycle_1(DMG_CPU &m_cpu) {
    RL_N_MCycle_1<Reg_u8::A>(m_cpu);
    m_cpu.m_regs.set(Reg_flag::Z, false);
};
void RRCA_MCycle_1(DMG_CPU &m_cpu) {
    RRC_N_MCycle_1<Reg_u8::A>(m_cpu);
    m_cpu.m_regs.set(Reg_flag::Z, false);
};
void RRA_N_MCycle_1(DMG_CPU &m_cpu) {
    RR_N_MCycle_1<Reg_u8::A>(m_cpu);
    m_cpu.m_regs.set(Reg_flag::Z, false);
};

Opcode Opcode_x07_RLCA = { { RLCA_MCycle_1 }, 1 };
Opcode Opcode_x0F_RRCA = { { RRCA_MCycle_1 }, 1 };
Opcode Opcode_x17_RLA = { { RLA_MCycle_1 }, 1 };
Opcode Opcode_x1F_RRA = { { RRA_N_MCycle_1 }, 1 };


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


// ----------------- CB Opcodes -----------------
void CB_HL_MCycle_1(DMG_CPU &m_cpu) {
    // Waits for bus to be free
};
void CB_HL_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read());
};
template<MCycleFn Opcode>
void CB_HL_MCycle_3(DMG_CPU &m_cpu) {
    Opcode(m_cpu);
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_Memory.Write(m_cpu.m_regs.get(Reg_u8::temp_L));
};

Opcode Opcode_CB_x00_RLC_B = { { RLC_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x01_RLC_C = { { RLC_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x02_RLC_D = { { RLC_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x03_RLC_E = { { RLC_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x04_RLC_H = { { RLC_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x05_RLC_L = { { RLC_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x06_RLC_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RLC_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x07_RLC_A = { { RLC_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_CB_x08_RRC_B = { { RRC_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x09_RRC_C = { { RRC_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x0A_RRC_D = { { RRC_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x0B_RRC_E = { { RRC_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x0C_RRC_H = { { RRC_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x0D_RRC_L = { { RRC_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x0E_RRC_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RRC_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x0F_RRC_A = { { RRC_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_CB_x10_RL_B = { { RL_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x11_RL_C = { { RL_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x12_RL_D = { { RL_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x13_RL_E = { { RL_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x14_RL_H = { { RL_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x15_RL_L = { { RL_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x16_RL_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RL_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x17_RL_A = { { RL_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_CB_x18_RR_B = { { RR_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x19_RR_C = { { RR_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x1A_RR_D = { { RR_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x1B_RR_E = { { RR_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x1C_RR_H = { { RR_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x1D_RR_L = { { RR_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x1E_RR_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RR_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x1F_RR_A = { { RR_N_MCycle_1<Reg_u8::A> }, 1 };

template<Reg_u8 REG>
void SLA_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_7 = (old_a >> 7) & 1;
    uint8_t new_a = (old_a << 1);
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_7 == 1));
};
template<Reg_u8 REG>
void SRA_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_7 = old_a & 0b1000'0000;
    uint8_t bit_0 = old_a & 1;
    uint8_t new_a = (old_a >> 1) | bit_7;
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_0 == 1));
};
template<Reg_u8 REG>
void SWAP_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t new_a = (old_a << 4) | (old_a >> 4);
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, false);
};
template<Reg_u8 REG>
void SRL_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_a = m_cpu.m_regs.get(REG);
    uint8_t bit_0 = old_a & 1;
    uint8_t new_a = (old_a >> 1);
    m_cpu.m_regs.set(REG, new_a);
    m_cpu.m_regs.set(Reg_flag::Z, (new_a == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, false);
    m_cpu.m_regs.set(Reg_flag::C, (bit_0 == 1));
};

Opcode Opcode_CB_x20_SLA_B = { { SLA_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x21_SLA_C = { { SLA_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x22_SLA_D = { { SLA_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x23_SLA_E = { { SLA_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x24_SLA_H = { { SLA_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x25_SLA_L = { { SLA_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x26_SLA_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SLA_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x27_SLA_A = { { SLA_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_CB_x28_SRA_B = { { SRA_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x29_SRA_C = { { SRA_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x2A_SRA_D = { { SRA_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x2B_SRA_E = { { SRA_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x2C_SRA_H = { { SRA_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x2D_SRA_L = { { SRA_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x2E_SRA_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SRA_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x2F_SRA_A = { { SRA_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_CB_x30_SWAP_B = { { SWAP_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x31_SWAP_C = { { SWAP_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x32_SWAP_D = { { SWAP_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x33_SWAP_E = { { SWAP_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x34_SWAP_H = { { SWAP_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x35_SWAP_L = { { SWAP_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x36_SWAP_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SWAP_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x37_SWAP_A = { { SWAP_N_MCycle_1<Reg_u8::A> }, 1 };

Opcode Opcode_CB_x38_SRL_B = { { SRL_N_MCycle_1<Reg_u8::B> }, 1 };
Opcode Opcode_CB_x39_SRL_C = { { SRL_N_MCycle_1<Reg_u8::C> }, 1 };
Opcode Opcode_CB_x3A_SRL_D = { { SRL_N_MCycle_1<Reg_u8::D> }, 1 };
Opcode Opcode_CB_x3B_SRL_E = { { SRL_N_MCycle_1<Reg_u8::E> }, 1 };
Opcode Opcode_CB_x3C_SRL_H = { { SRL_N_MCycle_1<Reg_u8::H> }, 1 };
Opcode Opcode_CB_x3D_SRL_L = { { SRL_N_MCycle_1<Reg_u8::L> }, 1 };
Opcode Opcode_CB_x3E_SRL_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SRL_N_MCycle_1<Reg_u8::temp_L>> }, 3 };
Opcode Opcode_CB_x3F_SRL_A = { { SRL_N_MCycle_1<Reg_u8::A> }, 1 };

template<int BIT, Reg_u8 REG>
void BIT_X_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_reg = m_cpu.m_regs.get(REG);
    uint8_t bit_x = (old_reg >> BIT) & 1;
    m_cpu.m_regs.set(Reg_flag::Z, (bit_x == 0));
    m_cpu.m_regs.set(Reg_flag::N, false);
    m_cpu.m_regs.set(Reg_flag::H, true);
};
void CB_HL_BIT_MCycle_1(DMG_CPU &m_cpu) {
    // Waits for bus to be free
};
template<MCycleFn Opcode>
void CB_HL_BIT_MCycle_2(DMG_CPU &m_cpu) {
    m_cpu.m_Memory.latchBus(m_cpu.m_regs.get(Reg_u16::HL));
    m_cpu.m_regs.set(Reg_u8::temp_L, m_cpu.m_Memory.Read());
    Opcode(m_cpu);
};

Opcode Opcode_CB_x40_BIT_0_B = { { BIT_X_N_MCycle_1<0, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x41_BIT_0_C = { { BIT_X_N_MCycle_1<0, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x42_BIT_0_D = { { BIT_X_N_MCycle_1<0, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x43_BIT_0_E = { { BIT_X_N_MCycle_1<0, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x44_BIT_0_H = { { BIT_X_N_MCycle_1<0, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x45_BIT_0_L = { { BIT_X_N_MCycle_1<0, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x46_BIT_0_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<0, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x47_BIT_0_A = { { BIT_X_N_MCycle_1<0, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x48_BIT_1_B = { { BIT_X_N_MCycle_1<1, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x49_BIT_1_C = { { BIT_X_N_MCycle_1<1, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x4A_BIT_1_D = { { BIT_X_N_MCycle_1<1, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x4B_BIT_1_E = { { BIT_X_N_MCycle_1<1, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x4C_BIT_1_H = { { BIT_X_N_MCycle_1<1, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x4D_BIT_1_L = { { BIT_X_N_MCycle_1<1, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x4E_BIT_1_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<1, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x4F_BIT_1_A = { { BIT_X_N_MCycle_1<1, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x50_BIT_2_B = { { BIT_X_N_MCycle_1<2, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x51_BIT_2_C = { { BIT_X_N_MCycle_1<2, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x52_BIT_2_D = { { BIT_X_N_MCycle_1<2, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x53_BIT_2_E = { { BIT_X_N_MCycle_1<2, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x54_BIT_2_H = { { BIT_X_N_MCycle_1<2, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x55_BIT_2_L = { { BIT_X_N_MCycle_1<2, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x56_BIT_2_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<2, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x57_BIT_2_A = { { BIT_X_N_MCycle_1<2, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x58_BIT_3_B = { { BIT_X_N_MCycle_1<3, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x59_BIT_3_C = { { BIT_X_N_MCycle_1<3, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x5A_BIT_3_D = { { BIT_X_N_MCycle_1<3, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x5B_BIT_3_E = { { BIT_X_N_MCycle_1<3, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x5C_BIT_3_H = { { BIT_X_N_MCycle_1<3, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x5D_BIT_3_L = { { BIT_X_N_MCycle_1<3, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x5E_BIT_3_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<3, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x5F_BIT_3_A = { { BIT_X_N_MCycle_1<3, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x60_BIT_4_B = { { BIT_X_N_MCycle_1<4, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x61_BIT_4_C = { { BIT_X_N_MCycle_1<4, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x62_BIT_4_D = { { BIT_X_N_MCycle_1<4, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x63_BIT_4_E = { { BIT_X_N_MCycle_1<4, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x64_BIT_4_H = { { BIT_X_N_MCycle_1<4, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x65_BIT_4_L = { { BIT_X_N_MCycle_1<4, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x66_BIT_4_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<4, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x67_BIT_4_A = { { BIT_X_N_MCycle_1<4, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x68_BIT_5_B = { { BIT_X_N_MCycle_1<5, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x69_BIT_5_C = { { BIT_X_N_MCycle_1<5, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x6A_BIT_5_D = { { BIT_X_N_MCycle_1<5, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x6B_BIT_5_E = { { BIT_X_N_MCycle_1<5, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x6C_BIT_5_H = { { BIT_X_N_MCycle_1<5, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x6D_BIT_5_L = { { BIT_X_N_MCycle_1<5, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x6E_BIT_5_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<5, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x6F_BIT_5_A = { { BIT_X_N_MCycle_1<5, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x70_BIT_6_B = { { BIT_X_N_MCycle_1<6, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x71_BIT_6_C = { { BIT_X_N_MCycle_1<6, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x72_BIT_6_D = { { BIT_X_N_MCycle_1<6, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x73_BIT_6_E = { { BIT_X_N_MCycle_1<6, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x74_BIT_6_H = { { BIT_X_N_MCycle_1<6, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x75_BIT_6_L = { { BIT_X_N_MCycle_1<6, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x76_BIT_6_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<6, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x77_BIT_6_A = { { BIT_X_N_MCycle_1<6, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x78_BIT_7_B = { { BIT_X_N_MCycle_1<7, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x79_BIT_7_C = { { BIT_X_N_MCycle_1<7, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x7A_BIT_7_D = { { BIT_X_N_MCycle_1<7, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x7B_BIT_7_E = { { BIT_X_N_MCycle_1<7, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x7C_BIT_7_H = { { BIT_X_N_MCycle_1<7, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x7D_BIT_7_L = { { BIT_X_N_MCycle_1<7, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x7E_BIT_7_HL = { { CB_HL_BIT_MCycle_1, CB_HL_BIT_MCycle_2<BIT_X_N_MCycle_1<7, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x7F_BIT_7_A = { { BIT_X_N_MCycle_1<7, Reg_u8::A> }, 1 };

template<int BIT, Reg_u8 REG>
void RES_X_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_reg = m_cpu.m_regs.get(REG);
    uint8_t new_reg = old_reg & ~(1 << BIT);
    m_cpu.m_regs.set(REG, new_reg);
};

Opcode Opcode_CB_x80_RES_0_B = { { RES_X_N_MCycle_1<0, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x81_RES_0_C = { { RES_X_N_MCycle_1<0, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x82_RES_0_D = { { RES_X_N_MCycle_1<0, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x83_RES_0_E = { { RES_X_N_MCycle_1<0, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x84_RES_0_H = { { RES_X_N_MCycle_1<0, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x85_RES_0_L = { { RES_X_N_MCycle_1<0, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x86_RES_0_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<0, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x87_RES_0_A = { { RES_X_N_MCycle_1<0, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x88_RES_1_B = { { RES_X_N_MCycle_1<1, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x89_RES_1_C = { { RES_X_N_MCycle_1<1, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x8A_RES_1_D = { { RES_X_N_MCycle_1<1, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x8B_RES_1_E = { { RES_X_N_MCycle_1<1, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x8C_RES_1_H = { { RES_X_N_MCycle_1<1, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x8D_RES_1_L = { { RES_X_N_MCycle_1<1, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x8E_RES_1_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<1, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x8F_RES_1_A = { { RES_X_N_MCycle_1<1, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x90_RES_2_B = { { RES_X_N_MCycle_1<2, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x91_RES_2_C = { { RES_X_N_MCycle_1<2, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x92_RES_2_D = { { RES_X_N_MCycle_1<2, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x93_RES_2_E = { { RES_X_N_MCycle_1<2, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x94_RES_2_H = { { RES_X_N_MCycle_1<2, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x95_RES_2_L = { { RES_X_N_MCycle_1<2, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x96_RES_2_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<2, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x97_RES_2_A = { { RES_X_N_MCycle_1<2, Reg_u8::A> }, 1 };

Opcode Opcode_CB_x98_RES_3_B = { { RES_X_N_MCycle_1<3, Reg_u8::B> }, 1 };
Opcode Opcode_CB_x99_RES_3_C = { { RES_X_N_MCycle_1<3, Reg_u8::C> }, 1 };
Opcode Opcode_CB_x9A_RES_3_D = { { RES_X_N_MCycle_1<3, Reg_u8::D> }, 1 };
Opcode Opcode_CB_x9B_RES_3_E = { { RES_X_N_MCycle_1<3, Reg_u8::E> }, 1 };
Opcode Opcode_CB_x9C_RES_3_H = { { RES_X_N_MCycle_1<3, Reg_u8::H> }, 1 };
Opcode Opcode_CB_x9D_RES_3_L = { { RES_X_N_MCycle_1<3, Reg_u8::L> }, 1 };
Opcode Opcode_CB_x9E_RES_3_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<3, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_x9F_RES_3_A = { { RES_X_N_MCycle_1<3, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xA0_RES_4_B = { { RES_X_N_MCycle_1<4, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xA1_RES_4_C = { { RES_X_N_MCycle_1<4, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xA2_RES_4_D = { { RES_X_N_MCycle_1<4, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xA3_RES_4_E = { { RES_X_N_MCycle_1<4, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xA4_RES_4_H = { { RES_X_N_MCycle_1<4, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xA5_RES_4_L = { { RES_X_N_MCycle_1<4, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xA6_RES_4_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<4, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xA7_RES_4_A = { { RES_X_N_MCycle_1<4, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xA8_RES_5_B = { { RES_X_N_MCycle_1<5, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xA9_RES_5_C = { { RES_X_N_MCycle_1<5, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xAA_RES_5_D = { { RES_X_N_MCycle_1<5, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xAB_RES_5_E = { { RES_X_N_MCycle_1<5, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xAC_RES_5_H = { { RES_X_N_MCycle_1<5, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xAD_RES_5_L = { { RES_X_N_MCycle_1<5, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xAE_RES_5_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<5, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xAF_RES_5_A = { { RES_X_N_MCycle_1<5, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xB0_RES_6_B = { { RES_X_N_MCycle_1<6, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xB1_RES_6_C = { { RES_X_N_MCycle_1<6, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xB2_RES_6_D = { { RES_X_N_MCycle_1<6, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xB3_RES_6_E = { { RES_X_N_MCycle_1<6, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xB4_RES_6_H = { { RES_X_N_MCycle_1<6, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xB5_RES_6_L = { { RES_X_N_MCycle_1<6, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xB6_RES_6_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<6, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xB7_RES_6_A = { { RES_X_N_MCycle_1<6, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xB8_RES_7_B = { { RES_X_N_MCycle_1<7, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xB9_RES_7_C = { { RES_X_N_MCycle_1<7, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xBA_RES_7_D = { { RES_X_N_MCycle_1<7, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xBB_RES_7_E = { { RES_X_N_MCycle_1<7, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xBC_RES_7_H = { { RES_X_N_MCycle_1<7, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xBD_RES_7_L = { { RES_X_N_MCycle_1<7, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xBE_RES_7_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<RES_X_N_MCycle_1<7, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xBF_RES_7_A = { { RES_X_N_MCycle_1<7, Reg_u8::A> }, 1 };

template<int BIT, Reg_u8 REG>
void SET_X_N_MCycle_1(DMG_CPU &m_cpu) {
    uint8_t old_reg = m_cpu.m_regs.get(REG);
    uint8_t new_reg = old_reg | (1 << BIT);
    m_cpu.m_regs.set(REG, new_reg);
};

Opcode Opcode_CB_xC0_SET_0_B = { { SET_X_N_MCycle_1<0, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xC1_SET_0_C = { { SET_X_N_MCycle_1<0, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xC2_SET_0_D = { { SET_X_N_MCycle_1<0, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xC3_SET_0_E = { { SET_X_N_MCycle_1<0, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xC4_SET_0_H = { { SET_X_N_MCycle_1<0, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xC5_SET_0_L = { { SET_X_N_MCycle_1<0, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xC6_SET_0_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<0, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xC7_SET_0_A = { { SET_X_N_MCycle_1<0, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xC8_SET_1_B = { { SET_X_N_MCycle_1<1, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xC9_SET_1_C = { { SET_X_N_MCycle_1<1, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xCA_SET_1_D = { { SET_X_N_MCycle_1<1, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xCB_SET_1_E = { { SET_X_N_MCycle_1<1, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xCC_SET_1_H = { { SET_X_N_MCycle_1<1, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xCD_SET_1_L = { { SET_X_N_MCycle_1<1, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xCE_SET_1_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<1, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xCF_SET_1_A = { { SET_X_N_MCycle_1<1, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xD0_SET_2_B = { { SET_X_N_MCycle_1<2, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xD1_SET_2_C = { { SET_X_N_MCycle_1<2, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xD2_SET_2_D = { { SET_X_N_MCycle_1<2, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xD3_SET_2_E = { { SET_X_N_MCycle_1<2, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xD4_SET_2_H = { { SET_X_N_MCycle_1<2, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xD5_SET_2_L = { { SET_X_N_MCycle_1<2, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xD6_SET_2_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<2, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xD7_SET_2_A = { { SET_X_N_MCycle_1<2, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xD8_SET_3_B = { { SET_X_N_MCycle_1<3, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xD9_SET_3_C = { { SET_X_N_MCycle_1<3, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xDA_SET_3_D = { { SET_X_N_MCycle_1<3, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xDB_SET_3_E = { { SET_X_N_MCycle_1<3, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xDC_SET_3_H = { { SET_X_N_MCycle_1<3, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xDD_SET_3_L = { { SET_X_N_MCycle_1<3, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xDE_SET_3_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<3, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xDF_SET_3_A = { { SET_X_N_MCycle_1<3, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xE0_SET_4_B = { { SET_X_N_MCycle_1<4, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xE1_SET_4_C = { { SET_X_N_MCycle_1<4, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xE2_SET_4_D = { { SET_X_N_MCycle_1<4, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xE3_SET_4_E = { { SET_X_N_MCycle_1<4, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xE4_SET_4_H = { { SET_X_N_MCycle_1<4, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xE5_SET_4_L = { { SET_X_N_MCycle_1<4, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xE6_SET_4_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<4, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xE7_SET_4_A = { { SET_X_N_MCycle_1<4, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xE8_SET_5_B = { { SET_X_N_MCycle_1<5, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xE9_SET_5_C = { { SET_X_N_MCycle_1<5, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xEA_SET_5_D = { { SET_X_N_MCycle_1<5, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xEB_SET_5_E = { { SET_X_N_MCycle_1<5, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xEC_SET_5_H = { { SET_X_N_MCycle_1<5, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xED_SET_5_L = { { SET_X_N_MCycle_1<5, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xEE_SET_5_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<5, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xEF_SET_5_A = { { SET_X_N_MCycle_1<5, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xF0_SET_6_B = { { SET_X_N_MCycle_1<6, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xF1_SET_6_C = { { SET_X_N_MCycle_1<6, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xF2_SET_6_D = { { SET_X_N_MCycle_1<6, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xF3_SET_6_E = { { SET_X_N_MCycle_1<6, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xF4_SET_6_H = { { SET_X_N_MCycle_1<6, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xF5_SET_6_L = { { SET_X_N_MCycle_1<6, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xF6_SET_6_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<6, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xF7_SET_6_A = { { SET_X_N_MCycle_1<6, Reg_u8::A> }, 1 };

Opcode Opcode_CB_xF8_SET_7_B = { { SET_X_N_MCycle_1<7, Reg_u8::B> }, 1 };
Opcode Opcode_CB_xF9_SET_7_C = { { SET_X_N_MCycle_1<7, Reg_u8::C> }, 1 };
Opcode Opcode_CB_xFA_SET_7_D = { { SET_X_N_MCycle_1<7, Reg_u8::D> }, 1 };
Opcode Opcode_CB_xFB_SET_7_E = { { SET_X_N_MCycle_1<7, Reg_u8::E> }, 1 };
Opcode Opcode_CB_xFC_SET_7_H = { { SET_X_N_MCycle_1<7, Reg_u8::H> }, 1 };
Opcode Opcode_CB_xFD_SET_7_L = { { SET_X_N_MCycle_1<7, Reg_u8::L> }, 1 };
Opcode Opcode_CB_xFE_SET_7_HL = { { CB_HL_MCycle_1, CB_HL_MCycle_2, CB_HL_MCycle_3<SET_X_N_MCycle_1<7, Reg_u8::temp_L>> }, 2 };
Opcode Opcode_CB_xFF_SET_7_A = { { SET_X_N_MCycle_1<7, Reg_u8::A> }, 1 };


// ----------------- Unimplemented -----------------
void UNIMPLEMENTED_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED = { { UNIMPLEMENTED_MCycle_1 }, 1 };
void UNIMPLEMENTED_CB_MCycle_1(DMG_CPU &m_cpu) {
    printf("ERROR: Unimplemented CB Opcode");
    m_cpu.callAbort();
};
Opcode Opcode_xZZ_UNIMPLEMENTED_CB = { { UNIMPLEMENTED_CB_MCycle_1 }, 1 };

#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif
#ifdef __clang__
    #pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
    #pragma warning(pop)
#endif