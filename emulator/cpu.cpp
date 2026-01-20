
#include "cpu.h"

#include "../menus/MenuHandler.h"
#include "../SDL-Drawing-Library/DrawingContext.h" // This is mortally incorrect

DMG_CPU::DMG_CPU():
    m_Memory{ Memory_Handler() },
    m_regs{ Register_Handler() },
    m_ppu{ DMG_PPU() },
    m_apu{ DMG_APU() },
    m_cycle_count{ 0 },
    m_curr_opcode{ nullptr },
    m_curr_opcode_mcycle{ 0 },
    m_abort{ false }
{

};

Cart_Details DMG_CPU::Trigger_InitializeAll(std::vector<uint8_t>* rom, bool use_boot_rom) {
    Cart_Details cart_details = m_Memory.Initialize(rom, use_boot_rom);
    Initialize(use_boot_rom);
    m_ppu.Initialize();
    m_apu.Initialize();
    return cart_details;
};
void DMG_CPU::Trigger_PowerCycle() {
    m_Memory.PowerCycle();
    PowerCycle();
    m_ppu.PowerCycle();
    m_apu.PowerCycle();
};
void DMG_CPU::Trigger_ResetButton() {
    m_Memory.Reset();
    Reset();
    m_ppu.Reset();
    m_apu.Reset();
};

void DMG_CPU::Initialize(bool use_boot_rom) {
    if (use_boot_rom) {
        m_regs.set(Reg_u16::PC, 0);
        m_regs.set(Reg_u16::SP, 0xfffe); // 0xffff is not RAM
    }else {
        m_regs.set(Reg_u16::PC, 0x0100); // Assumes standard DMG Boot ROM was run
        m_regs.set(Reg_u8::A, 0x01);
        m_regs.set(Reg_u8::F, 0b1000'0000); // Z, n, H?, C?
        m_regs.set(Reg_u8::B, 0x00);
        m_regs.set(Reg_u8::C, 0x13);
        m_regs.set(Reg_u8::D, 0x00);
        m_regs.set(Reg_u8::E, 0xD8);
        m_regs.set(Reg_u8::H, 0x01);
        m_regs.set(Reg_u8::L, 0x46);
        m_regs.set(Reg_u16::SP, 0xfffe);
    }
};
void DMG_CPU::PowerCycle() {
    Reset(); // CPU never keeps anything
};
void DMG_CPU::Reset() {
    m_cycle_count = 0;
    m_abort = false;
    m_curr_opcode = nullptr;
    m_curr_opcode_mcycle = 0;
};


Opcode* DMG_CPU::parseOpcode(uint8_t opcode) {
    switch (opcode) {
        case 0x00: return &Opcode_x00_NOP;
        default: return &Opcode_xZZ_UNIMPLEMENTED;
    }
};
void DMG_CPU::runMCycle() {
    if (!m_curr_opcode || m_curr_opcode_mcycle >= m_curr_opcode->length) {
        uint8_t opcode = m_Memory.PC_Eat_Byte(m_regs);
        m_curr_opcode = parseOpcode(opcode);
        m_curr_opcode_mcycle = 0;
    }

    (m_curr_opcode->mcycles[m_curr_opcode_mcycle])(*this);
    m_curr_opcode_mcycle += 1;
};
void DMG_CPU::runTCycle() {
    // Optional later expansion
};

void DMG_CPU::callAbort() {
    m_abort = true;
};

// void DMG_CPU::consumeCycles(int cycles) { // Flip the system and splice the opcodes by cycle (or T-cycle accurate?)
//     m_cycle_count += cycles;

//     // pollEvents(); // Not so often
//     // m_Screen.runCycles(cycles, m_cycle_count); // Possibly not so often
//     // m_Sound.runCycles(cycles, m_cycle_count);

//     // if m_cycle_count > c_ONE_FRAME: m_Screen.presentRenderer(), m_cycle_count -= c_ONE_FRAME

//     // now = timeNS // Possibly avoid sleeping
//     // gap = last+c_4_CYCLES - now
//     // if gap > 0: delay(gap)
//     // last += c_4_CYCLES
// };
