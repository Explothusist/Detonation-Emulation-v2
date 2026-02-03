#ifndef EMULATOR_TIMER_HANDLER_
#define EMULATOR_TIMER_HANDLER_

#include <cstdint>
#include <vector>
#include <string>

class Interrupt_Handler;

class Timer_Handler {
    public:
        Timer_Handler();

        void Initialize(Interrupt_Handler* interrupts);
        void PowerCycle();
        void Reset();

        void runMCycle();
        void runTCycle();

        void requestInterrupt();

        void resetDiv(); // Happens on STOP
        void incTIMA();
        int checkTimaDivBit();

        void write_FF04(uint8_t value);
        uint8_t read_FF04();
        void write_FF05(uint8_t value);
        uint8_t read_FF05();
        void write_FF06(uint8_t value);
        uint8_t read_FF06();
        void write_FF07(uint8_t value);
        uint8_t read_FF07();

    private:
        // uint8_t m_ff04;
        uint16_t m_internal_counter;
        uint8_t m_ff05_tima;
        uint8_t m_ff06_t_mod;
        uint8_t m_ff07_ctrl;

        bool m_tima_enable;
        int m_tima_shift;

        bool m_tima_overflow;
        int m_tima_overflow_delay;
        // bool m_interrupt;

        Interrupt_Handler* m_interrupts;
};

#endif