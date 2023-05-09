// Copyright (C) 2022 Valentin-Ioan VINTILA.
// All rights reserved.

#include "../include/wi-utilities.hpp"

namespace wi {
    int32_t Log::m_logCounter = 0;
    Log::Log(
        const string filename,
        const bool appendDateTime,
        const uint32_t logAmmount
    ) : m_enabled(false),
        m_logAmmount(logAmmount),
        m_lastLvl(0) {
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        m_filename = filename;
        if (appendDateTime) {
            m_filename.push_back('-');
            m_filename += std::to_string(now->tm_year + 1900);
            m_filename.push_back('-');
            if (now->tm_mon <= 8)
                m_filename.push_back('0');
            m_filename += std::to_string(now->tm_mon + 1);
            m_filename.push_back('-');
            if (now->tm_mday <= 9)
                m_filename.push_back('0');
            m_filename += std::to_string(now->tm_mday);
            m_filename.push_back('-');
            if (now->tm_hour <= 9)
                m_filename.push_back('0');
            m_filename += std::to_string(now->tm_hour);
            m_filename.push_back('-');
            if (now->tm_min <= 9)
                m_filename.push_back('0');
            m_filename += std::to_string(now->tm_min);
        }
        m_filename.push_back('-');
        m_filename += std::to_string(++m_logCounter);
    }

    Log::~Log() {
        if (m_enabled)
            this->stop(true);
    }

    void Log::start(const bool openMessage) {
        if (m_enabled)
            return;
        m_fout.open(m_filename);
        if (!m_fout)
            throw std::runtime_error(string("Could not open for writing - file '") + m_filename + string("'."));
        m_enabled = true;
        if (openMessage) {
            std::time_t t = std::time(0);
            std::tm* now = std::localtime(&t);
            m_fout << "Log started at " <<
                std::setw(2) << std::setfill('0') << now->tm_hour << ":" <<
                std::setw(2) << std::setfill('0') << now->tm_min << ":" <<
                std::setw(2) << std::setfill('0') << now->tm_sec <<
                "\n= = = = =\n";
        }
    }

    void Log::restart(const bool openMessage) {
        if (m_enabled)
            this->stop(true);
        m_fout.open(m_filename, std::ios_base::app);
        if (!m_fout)
            throw std::runtime_error(string("Could not open for appending - file '") + m_filename + string("'."));
        m_enabled = true;
        if (openMessage) {
            std::time_t t = std::time(0);
            std::tm* now = std::localtime(&t);
            m_fout << "Log restarted at " <<
                std::setw(2) << std::setfill('0') << now->tm_hour << ":" <<
                std::setw(2) << std::setfill('0') << now->tm_min << ":" <<
                std::setw(2) << std::setfill('0') << now->tm_sec <<
                "\n= = = = =\n";
        }
    }

    void Log::stop(const bool openMessage) {
        if (!m_enabled)
            return;
        if (openMessage) {
            std::time_t t = std::time(0);
            std::tm* now = std::localtime(&t);
            m_fout << "\n= = = = =\nLog stopped at " <<
                std::setw(2) << std::setfill('0') << now->tm_hour << ":" <<
                std::setw(2) << std::setfill('0') << now->tm_min << ":" <<
                std::setw(2) << std::setfill('0') << now->tm_sec << ":" << std::endl;
        }
        m_fout.close();
        m_enabled = false;
    }

    void Log::print(
        string msg,
        string type,
        const uint32_t lvl
    ) {
        if (!m_enabled)
            return;
        if (m_lastLvl < lvl) {
            for (uint32_t i = 1; i <= m_lastLvl; ++i)
                m_fout << ":  ";
            for (uint32_t i = m_lastLvl + 1; i < lvl; ++i)
                m_fout << "|>>";
            m_fout << "|> ";
        } else if (m_lastLvl == lvl) {
            for (uint32_t i = 1; i < m_lastLvl; ++i)
                m_fout << ":  ";
            if (lvl > 0)
                m_fout << "|> ";
        } else { // m_lastLvl > lvl
            for (uint32_t i = 1; i < lvl; ++i)
                m_fout << ":  ";
            if (lvl > 0) {
                m_fout << "|" << std::endl;
                for (uint32_t i = 1; i < lvl; ++i)
                    m_fout << ":  ";
                m_fout << "|> ";
            }
            else
                m_fout << std::endl;
        }
        if (type.size())
            m_fout << "[ " << type << " ] ";
        m_fout << msg << std::endl;
        m_lastLvl = lvl;
    }

    void Log::printText(
        string msg,
        const uint32_t lvl
    ) {
        this->print(msg, "", lvl);
    }

    void Log::printInfo(
        string msg,
        const uint32_t lvl
    ) {
        if ((m_logAmmount & WI_LOG_AMMOUNT_INFO) == WI_LOG_AMMOUNT_INFO)
            this->print(msg, "info", lvl);
    }

    void Log::printWarn(
        string msg,
        const uint32_t lvl
    ) {
        if ((m_logAmmount & WI_LOG_AMMOUNT_WARN) == WI_LOG_AMMOUNT_WARN)
            this->print(msg, "warn", lvl);
    }

    void Log::printError(
        string msg,
        const uint32_t lvl
    ) {
        if ((m_logAmmount & WI_LOG_AMMOUNT_ERROR) == WI_LOG_AMMOUNT_ERROR)
            this->print(msg, "error", lvl);
    }

    string uc_to_hex_string(
        unsigned char *uc,
        const uint32_t ucLength_bytes,
        const char shortDelim,
        const string longDelim,
        const uint32_t blockSize_bytes,
        const bool useCapital
    ) {
        string result = "";
        const char begLetter = useCapital ? 'A' : 'a';
        for (uint32_t i = 0; i < ucLength_bytes; ++i) {
            if (i) {
                if (blockSize_bytes && longDelim.size() && i % blockSize_bytes == 0)
                    result += longDelim;
                else if (shortDelim)
                    result.push_back(shortDelim);
            }

            const unsigned char lb = uc[i] >> 4;
            if (lb <= 9)
                result.push_back('0' + lb);
            else result.push_back(begLetter + lb - 10);

            const unsigned char rb = uc[i] & 0x0f;
            if (rb <= 9)
                result.push_back('0' + rb);
            else result.push_back(begLetter + rb - 10);
        }
        return result;
    }

    string to_hex_string(
        uint32_t number,
        uint32_t minDigits,
        const bool useCapital
    ) {
        string result = "";
        const char begLetter = useCapital ? 'A' : 'a';

        bool mustShow = false;
        for (
            uint32_t check = 0xf0000000, toKill = 7;
            check;
            check >>= 4, toKill--
        ) {
            uint32_t part = ((number & check) >> (toKill << 2));
            if (minDigits > toKill || part)
                mustShow = true;
            if (mustShow) {
                if (part <= 9)
                    result.push_back('0' + part);
                else result.push_back(begLetter + part - 10);
            }
        }
        if (!result.size())
            result.push_back('0');
        return result;
    }

    void *memxorcpy(
        void *destination,
        const void *source,
        const uint32_t num
    ) {
        uint32_t num4 = (num >> 2);
        for (uint32_t i = 0; i < num4; ++i)
            ((uint32_t *)destination)[i] ^= ((const uint32_t *)source)[i];
        for (uint32_t i = (num4 << 2); i < num; ++i)
            ((uint8_t *)destination)[i] ^= ((const uint8_t *)source)[i];
        return destination;
    }
}
