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

        void println(std::string value);

        std::deque<std::string>* getLogfile();

    private:
        std::deque<std::string> m_logfile;

        int m_log_length;

        void capLength();
};

#endif