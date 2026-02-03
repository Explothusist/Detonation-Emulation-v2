#ifndef EMULATOR_SERIAL_HANDLER_
#define EMULATOR_SERIAL_HANDLER_

#include <cstdint>
#include <vector>
#include <string>

class Interrupt_Handler;

class Serial_Handler {
    public:
        Serial_Handler();

        void Initialize(Interrupt_Handler* interrupts);
        void PowerCycle();
        void Reset();

        void requestInterrupt();

        void write_FF01(uint8_t value);
        uint8_t read_FF01();
        void write_FF02(uint8_t value);
        uint8_t read_FF02();

    private:
        uint8_t m_ff01;
        uint8_t m_ff02;

        std::string m_debug_out; // Optional Serial as debug out (Blaarg's, etc.)

        Interrupt_Handler* m_interrupts;
};

#endif