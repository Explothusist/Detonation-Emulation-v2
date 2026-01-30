#ifndef EMULATOR_UTILS_
#define EMULATOR_UTILS_

// #include <SDL3/SDL.h>
#include <vector>
#include <cstdint>
#include <string>

#include "enable_logging.h"

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

typedef enum {
    State_InMenu,
    State_InEmulator
} EmulatorState;

typedef enum {
    Main_Menu,
    Pause_Menu,
    Recent_Menu,
    Keybindings_Menu,
    Options_Menu,
    Cartridge_Info,
#ifdef DEBUG_LOGGING
    Logfile_Menu_Main,
    Logfile_Menu_Pause
#endif
} Menu_ID;

typedef enum {
    EFFECT_NONE,
    EFFECT_TO_MENU,
    EFFECT_SELECT_ROM_RELOAD_RECENT,
    EFFECT_SET_KEYBIND,
    EFFECT_FORGET_KEYBIND,
    EFFECT_SAVE_KEYBIND,
    EFFECT_TOGGLE,
    EFFECT_FORGET_OPTIONS,
    EFFECT_SAVE_OPTIONS,
    EFFECT_LOAD_RECENT_RELOAD_RECENT,
    EFFECT_BACK_TO_EMULATOR,
    EFFECT_RETURN_TO_MAIN,
    EFFECT_ASK_LOAD_ANYWAY,
    EFFECT_LOAD_ANYWAY,
    EFFECT_TO_MENU_UNINITIALIZE_EMULATOR,
    EFFECT_UNINITIALIZE_EMULATOR,
#ifdef DEBUG_LOGGING
    EFFECT_DUMP_LOGFILE
#endif
} EntryEffectType;

typedef enum {
    TOGGLE_BOOT_ROM,
    TOGGLE_STRICT_LOADING,
    TOGGLE_DISPLAY_CART_INFO,
#ifdef DEBUG_LOGGING
    TOGGLE_LOG_ENABLE,
    TOGGLE_LOG_LENGTH
#endif
} ToggleID;

typedef enum {
    COLOR_BLACK,
    COLOR_ORANGE,
    COLOR_RED
} EntryColor;

std::vector<uint32_t>* get_default_keybindings();

// std::string get_string_from_keycode(uint32_t code);

class Emulator_Options { // Just a container
    public:
        Emulator_Options(bool run_boot_rom, bool strict_loading, bool display_cart_info, int log_length, bool log_enable);

        void forget_temps();
        void save_temps();

        bool m_run_boot_rom;
        bool m_temp_run_boot_rom;

        bool m_strict_loading;
        bool m_temp_strict_loading;

        bool m_display_cart_info;
        bool m_temp_display_cart_info;

        int m_log_length;
        int m_temp_log_length;

        bool m_log_enable;
        bool m_temp_log_enable;
};

Emulator_Options* get_default_options();

constexpr int k_Cycle_Per_Frame = 17556;
constexpr double k_Frame_Hertz = 59.7275;
constexpr double k_NS_Per_Frame = 16742706.46; // 1,000,000,000 / 59.7275


std::string eatNextChunk(std::string& to_parse);
std::string eatNextCommaChunk(std::string& to_parse);
std::string to_upper(std::string str);
std::string to_lower(std::string str);
std::string padLeft(std::string str, int length, char pad_with);
std::string padLeft(std::string str, int length);
std::string padRight(std::string str, int length, char pad_with);
std::string padRight(std::string str, int length);
std::string to_string_base(int num, int base);
std::string trim_ws(std::string str);

std::string to_b8_out(uint8_t num);
std::string to_b16_out(uint16_t num);

bool is_int(const std::string & s);

#endif