#ifndef EMULATOR_INTERRUPT_HANDLER_
#define EMULATOR_INTERRUPT_HANDLER_

#include <cstdint>
#include <vector>
#include <string>
#include "../../utils.h"

class DMG_CPU;

class Interrupt_Handler {
    public:
        Interrupt_Handler();

        void Initialize();
        void PowerCycle();
        void Reset();

        // bool pollInterrupts();
        void requestInterrupt(Interrupt_Type type);
        void runMCycle(DMG_CPU &m_cpu);
        void runTCycle();
        void handleInterrupts(DMG_CPU &m_cpu);

        void write_FF0F(uint8_t value);
        uint8_t read_FF0F();
        void write_FFFF(uint8_t value);
        uint8_t read_FFFF();
        void write_IME(bool value);
        // bool read_IME();

        bool anyInterruptRequested();
        bool hasInterruptPending();
        bool interruptsEnabled(); // read_IME

    private:
        // uint8_t m_ff0f;
        uint8_t m_ffff;
        bool m_ime;

        bool m_vblank_enable;
        bool m_lcd_enable;
        bool m_timer_enable;
        bool m_serial_enable;
        bool m_joypad_enable;
        
        bool m_vblank_request;
        bool m_lcd_request;
        bool m_timer_request;
        bool m_serial_request;
        bool m_joypad_request;
};

#endif