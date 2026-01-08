
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
