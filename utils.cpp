
#include "utils.h"

#include <SDL3/SDL.h>

std::vector<uint32_t>* get_default_keybindings() {
    return new std::vector<uint32_t>({
        SDLK_UP,
        SDLK_RIGHT,
        SDLK_DOWN,
        SDLK_LEFT,
        SDLK_RETURN,
        SDLK_RSHIFT,
        SDLK_X,
        SDLK_Z
    });
    // return {
    //     0x40000052u,
    //     0x4000004fu,
    //     0x40000051u,
    //     0x40000050u,
    //     0x0000000du,
    //     0x400000e5u,
    //     0x00000078u,
    //     0x0000007au
    // };
};


// std::string get_string_from_keycode(uint32_t code) {
//     return SDL_GetKeyName(code); // Turns out this is easy, so this function is redundant
// };

Emulator_Options::Emulator_Options(bool run_boot_rom, bool strict_loading, bool display_cart_info):
    m_run_boot_rom{ run_boot_rom },
    m_temp_run_boot_rom{ run_boot_rom },
    m_strict_loading{ strict_loading },
    m_temp_strict_loading{ strict_loading },
    m_display_cart_info{ display_cart_info },
    m_temp_display_cart_info{ display_cart_info}
{
    
};
void Emulator_Options::forget_temps() {
    m_temp_run_boot_rom = m_run_boot_rom;
    m_temp_strict_loading = m_strict_loading;
    m_temp_display_cart_info = m_display_cart_info;
};
void Emulator_Options::save_temps() {
    m_run_boot_rom = m_temp_run_boot_rom;
    m_strict_loading = m_temp_strict_loading;
    m_display_cart_info = m_temp_display_cart_info;
};


Emulator_Options* get_default_options() {
    return new Emulator_Options(true, false, false);
};