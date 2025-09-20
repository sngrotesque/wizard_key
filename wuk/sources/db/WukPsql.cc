#include <db/WukPsql.hh>

static inline void free_conn(PGconn *conn) noexcept
{
    PQfinish(conn);
}

static inline void free_res(PGresult *res) noexcept
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

static inline std::string get_error_message(const PGconn *conn) noexcept
{
    std::string result(PQerrorMessage(conn));
    return result;
}

static inline std::string get_error_message(const PGresult *res) noexcept
{
    std::string result(PQresultErrorMessage(res));
    return result;
}

static inline wuk::i32 get_number_rows(const PGresult *res) noexcept
{
    return PQntuples(res);
}

static inline wuk::i32 get_number_cols(const PGresult *res) noexcept
{
    return PQnfields(res);
}

static inline bool is_result_success(ExecStatusType status) noexcept
{
    return  (status == PGRES_COMMAND_OK) ||
            (status == PGRES_TUPLES_OK)  ||
            (status == PGRES_COPY_OUT)   ||
            (status == PGRES_COPY_IN);
}

namespace wuk::db::psql {
    Connection::Connection(PGconn *conn) noexcept
    {
        this->m_conn = conn;
    }

    Connection::Connection(const std::string &conninfo)
    {
        this->connect(conninfo);
    }

    Connection::~Connection()
    {
        this->disconnect();
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

    void Connection::connect(const std::string &conninfo)
    {
        if (this->m_conn) {
            this->disconnect();
        }
        this->m_conn = PQconnectdb(conninfo.c_str());

        auto status = get_status(this->m_conn);
        if (status != CONNECTION_OK) {
            std::string err_message = get_error_message(this->m_conn);
            this->disconnect();
            throw wuk::Exception(status, "wuk::db::psql::Connection::connect",
                err_message);
        }
    }

    void Connection::reconnect()
    {
        if (!this->m_conn) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::db::psql::Connection::reconnect",
                "No existing connection to reset.");
        }
        PQreset(this->m_conn);

        auto status = get_status(this->m_conn);
        if (status != CONNECTION_OK) {
            std::string err_message = get_error_message(this->m_conn);
            this->disconnect();
            throw wuk::Exception(status, "wuk::db::psql::Connection::reconnect",
                err_message);
        }
    }

    void Connection::disconnect() noexcept
    {
        free_conn(this->m_conn);
        this->m_conn = nullptr;
    }

    bool Connection::is_connected() const noexcept
    {
        return (this->m_conn) && (get_status(this->m_conn) == CONNECTION_OK);
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

    Result::Result(PGresult *result) noexcept
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

    bool Result::is_validity() noexcept
    {
        // 空结果检查
        if (!this->m_res) {
            return false;
        }

        // 检查结果状态
        auto status = get_status(this->m_res);
        if (!is_result_success(status)) {
            return false;
        }

        // 获取结果中的行列数
        this->n_rows = this->get_row_count();
        this->n_cols = this->get_col_count();

        // 行数为0代表无数据
        if (this->n_rows == 0) {
            return false;
        }

        return true;
    }

    wuk::i32 Result::get_row_count() const noexcept
    {
        return get_number_rows(this->m_res);
    }

    wuk::i32 Result::get_col_count() const noexcept
    {
        return get_number_cols(this->m_res);
    }

    std::string Result::get_value(wuk::i32 row, wuk::i32 col) noexcept
    {
        // 有效性检查同时赋值行列数
        if (!this->is_validity()) {
            return {};
        }

        // 越界检查
        bool invalid_row = (row < 0) || (row >= this->n_rows);
        bool invalid_col = (col < 0) || (col >= this->n_cols);
        if (invalid_row || invalid_col) {
            return {};
        }
        // NULL值检查
        if (PQgetisnull(this->m_res, row, col) == 1) {
            return {};
        }

        // 正式开始获取数据
        const char *data = PQgetvalue(this->m_res, row, col);
        wuk::i32 size = PQgetlength(this->m_res, row, col);
        return std::string(data, size);
    }

    std::vector<std::string> Result::operator[](const wuk::i32 row) noexcept
    {
        // 有效性检查同时赋值行列数
        if (!this->is_validity()) {
            return {};
        }

        // 越界检查
        bool invalid_row = (row < 0) || (row >= this->n_rows);
        if (invalid_row) {
            return {};
        }

        // 存入数据
        std::vector<std::string> result(this->n_cols);

        for (wuk::i32 col = 0; col < this->n_cols; ++col) {
            result[col] = this->get_value(row, col);
        }

        return result;
    }

    //////////////////////////////////////////////////////////

    Work::Work(Connection &&conn) noexcept
    {
        this->m_conn = std::move(conn);
    }

    Work::~Work()
    {
        this->m_conn.disconnect();
    }

    Result Work::exec(const char *sql)
    {
        if (!this->m_conn.is_connected()) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::db::psql::Work::exec",
                "No active database connection.");
        }
        PGresult *exec_res = PQexec(this->m_conn.get_conn(), sql);

        auto status = get_status(exec_res);
        if (!is_result_success(status)) {
            std::string err_message = get_error_message(exec_res);
            free_res(exec_res);
            throw wuk::Exception(status, "wuk::db::psql::Work::exec",
                err_message);
        }

        return Result(exec_res);
    }

    Result Work::exec(const char *sql, std::vector<Param> params, ResultFormat f)
    {
        if (!this->m_conn.is_connected()) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::db::psql::Work::exec",
                "No active database connection.");
        }
        std::vector<const char *> param_values;
        std::vector<wuk::i32> param_lengths;
        std::vector<wuk::i32> param_formats;

        for (const auto &item : params) {
            param_values.push_back(item.data.c_str());
            param_lengths.push_back(item.data.size());
            param_formats.push_back(item.is_binary);
        }

        PGresult *exec_res = PQexecParams(
            this->m_conn.get_conn(),
            sql,
            params.size(),
            nullptr,
            param_values.data(),
            param_lengths.data(),
            param_formats.data(),
            static_cast<wuk::i32>(f)
        );

        auto status = get_status(exec_res);
        if (!is_result_success(status)) {
            std::string err_message = get_error_message(exec_res);
            free_res(exec_res);
            throw wuk::Exception(status, "wuk::db::psql::Work::exec",
                err_message);
        }

        return Result(exec_res);
    }
}


