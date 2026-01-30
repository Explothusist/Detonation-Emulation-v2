
#include "emulator_log.h"
#include "../enable_logging.h"

Emulator_Logfile::Emulator_Logfile():
    m_logfile{ std::deque<std::string>() },
    m_log_length{ 1000 }
{

};

void Emulator_Logfile::clearLog() {
    m_logfile.clear();
};
void Emulator_Logfile::setOptions(int log_length, bool log_enable) {
    m_log_length = log_length;
    if (!log_enable) {
        clearLog();
    }
};

void Emulator_Logfile::println(std::string value) {
    m_logfile.push_front(value);
    capLength();
};

std::deque<std::string>* Emulator_Logfile::getLogfile() {
    return &m_logfile;
};

void Emulator_Logfile::capLength() {
    while (static_cast<int>(m_logfile.size()) > m_log_length) {
        m_logfile.pop_back();
    }
};
