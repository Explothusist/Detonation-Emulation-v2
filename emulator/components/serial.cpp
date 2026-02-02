
#include "serial.h"

#include "emulator_log.h"
#include "../../enable_logging.h"

Serial_Handler::Serial_Handler():
    m_ff01{ 0 },
    m_ff02{ 0 }
{

};

void Serial_Handler::Initialize() {

};
void Serial_Handler::PowerCycle() {

};
void Serial_Handler::Reset() {
    m_ff01 = 0;
    m_ff02 = 0;
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
        m_interrupt = true;
        m_debug_out += char(m_ff01);
#ifdef DEBUG_LOGGING
        printf("Serial Write: '%s'", m_debug_out.c_str());
        if (m_log_enable) {
            m_logfile.print("Serial Write: '"+m_debug_out+"'");
        }
#endif
    }
};
uint8_t Serial_Handler::read_FF02() {
    return m_ff02 | 0b0111'1110;
};

