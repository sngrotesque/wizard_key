/**
 * @file WukPsql.cc
 * @author sngrotesque
 * 
 * 注意在释放时，应先释放掉PGresult指针再释放PGconn，否则可能会出现问题。
 */
#include <im/WukPsql.hh>

static inline void free_conn(PGconn *conn)
{
    PQfinish(conn);
}

static inline void free_res(PGresult *res)
{
    PQclear(res);
}

static inline ConnStatusType get_status(const PGconn *conn) noexcept
{
    return PQstatus(conn);
}

static inline ExecStatusType get_status(const PGresult *res) noexcept
{
    return PQresultStatus(res);
}

static inline std::string get_error_message(const PGconn *conn)
{
    std::string result(PQerrorMessage(conn));
    return result;
}

static inline std::string get_error_message(const PGresult *res)
{
    std::string result(PQresultErrorMessage(res));
    return result;
}

static PGresult *exec_param(
    PGconn *conn,
    const char *query,
    std::vector<wuk::im::psql::Param> params,
    wuk::im::psql::ResultFormat res_fmt
)
{
    std::vector<const char *> param_values;
    std::vector<wuk::i32> param_lengths;
    std::vector<wuk::i32> param_formats;

    for (const auto &item : params) {
        param_values.push_back(item.data.c_str());
        param_lengths.push_back(item.data.size());
    }

    PGresult *result = PQexecParams(
        conn,
        query,
        params.size(),
        nullptr,
        param_values.data(),
        param_lengths.data(),
        param_formats.data(),
        static_cast<wuk::i32>(res_fmt)
    );

    return result;
}

static PGresult *exec_param(
    PGconn *conn,
    const char *query
)
{
    return PQexec(conn, query);
}

namespace wuk::im::psql {
    Connection::Connection(PGconn *conn)
    {
        this->m_conn = conn;
    }

    Connection::Connection(const char *conninfo)
    {
        this->connect(conninfo);
    }

    Connection::~Connection()
    {
        free_conn(this->m_conn);
    }

    Connection::Connection(Connection &&other) noexcept
    {
        if (this == &other) {
            return;
        }
        this->m_conn = other.m_conn;
        other.m_conn = nullptr;
    }

    Connection &Connection::operator=(Connection &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        free_conn(this->m_conn);

        this->m_conn = other.m_conn;
        other.m_conn = nullptr;

        return *this;
    }

    void Connection::connect(const char *conninfo)
    {
        this->m_conn = PQconnectdb(conninfo);

        wuk::i32 err_code = get_status(this->m_conn);
        if (err_code != CONNECTION_OK) {
            std::string err_message = get_error_message(this->m_conn);
            free_conn(this->m_conn);
            throw wuk::Exception(err_code, "wuk::im::psql::Connection::connect",
                err_message);
        }
    }

    void Connection::reconnect(const char *conninfo)
    {
        if (conninfo == nullptr) {
            PQreset(this->m_conn);
        } else {
            this->connect(conninfo);
        }
    }

    void Connection::disconnect() noexcept
    {
        free_conn(this->m_conn);
    }

    const PGconn *Connection::get_conn() const noexcept
    {
        return this->m_conn;
    }

    PGconn *Connection::get_conn() noexcept
    {
        return this->m_conn;
    }

    //////////////////////////////////////////////////////////

    Result::Result(PGresult *result)
    {
        this->m_res = result;
    }

    Result::~Result()
    {
        free_res(this->m_res);
    }

    Result::Result(const Result &other) noexcept
    {
        // 注意PQcopyResult接受nullptr并直接返回nullptr。
        this->m_res = PQcopyResult(
            other.m_res,
            PG_COPYRES_TUPLES | PG_COPYRES_EVENTS
        );
    }

    Result::Result(Result &&other) noexcept
    {
        free_res(this->m_res);
        this->m_res = other.m_res;
        other.m_res = nullptr;
    }

    Result &Result::operator=(const Result &other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        free_res(this->m_res);
        this->m_res = PQcopyResult(
            other.m_res,
            PG_COPYRES_TUPLES | PG_COPYRES_EVENTS
        );
        return *this;
    }

    Result &Result::operator=(Result &&other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        free_res(this->m_res);
        this->m_res = other.m_res;
        other.m_res = nullptr;
        return *this;
    }

    //////////////////////////////////////////////////////////

    Work::Work(Connection &&conn) noexcept
    {
        this->m_conn = conn;
    }

    Result Work::exec(const char *sql)
    {
        Result res = exec_param(this->m_conn.get_conn(), sql);

        return res;
    }

    Result Work::exec(const char *sql, std::vector<Param> params, ResultFormat fmt)
    {
        Result res;

        return res;
    }
}
