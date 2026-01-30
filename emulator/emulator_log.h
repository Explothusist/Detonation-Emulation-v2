#ifndef EMULATOR_LOGFILE_
#define EMULATOR_LOGFILE_

// #include <vector>
#include <deque>
#include <string>
#include "../utils.h"

class Emulator_Logfile {
    public:
        Emulator_Logfile();

        void clearLog();
        void setOptions(int log_length, bool log_enable);

        void print(std::string);
        void println(std::string value);
        void dumpLine();

        std::deque<std::string>* getLogfile();

    private:
        std::deque<std::string> m_logdump;

        int m_log_length;
        std::string m_log_line;

        void capLength();
};

extern Emulator_Logfile m_logfile;
extern bool m_log_enable;

#endif