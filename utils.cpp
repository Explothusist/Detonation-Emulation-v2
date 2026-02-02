
#include "utils.h"

#include <SDL3/SDL.h>
#include <algorithm>

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

Emulator_Options::Emulator_Options(bool run_boot_rom, bool strict_loading, bool display_cart_info, int log_length, bool log_enable):
    m_run_boot_rom{ run_boot_rom },
    m_temp_run_boot_rom{ run_boot_rom },
    m_strict_loading{ strict_loading },
    m_temp_strict_loading{ strict_loading },
    m_display_cart_info{ display_cart_info },
    m_temp_display_cart_info{ display_cart_info },
    m_log_length{ log_length },
    m_temp_log_length{ log_length },
    m_log_enable{ log_enable },
    m_temp_log_enable{ log_enable }
{
    
};
void Emulator_Options::forget_temps() {
    m_temp_run_boot_rom = m_run_boot_rom;
    m_temp_strict_loading = m_strict_loading;
    m_temp_display_cart_info = m_display_cart_info;
    m_temp_log_length = m_log_length;
    m_temp_log_enable = m_log_enable;
};
void Emulator_Options::save_temps() {
    m_run_boot_rom = m_temp_run_boot_rom;
    m_strict_loading = m_temp_strict_loading;
    m_display_cart_info = m_temp_display_cart_info;
    m_log_length = m_temp_log_length;
    m_log_enable = m_temp_log_enable;
};


Emulator_Options* get_default_options() {
    return new Emulator_Options(true, false, false, 1000, true);
};


// Function Definitions
/*
    Reads the string like a white-space-delineated list and returns
    the next portion. Modifies the original string to remove the 
    portion it returns.
    For CSV reading, see eatNextCommaChunk.
*/
std::string eatNextChunk(std::string& to_parse) {
    int chunkStart = 0;
    int chunkEnd = 0;
    while (isspace(to_parse[chunkStart]) && chunkStart < static_cast<int>(to_parse.length())) {
        chunkStart += 1;
    }
    chunkEnd = chunkStart;
    while (!isspace(to_parse[chunkEnd]) && chunkEnd < static_cast<int>(to_parse.length())) {
        chunkEnd += 1;
    }
    std::string nextChunk = to_parse.substr(chunkStart, chunkEnd-chunkStart);
    to_parse = to_parse.substr(chunkEnd);
    return nextChunk;
};
/*
    Reads the string like a comma-delineated list and returns
    the next portion. Modifies the original string to remove the 
    portion it returns.
    For white-space-delineated, see eatNextChunk.
*/
std::string eatNextCommaChunk(std::string& to_parse) {
    int chunkStart = 0;
    int chunkEnd = 0;
    while (to_parse[chunkStart] == ',' && chunkStart < static_cast<int>(to_parse.length())) {
        chunkStart += 1;
    }
    chunkEnd = chunkStart;
    while (to_parse[chunkEnd] != ',' && chunkEnd < static_cast<int>(to_parse.length())) {
        chunkEnd += 1;
    }
    std::string nextChunk = to_parse.substr(chunkStart, chunkEnd-chunkStart);
    to_parse = to_parse.substr(chunkEnd);
    return nextChunk;
};

/*
    Performs std::toupper on each character in the string individually
*/
std::string to_upper(std::string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
};

/*
    Performs std::toupper on each character in the string individually
*/
std::string to_lower(std::string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
};

/*
    Pads left of string with given character until it is the length provided
*/
std::string padLeft(std::string str, int length, char pad_with) {
    if (static_cast<int>(str.size()) < length) {
        str.insert(0, length - str.size(), pad_with);
    }
    return str;
};
/*
    Pads left of string with spaces until it is the length provided
*/
std::string padLeft(std::string str, int length) {
    // Pads with spaces
    return padLeft(str, length, ' ');
};
/*
    Pads right of string with given character until it is the length provided
*/
std::string padRight(std::string str, int length, char pad_with) {
    if (static_cast<int>(str.size()) < length) {
        str.insert(str.size(), length - str.size(), pad_with);
    }
    return str;
};
/*
    Pads right of string with spaces until it is the length provided
*/
std::string padRight(std::string str, int length) {
    return padRight(str, length, ' ');
};
const std::string digits = "0123456789ABCDEF";
/*
    Converts the given number into a string in the given base
*/
std::string to_string_base(int num, int base) {
    std::string result;
 
    while(num > 0) {
        result = digits[num % base] + result;
        num /= base;
    }
    
    return result;
};


std::string to_b8_out(uint8_t num) {
    return padLeft(to_string_base(num, 16), 2, '0');
};
std::string to_b16_out(uint16_t num) {
    return padLeft(to_string_base(num, 16), 4, '0');
};

std::string trim_ws(std::string str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), str.end());
    return str;
};

bool is_int(const std::string & s) {
   if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
};
