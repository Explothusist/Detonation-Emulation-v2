
#include "timer.h"

#include "emulator_log.h"
#include "../../enable_logging.h"
#include "interrupts.h"
#include "../../utils.h"

Timer_Handler::Timer_Handler():
    m_internal_counter{ 0 },
    m_ff05_tima{ 0 },
    m_ff06_t_mod{ 0 },
    m_ff07_ctrl{ 0 },
    m_tima_enable{ false },
    m_tima_shift{ 9 },
    m_tima_overflow{ false },
    m_tima_overflow_delay{ 0 },
    m_interrupts{ nullptr }
{

};

void Timer_Handler::Initialize(Interrupt_Handler* interrupts) {
    m_interrupts = interrupts;
};
void Timer_Handler::PowerCycle() {
    Reset();
};
void Timer_Handler::Reset() {
    m_internal_counter = 0;
    m_ff05_tima = 0;
    m_ff06_t_mod = 0;
    m_ff07_ctrl = 0;
    m_tima_shift = 9;
    m_tima_overflow = false;
    m_tima_overflow_delay = 0;
};

void Timer_Handler::runMCycle() {
    for (int i = 0; i < 4; i++) { // Run 4 T-Cycles
        runTCycle();
    }
};
void Timer_Handler::runTCycle() {
    int old_bit = checkTimaDivBit();
    m_internal_counter += 1;
    if (old_bit == 1 && checkTimaDivBit() == 0) { // Falling edge
        incTIMA();
    }
    if (m_tima_overflow && m_tima_overflow_delay > 0) {
        m_tima_overflow_delay -= 1;
        if (m_tima_overflow_delay == 0) {
            m_tima_overflow = false;
            requestInterrupt();
            m_ff05_tima = m_ff06_t_mod;
        }
    }
};

void Timer_Handler::requestInterrupt() {
    m_interrupts->requestInterrupt(TIMER_INTERRUPT);
};


void Timer_Handler::resetDiv() {
    int old_bit = checkTimaDivBit();
    m_internal_counter = 0;
    if (old_bit == 1) { // Falling edge
        incTIMA();
    }
};
void Timer_Handler::incTIMA() {
    if (m_ff05_tima == 0xff && !m_tima_overflow) {
        m_tima_overflow = true;
        m_tima_overflow_delay = 4;
    }
    m_ff05_tima += 1;
};
int Timer_Handler::checkTimaDivBit() {
    return ((m_internal_counter >> m_tima_shift) & (m_tima_enable ? 1 : 0));
};

void Timer_Handler::write_FF04(uint8_t value) { // DIV
    resetDiv();
};
uint8_t Timer_Handler::read_FF04() {
    return (m_internal_counter >> 8);
};
void Timer_Handler::write_FF05(uint8_t value) { // TIMA
    m_ff05_tima = value;
    m_tima_overflow = false;
    m_tima_overflow_delay = 0;
};
uint8_t Timer_Handler::read_FF05() {
    return m_ff05_tima;
};
void Timer_Handler::write_FF06(uint8_t value) { // Modulo
    m_ff06_t_mod = value;
};
uint8_t Timer_Handler::read_FF06() {
    return m_ff06_t_mod;
};
void Timer_Handler::write_FF07(uint8_t value) { // Control
    int old_bit = checkTimaDivBit();
    m_ff07_ctrl = value & 0b0000'0111;
    m_tima_enable = (((m_ff07_ctrl >> 2) & 1) == 1); // 1 is enable, 0 is disable
    switch (m_ff07_ctrl & 0b11) {
        case 0b01:
            m_tima_shift = 3;
            break;
        case 0b10:
            m_tima_shift = 5;
            break;
        case 0b11:
            m_tima_shift = 7;
            break;
        case 0b00:
            m_tima_shift = 9;
            break;
    }
    if (old_bit == 1 && checkTimaDivBit() == 0) {
        incTIMA();
    }
};
uint8_t Timer_Handler::read_FF07() {
    return m_ff07_ctrl | 0b1111'1000;
};

