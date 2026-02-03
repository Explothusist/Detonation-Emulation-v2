#ifndef EMULATOR_CONTROLLER_HANDLER_
#define EMULATOR_CONTROLLER_HANDLER_

#include <cstdint>
#include <vector>

class Interrupt_Handler;

class Controller_Handler {
    public:
        Controller_Handler();

        void Initialize(std::vector<uint32_t>* keybindings, Interrupt_Handler* interrupts);
        void PowerCycle();
        void Reset();

        void requestInterrupt();

        void triggerEvent(uint32_t key, bool state);

        uint8_t read_FF00();
        void write_FF00(uint8_t value);

        bool anyButtonPressed();

    private:
        bool m_button_states[8];
        bool m_old_button_states[8];
        uint8_t m_ff00;
        std::vector<uint32_t>* m_keybindings; // uint32_t as in SDL_Event.key_thingy

        Interrupt_Handler* m_interrupts;
};

#endif