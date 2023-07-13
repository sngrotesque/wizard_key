// Copyright (C) 2022 Valentin-Ioan VINTILA.
// All rights reserved.

#ifndef WI_UTILITIES_HPP
#define WI_UTILITIES_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdio>

using std::string;

#define WI_LOG_AMMOUNT_INFO  0b001
#define WI_LOG_AMMOUNT_WARN  0b010
#define WI_LOG_AMMOUNT_ERROR 0b100
#define WI_LOG_AMMOUNT_ALL   (WI_LOG_AMMOUNT_INFO | \
                              WI_LOG_AMMOUNT_WARN | \
                              WI_LOG_AMMOUNT_ERROR)

namespace wi {
    class Log {
    private:
        static int32_t m_logCounter;
        string m_filename;
        bool m_enabled;
        uint32_t m_logAmmount;
        uint32_t m_lastLvl;

        std::ofstream m_fout;

    public:
        Log(
            string filename = "logs/log",
            const bool appendDateTime = true,
            const uint32_t logAmmount = WI_LOG_AMMOUNT_ALL
        );
        ~Log();

        void start(const bool openMessage = true);
        void restart(const bool openMessage = true);
        void stop(const bool closeMessage = true);

        void print(
            string msg,
            string type,
            const uint32_t lvl = 0
        );

        void printText(
            string msg,
            const uint32_t lvl = 0
        );

        void printInfo(
            string msg,
            const uint32_t lvl = 0
        );

        void printWarn(
            string msg,
            const uint32_t lvl = 0
        );

        void printError(
            string msg,
            const uint32_t lvl = 0
        );
    };

    string uc_to_hex_string(
        unsigned char *uc,
        const uint32_t ucLength_bytes,
        const char shortDelim = ' ',
        const string longDelim = "   ",
        const uint32_t blockSize_bytes = 4,
        const bool useCapital = true
    );

    string to_hex_string(
        uint32_t number,
        uint32_t minDigits = 0,
        const bool useCapital = true
    );

    void *memxorcpy(
        void *destination,
        const void *source,
        const uint32_t num
    );
}

#endif // WI_UTILITIES_HPP
