
#include "emulator_frontend.h"

#include "../menus/MenuHandler.h"
#include "../SDL-Drawing-Library/DrawingContext.h" // This is mortally incorrect

DMG_Emulator::DMG_Emulator(DrawingContext* ctx, Menu_Handler* menus, Emulator_Options* options, EmulatorState &state):
    m_ctx{ ctx },
    m_menus{ menus },
    m_options{ options },
    m_state{ state },
    m_cpu{ DMG_CPU() },
    m_cart_details{ Cart_Details() },
    m_initialized{ false },
    m_emulation_begun{ false }
{

};

void DMG_Emulator::Start_Emulation(std::vector<uint8_t>* rom) {
    m_cart_details = m_cpu.Trigger_InitializeAll(rom, m_options->m_run_boot_rom);

    m_menus->replaceKeyEntriesOnMenu(m_cart_details.getCartDetails(), Cartridge_Info); // Cartridge Info
    m_menus->setColorsOnMenu(m_cart_details.getCartDetailColors(), Cartridge_Info);

    if (!m_cart_details.hadLoadError() && !(m_options->m_strict_loading && m_cart_details.hadLoadWarning())) {
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
            "An error has occured while loading the cartridge: "+m_cart_details.getFirstError(),
            {"Cancel", "View Details"},
            {EntryEffect(EFFECT_UNINITIALIZE_EMULATOR, 0), EntryEffect(EFFECT_TO_MENU, Cartridge_Info)}
        ));
    }
    m_initialized = true;
};
void DMG_Emulator::Resume_Emulation() {
    if (!m_emulation_begun) {
        if (!m_cart_details.hadLoadError()) {
            printf("COMPLETE: Emulation Setup: Emulation Beginning\n");
            m_menus->switchToMenu(Pause_Menu); // Pause Menu
            m_state = State_InEmulator;
            m_emulation_begun = true;
        }else {
            printf("ERROR: Emulation Setup: Emulation Aborted\n");
            m_menus->createPopup(Popup(
                "The cartridge cannot be loaded because of the error: "+m_cart_details.getFirstError(),
                {"Cancel"},
                {EntryEffect(EFFECT_TO_MENU_UNINITIALIZE_EMULATOR, -1)}
            ));
        }
    }else {
        m_state = State_InEmulator;
    }
};

void DMG_Emulator::drawSelf() {
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

bool DMG_Emulator::hasInitialized() {
    return m_initialized;
};
void DMG_Emulator::unInitialize() {
    m_initialized = false;
    m_emulation_begun = false;
    m_cart_details = Cart_Details();
    m_cpu.Trigger_PowerCycle();
};
