
#include "emulator_log.h"
#include "../../enable_logging.h"

Emulator_Logfile::Emulator_Logfile():
    m_logdump{ std::deque<std::string>() },
    m_log_length{ 1000 }
{

};

void Emulator_Logfile::Initialize(int log_length, bool log_enable) {
    m_log_length = log_length;
    if (!log_enable) {
        Reset();
    }
};
void Emulator_Logfile::PowerCycle() {
    Reset();
};
void Emulator_Logfile::Reset() {
    m_logdump.clear();
};

// void Emulator_Logfile::clearLog() {
//     m_logdump.clear();
// };
// void Emulator_Logfile::setOptions(int log_length, bool log_enable) {
//     m_log_length = log_length;
//     if (!log_enable) {
//         clearLog();
//     }
// };


void Emulator_Logfile::print(std::string value) {
    m_log_line += value;
};
void Emulator_Logfile::println(std::string value) {
    print(value);
    dumpLine();
};
void Emulator_Logfile::dumpLine() {
    m_logdump.push_front(m_log_line);
    m_log_line = "";
    capLength();
};

std::deque<std::string>* Emulator_Logfile::getLogfile() {
    return &m_logdump;
};

void Emulator_Logfile::capLength() {
    while (static_cast<int>(m_logdump.size()) > m_log_length) {
        m_logdump.pop_back();
    }
};


Emulator_Logfile m_logfile = Emulator_Logfile();
bool m_log_enable = true;
