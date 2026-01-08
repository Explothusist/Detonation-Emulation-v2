#ifndef EMULATOR_UTILS_
#define EMULATOR_UTILS_

// #include <SDL3/SDL.h>
#include <vector>
#include <cstdint>
#include <string>

typedef enum {
    Key_Up,
    Key_Right,
    Key_Down,
    Key_Left,
    Key_Start,
    Key_Select,
    Key_A,
    Key_B
} Keybind;

std::vector<uint32_t>* get_default_keybindings();

// std::string get_string_from_keycode(uint32_t code);

class Emulator_Options { // Just a container
    public:
        Emulator_Options(bool run_boot_rom);

        void forget_temps();
        void save_temps();

        bool m_run_boot_rom;
        bool m_temp_run_boot_rom;
};

Emulator_Options* get_default_options();

#endif