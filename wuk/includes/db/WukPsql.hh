/**
 * @file WukPsql.hh
 * @author sngrotesque
 * @brief libpq库的C++实现（非完全通用）
 * 
 * 为什么不用libpqxx？
 * 
 * 官网：https://pqxx.org/libpqxx/
 * 文档写的跟屎一样，完全不理解哪些内容应该跟哪些代码组合在一起，真的是写的莫名其妙，我一眼都不想多看。
 * 其次，我使用libpq只是为了与我的PostgreSQL服务器进行处理，不需要用到所有功能，
 * 本身libpq库就比较大了，还引入一个libpqxx那不是更大了？
 * 更何况它的文档写的非常糟糕，我不想引入什么莫名其妙的代码BUG。
 * 
 * 你要问我那哪种文档才算好文档？
 * 请看（我将给出具体的模块文档，但这只是示例）：
 * 1. 微软的Win32API：https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect
 * 2. Python的类说明：https://docs.python.org/3/library/socket.html#socket.socket.connect
 * 3. Man7的Linux文档：https://man7.org/linux/man-pages/man2/connect.2.html
 */
#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <libpq-fe.h>

#include <vector>

namespace wuk::db::psql {
// 类类型声明
    class LIBWUK_API Connection;
    class LIBWUK_API Result;
    class LIBWUK_API Work;

// 基本类型声明
    struct Param {
        std::string data;
        wuk::i32 is_binary;
    };

    enum class ExecType {
        WRITE, // 写入操作
        READ,  // 读取操作
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
        Connection() = default;
        Connection(PGconn *conn) noexcept;
        Connection(const std::string &conninfo);
        ~Connection();

    public:
        // 拷贝构造
        Connection(const Connection &other) = delete;
        // 移动构造
        Connection(Connection &&other) noexcept;
        // 拷贝赋值
        Connection &operator=(const Connection &other) = delete;
        // 移动赋值
        Connection &operator=(Connection &&other) noexcept;

    public:
        void connect(const std::string &conninfo);
        void reconnect(const std::string &conninfo = nullptr);
        void disconnect() noexcept;

    public:
        const PGconn *get_conn() const noexcept;
        PGconn *get_conn() noexcept;
    };

// Result
    class LIBWUK_API Result {
    private:
        PGresult *m_res = nullptr;
        wuk::i32 n_rows = 0; // 行数
        wuk::i32 n_cols = 0; // 列数

    public:
        Result() = default;
        Result(PGresult *result) noexcept;
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
        /* 有效性检查，同时初始化行列数
         * 后续可以考虑在此方法中添加一个对于 m_res 是否有错误（get_status）的判断，但这
         * 需要Work类的配合，否则无法实现。
         */
        bool is_validity() noexcept;
        wuk::i32 get_row_count() const noexcept;
        wuk::i32 get_col_count() const noexcept;
        std::string get_value(wuk::i32 row, wuk::i32 col) noexcept;
        std::vector<std::string> operator[](const wuk::i32 row) noexcept;
    };

// Work
    class LIBWUK_API Work {
    private:
        Connection m_conn;

    public:
        Work() = default;
        Work(Connection &&conn) noexcept;
        ~Work();

        /* 如果执行的是例如 INSERT 这种操作，那么Result内部不会有任何值。调用者应该明白这一点。
         * 我不想去做像是 libpqxx 库那样的区分有数据和无数据的类。
         * 当然后期也可以添加一个模板来让用户指定此操作是否会有返回数据。
         */
        Result exec(const char *sql);
        Result exec(const char *sql, std::vector<Param> params, ResultFormat f);

        
    };
}
#endif
