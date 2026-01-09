
#include "cpu.h"

DMG_CPU::DMG_CPU(Emulator_Options* options, EmulatorState &state):
    m_Memory{ Memory_Handler() },
    m_regs{ Register_Handler() },
    m_options{ options },
    m_state{ state }
{

};

void DMG_CPU::Start_Emulation(std::vector<uint8_t>* rom) {
    m_Memory.Setup(rom, m_options->m_run_boot_rom);

    if (m_options->m_run_boot_rom) {
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

        m_Memory._Set(0xff00, 0xcf);
        m_Memory._Set(0xff01, 0x00);
        m_Memory._Set(0xff02, 0x7e);
        m_Memory._Set(0xff04, 0xab);
        m_Memory._Set(0xff05, 0x00);
        m_Memory._Set(0xff06, 0x00);
        m_Memory._Set(0xff07, 0xf8);
        m_Memory._Set(0xff0f, 0xe1);
        m_Memory._Set(0xff10, 0x80);
        m_Memory._Set(0xff11, 0xbf);
        m_Memory._Set(0xff12, 0xf3);
        m_Memory._Set(0xff13, 0xff);
        m_Memory._Set(0xff14, 0xbf);
        m_Memory._Set(0xff16, 0x3f);
        m_Memory._Set(0xff17, 0x00);
        m_Memory._Set(0xff18, 0xff);
        m_Memory._Set(0xff19, 0xbf);
        m_Memory._Set(0xff1a, 0x7f);
        m_Memory._Set(0xff1b, 0xff);
        m_Memory._Set(0xff1c, 0x9f);
        m_Memory._Set(0xff1d, 0xff);
        m_Memory._Set(0xff1e, 0xbf);
        m_Memory._Set(0xff20, 0xff);
        m_Memory._Set(0xff21, 0x00);
        m_Memory._Set(0xff22, 0x00);
        m_Memory._Set(0xff23, 0xfb);
        m_Memory._Set(0xff24, 0x77);
        m_Memory._Set(0xff25, 0xf3);
        m_Memory._Set(0xff26, 0xf1);
        m_Memory._Set(0xff40, 0x91);
        m_Memory._Set(0xff41, 0x81);
        m_Memory._Set(0xff42, 0x00);
        m_Memory._Set(0xff43, 0x00);
        m_Memory._Set(0xff44, 0x00);
        m_Memory._Set(0xff45, 0x00);
        m_Memory._Set(0xff46, 0xff);
        m_Memory._Set(0xff47, 0xfc);
        m_Memory._Set(0xff48, 0x00);
        m_Memory._Set(0xff49, 0x00);
        m_Memory._Set(0xff4a, 0x00);
        m_Memory._Set(0xff4b, 0x00);
        m_Memory._Set(0xffff, 0x00);
    }

    if (!m_Memory.hadLoadError() && !(m_options->m_strict_loading && m_Memory.hadLoadWarning())) {
        if (m_options->m_display_cart_info) { // Confirmation Screen, actually
            printf("Well, confirmation screen actually\n");
        }else {
            printf("COMPLETE: Emulation Setup: Emulation Beginning\n");
            m_state = State_InEmulator;
        }
    }else {
        printf("ERROR: Emulation Setup: Emulation Aborted\n");
    }
};