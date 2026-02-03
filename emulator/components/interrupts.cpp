
#include "interrupts.h"
#include "../cpu.h"
#include "../opcodes.h"

Interrupt_Handler::Interrupt_Handler():
    // m_ff0f{ 0 },
    m_ffff{ 0 },
    m_ime{ false },
    m_vblank_enable{ false },
    m_lcd_enable{ false },
    m_timer_enable{ false },
    m_serial_enable{ false },
    m_joypad_enable{ false },
    m_vblank_request{ false },
    m_lcd_request{ false },
    m_timer_request{ false },
    m_serial_request{ false },
    m_joypad_request{ false }
{

};

void Interrupt_Handler::Initialize() {

};
void Interrupt_Handler::PowerCycle() {
    Reset();
};
void Interrupt_Handler::Reset() {
    // m_ff0f = 0;
    m_ffff = 0;
    m_ime = false;
    m_vblank_enable = false;
    m_lcd_enable = false;
    m_timer_enable = false;
    m_serial_enable = false;
    m_joypad_enable = false;
    m_vblank_request = false;
    m_lcd_request = false;
    m_timer_request = false;
    m_serial_request = false;
    m_joypad_request = false;
};


// void Interrupt_Handler::pollInterrupts() {

// };
void Interrupt_Handler::requestInterrupt(Interrupt_Type type) {
    switch (type) {
        case VBLANK_INTERRUPT:
            m_vblank_request = true;
            break;
        case LCD_INTERRUPT:
            m_lcd_request = true;
            break;
        case TIMER_INTERRUPT:
            m_timer_request = true;
            break;
        case SERIAL_INTERRUPT:
            m_serial_request = true;
            break;
        case JOYPAD_INTERRUPT:
            m_joypad_request = true;
            break;
    }
};
void Interrupt_Handler::runMCycle(DMG_CPU &m_cpu) {
    // if (m_cpu.m_ppu.pollVBlankInterrupt()) { // No constant polling
    //     m_vblank_request = true;
    // }
    // if (m_cpu.m_ppu.pollLCDInterrupt()) {
    //     m_lcd_request = true;
    // }
    // if (m_cpu.m_Memory.m_timer.pollInterrupt()) {
    //     m_timer_request = true;
    // }
    // if (m_cpu.m_Memory.m_serial.pollInterrupt()) {
    //     m_serial_request = true;
    // }
    // if (m_cpu.m_Memory.m_controllers.pollInterrupt()) {
    //     m_joypad_request = true;
    // }

    if (m_cpu.isBetweenOpcodes()) {
        handleInterrupts(m_cpu);
    }
};
void Interrupt_Handler::runTCycle() {};
void Interrupt_Handler::handleInterrupts(DMG_CPU &m_cpu) {
    if (m_ime) {
        if (m_vblank_request && m_vblank_enable) {
            m_cpu.fireInterrupt(&Interrupt_VBLANK_RST_40);
            m_vblank_request = false;
        }else if (m_lcd_request && m_lcd_enable) {
            m_cpu.fireInterrupt(&Interrupt_LCD_RST_48);
            m_lcd_request = false;
        }else if (m_timer_request && m_timer_enable) {
            m_cpu.fireInterrupt(&Interrupt_TIMER_RST_50);
            m_timer_request = false;
        }else if (m_serial_request && m_serial_enable) {
            m_cpu.fireInterrupt(&Interrupt_SERIAL_RST_58);
            m_serial_request = false;
        }else if (m_joypad_request && m_joypad_enable) {
            m_cpu.fireInterrupt(&Interrupt_JOYPAD_RST_60);
            m_joypad_request = false;
        }
    }
};


void Interrupt_Handler::write_FF0F(uint8_t value) {
    // m_ff0f = (value & 0b0001'1111);
    m_vblank_request = (((value >> 0) & 1) == 1); // Can manual request or cancel interrupts
    m_lcd_request = (((value >> 1) & 1) == 1);
    m_timer_request = (((value >> 2) & 1) == 1);
    m_serial_request = (((value >> 3) & 1) == 1);
    m_joypad_request = (((value >> 4) & 1) == 1);
};
uint8_t Interrupt_Handler::read_FF0F() {
    // uint8_t ret = ((m_ff0f | 0b1110'0000) & 0b1110'0000);
    uint8_t ret = 0b1110'0000;
    if (m_vblank_request) {
        ret |= (1 << 0);
    }
    if (m_lcd_request) {
        ret |= (1 << 1);
    }
    if (m_timer_request) {
        ret |= (1 << 2);
    }
    if (m_serial_request) {
        ret |= (1 << 3);
    }
    if (m_joypad_request) {
        ret |= (1 << 4);
    }
    return ret;
};
void Interrupt_Handler::write_FFFF(uint8_t value) {
    m_ffff = (value & 0b0001'1111);
    m_vblank_enable = (((m_ffff >> 0) & 1) == 1);
    m_lcd_enable = (((m_ffff >> 1) & 1) == 1);
    m_timer_enable = (((m_ffff >> 2) & 1) == 1);
    m_serial_enable = (((m_ffff >> 3) & 1) == 1);
    m_joypad_enable = (((m_ffff >> 4) & 1) == 1);
};
uint8_t Interrupt_Handler::read_FFFF() {
    return (m_ffff | 0b1110'0000);
};
void Interrupt_Handler::write_IME(bool value) {
    m_ime = value;
};
// bool Interrupt_Handler::read_IME() {
//     return m_ime;
// };

bool Interrupt_Handler::anyInterruptRequested() {
    return (read_FF0F() & 0b1110'0000) != 0;
};
bool Interrupt_Handler::hasInterruptPending() {
    return (read_FF0F() & read_FFFF() & 0b1110'0000) != 0;
};
bool Interrupt_Handler::interruptsEnabled() {
    return m_ime;
};

