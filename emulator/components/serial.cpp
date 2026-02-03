
#include "serial.h"

#include "emulator_log.h"
#include "../../enable_logging.h"
#include "../../utils.h"
#include "interrupts.h"

Serial_Handler::Serial_Handler():
    m_ff01{ 0 },
    m_ff02{ 0 },
    m_debug_out{ "" },
    m_interrupts{ nullptr }
{

};

void Serial_Handler::Initialize(Interrupt_Handler* interrupts) {
    m_interrupts = interrupts;
};
void Serial_Handler::PowerCycle() {
    Reset();
};
void Serial_Handler::Reset() {
    m_ff01 = 0;
    m_ff02 = 0;
    m_debug_out = "";
};

void Serial_Handler::requestInterrupt() {
    m_interrupts->requestInterrupt(SERIAL_INTERRUPT);
};


void Serial_Handler::write_FF01(uint8_t value) {
    m_ff01 = value;
};
uint8_t Serial_Handler::read_FF01() {
    return m_ff01;
};
void Serial_Handler::write_FF02(uint8_t value) {
    m_ff02 = value & 0b1000'0001;
    if (((m_ff02 >> 7) & 1) == 1) { // Begin Serial Write
        m_ff02 &= 0b0111'1111;
        requestInterrupt();
        m_debug_out += char(m_ff01);
#ifdef DEBUG_LOGGING
        printf("Serial Write: '%s'", m_debug_out.c_str());
        if (m_log_enable) {
            m_logfile.print("Serial Write: '"+m_debug_out+"'\n");
        }
#endif
    }
};
uint8_t Serial_Handler::read_FF02() {
    return m_ff02 | 0b0111'1110;
};

