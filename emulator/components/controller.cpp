
#include "controller.h"

#include "../../utils.h"
#include "interrupts.h"

Controller_Handler::Controller_Handler():
    m_ff00{ 0 },
    m_keybindings{ nullptr },
    m_interrupts{ nullptr }
{
    Reset();
};

void Controller_Handler::Initialize(std::vector<uint32_t>* keybindings, Interrupt_Handler* interrupts) {
    m_keybindings = keybindings;
    m_interrupts = interrupts;
};
void Controller_Handler::PowerCycle() {
    Reset();
};
void Controller_Handler::Reset() {
    m_ff00 = 0;
    for (int i = 0; i < 8; i++) {
        m_button_states[i] = false;
        m_old_button_states[i] = false;
    }
};

void Controller_Handler::requestInterrupt() {
    m_interrupts->requestInterrupt(JOYPAD_INTERRUPT);
};

void Controller_Handler::triggerEvent(uint32_t key, bool state) {
    for (int i = 0; i < 8; i++) {
        if (key == m_keybindings->at(i)) {
            m_button_states[i] = state;
            if (state == true) {
                requestInterrupt();
            }
            break; // If there is a duplicate key, this will make only one trigger
        }
    }
};

void Controller_Handler::write_FF00(uint8_t value) {
    m_ff00 = (value & 0b0011'0000);
};
uint8_t Controller_Handler::read_FF00() {
    uint8_t ret = (m_ff00 & 0b0011'0000) | 0b1100'1111; // 0 is pressed, 1 is released
    if (((m_ff00 >> 4) & 1) == 0) { // Select D-Pad
        if (m_button_states[ButtonRight]) { ret &= ~(1 << 0); }
        if (m_button_states[ButtonLeft]) { ret &= ~(1 << 1); }
        if (m_button_states[ButtonUp]) { ret &= ~(1 << 2); }
        if (m_button_states[ButtonDown]) { ret &= ~(1 << 3); }
    }
    if (((m_ff00 >> 5) & 1) == 0) { // Select Buttons
        if (m_button_states[ButtonA]) { ret &= ~(1 << 0); }
        if (m_button_states[ButtonB]) { ret &= ~(1 << 1); }
        if (m_button_states[ButtonSelect]) { ret &= ~(1 << 2); }
        if (m_button_states[ButtonStart]) { ret &= ~(1 << 3); }
    }
    return ret;
};

bool Controller_Handler::anyButtonPressed() {
    for (int i = 0; i < 8; i++) {
        if (m_button_states[i]) {
            return true;
        }
    }
    return false;
};
