#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <libpq-fe.h>

#include <vector>

namespace wuk::im::psql {
// 类类型声明
    class LIBWUK_API Connection;
    class LIBWUK_API Result;
    class LIBWUK_API Work;

// 基本类型声明
    struct Param {
        std::string data;
        wuk::i32 is_binary;
    };

    enum class ResultFormat : wuk::i32 {
        TEXT   = 0,
        BINARY = 1,
    };

// Connection
    class LIBWUK_API Connection {
    protected:
        PGconn *m_conn = nullptr;

    public:
        friend class Result;

        Connection() = default;
        Connection(PGconn *conn);
        Connection(const char *conninfo);
        ~Connection();

    public:
        // 拷贝构造
        Connection(const Connection &other) = default;
        // 移动构造
        Connection(Connection &&other) noexcept;
        // 拷贝赋值
        Connection &operator=(const Connection &other) = default;
        // 移动赋值
        Connection &operator=(Connection &&other) noexcept;

    public:
        void connect(const char *conninfo);
        void reconnect(const char *conninfo = nullptr);
        void disconnect() noexcept;

    public:
        const PGconn *get_conn() const noexcept;
        PGconn *get_conn() noexcept;
    };

// Result
    class LIBWUK_API Result {
    private:
        PGresult *m_res = nullptr;

    public:
        Result() = default;
        Result(PGresult *result);
        ~Result();

    public:
        // 拷贝构造
        Result(const Result &other) noexcept;
        // 移动构造
        Result(Result &&other) noexcept;
        // 拷贝赋值
        Result &operator=(const Result &other) noexcept;
        // 移动赋值
        Result &operator=(Result &&other) noexcept;

    public:
        std::string operator[](const wuk::i32 index);
    };

// Work
    class LIBWUK_API Work {
    private:
        Connection m_conn;

    public:
        Work() = default;
        Work(Connection &&conn) noexcept;

        Result exec(const char *sql);
        Result exec(const char *sql, std::vector<Param> params, ResultFormat fmt);
    };
}
#endif
