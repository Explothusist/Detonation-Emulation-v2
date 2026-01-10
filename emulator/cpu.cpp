
#include "cpu.h"

#include "../menus/MenuHandler.h"
#include "../SDL-Drawing-Library/DrawingContext.h" // This is mortally incorrect

DMG_CPU::DMG_CPU(DrawingContext* ctx, Menu_Handler* menus, Emulator_Options* options, EmulatorState &state):
    m_Memory{ Memory_Handler() },
    m_regs{ Register_Handler() },
    m_ctx{ ctx },
    m_menus{ menus },
    m_options{ options },
    m_state{ state },
    m_initialized{ false },
    m_emulation_begun{ false }
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

    m_menus->replaceKeyEntriesOnMenu(m_Memory.getCartDetails(), Cartridge_Info); // Cartridge Info

    if (!m_Memory.hadLoadError() && !(m_options->m_strict_loading && m_Memory.hadLoadWarning())) {
        if (m_options->m_display_cart_info) { // Confirmation Screen, actually
            // printf("Well, confirmation screen actually\n");
            m_menus->switchToMenu(Cartridge_Info); // Cartridge Info
        }else { // Popup Time!!! WORKING HERE
            printf("COMPLETE: Emulation Setup: Emulation Beginning\n");
            m_menus->switchToMenu(Pause_Menu); // Pause Menu
            m_state = State_InEmulator;
            m_emulation_begun = true;
        }
    }else {
        printf("ERROR: Emulation Setup: Emulation Aborted\n");
        m_menus->createPopup(Popup(
            "An error has occured while loading the cartridge: "+m_Memory.getFirstError(),
            {"Cancel", "View Details"},
            {EntryEffect(EFFECT_NONE, -1), EntryEffect(EFFECT_TO_MENU, Cartridge_Info)}
        ));
    }
};
void DMG_CPU::Resume_Emulation() {
    if (!m_emulation_begun) {
        if (!m_Memory.hadLoadError()) {
            printf("COMPLETE: Emulation Setup: Emulation Beginning\n");
            m_menus->switchToMenu(Pause_Menu); // Pause Menu
            m_state = State_InEmulator;
            m_emulation_begun = true;
        }else {
            printf("ERROR: Emulation Setup: Emulation Aborted\n");
            m_menus->createPopup(Popup(
                "The cartridge cannot be loaded because of the error: "+m_Memory.getFirstError(),
                {"Cancel"},
                {EntryEffect(EFFECT_TO_MENU, -1)}
            ));
        }
    }else {
        m_state = State_InEmulator;
    }
};

void DMG_CPU::drawSelf() {
    m_ctx->background(0, 0, 0);

    int dmg_width = 160;
    int dmg_height = 144;

    int full_x = m_ctx->getScreenWidth();
    int full_y = m_ctx->getScreenHeight();
    int max_scale = std::min(full_x / dmg_width, full_y / dmg_height);

    int base_w = dmg_width * max_scale;
    int base_h = dmg_height * max_scale;

    int base_x = (full_x - base_w) / 2;
    int base_y = (full_y - base_h) / 2;

    m_ctx->fill(200, 200, 200);
    m_ctx->rect(base_x, base_y, base_w, base_h);

    m_ctx->presentRenderer();
};

bool DMG_CPU::hasInitialized() {
    return m_initialized;
};
void DMG_CPU::unInitialize() {
    m_initialized = false;
    m_Memory.Full_Reset();
};
