#include <im/WukPsql.hh>

#define RETURN return *this

namespace wuk::im {
    PsqlConnInfo::PsqlConnInfo(const PsqlConnParams &params)
    {
        this->set_conninfo(params);
    }

    PsqlConnInfo &PsqlConnInfo::set_host(const std::string &value)
    {
        this->m_host = value; RETURN;
    }

    PsqlConnInfo &PsqlConnInfo::set_port(const wuk::u16 &value)
    {
        this->m_port = value; RETURN;
    }

    PsqlConnInfo &PsqlConnInfo::set_user(const std::string &value)
    {
        this->m_user = value; RETURN;
    }

    PsqlConnInfo &PsqlConnInfo::set_password(const std::string &value)
    {
        this->m_password = value; RETURN;
    }

    PsqlConnInfo &PsqlConnInfo::set_dbname(const std::string &value)
    {
        this->m_dbname = value; RETURN;
    }

    void PsqlConnInfo::set_conninfo(const PsqlConnParams &params) noexcept
    {
        this->set_host(params.m_host);
        this->set_port(params.m_port);
        this->set_user(params.m_user);
        this->set_password(params.m_password);
        this->set_dbname(params.m_dbname);
    }

    std::string PsqlConnInfo::get_host() const noexcept
    {
        return this->m_host;
    }

    wuk::u16 PsqlConnInfo::get_port() const noexcept
    {
        return this->m_port;
    }

    std::string PsqlConnInfo::get_user() const noexcept
    {
        return this->m_user;
    }

    std::string PsqlConnInfo::get_password() const noexcept
    {
        return this->m_password;
    }

    std::string PsqlConnInfo::get_dbname() const noexcept
    {
        return this->m_dbname;
    }

    std::string PsqlConnInfo::get_conninfo() const noexcept
    {
        std::stringstream conninfo;
        conninfo \
            << "host="     << this->m_host     << " "
            << "port="     << this->m_port     << " "
            << "user="     << this->m_user     << " "
            << "password=" << this->m_password << " "
            << "dbname="   << this->m_dbname;
        return conninfo.str();
    }

    ///////////////////////////////////////////////////////////////////////////

    ExecStatusType Psql::get_status(const PGresult *res)
    {
        // http://postgres.cn/docs/16/libpq-exec.html#LIBPQ-PQRESULTSTATUS

        /**
         * PGRES_EMPTY_QUERY        发送到服务器的字符串是空的。
         * PGRES_COMMAND_OK         成功完成一个不返回数据的命令。
         * PGRES_TUPLES_OK          成功完成返回数据的命令（例如SELECT或SHOW）。
         * PGRES_COPY_OUT           从服务器复制数据传输已开始。
         * PGRES_COPY_IN            开始进行复制（到服务器）的数据传输。
         * PGRES_BAD_RESPONSE       服务器的响应无法被理解。
         * PGRES_NONFATAL_ERROR     发生了一个非致命错误（通知或警告）。
         * PGRES_FATAL_ERROR        发生了致命错误。
         * PGRES_COPY_BOTH          开始进行数据传输（从服务器复制到服务器）。
         *                          此功能目前仅用于流式复制， 因此在普通应用程序中不应出现此状态。
         * PGRES_SINGLE_TUPLE       PGresult包含当前命令的单个结果元组。
         *                          仅当查询选择了单行模式时才会出现此状态（请参阅第 34.6 节）。
         * PGRES_PIPELINE_SYNC      PGresult表示管道模式中的一个同步点， 由PQpipelineSync请求。
         *                          此状态仅在选择了管道模式时出现。
         * PGRES_PIPELINE_ABORTED   PGresult表示接收到服务器错误的管道。
         *                          必须重复调用PQgetResult，每次调用都会返回此状态码，
         *                          直到当前管道结束，此时它将返回PGRES_PIPELINE_SYNC，然后可以恢复正常处理。
         */
        return PQresultStatus(res);
    }

    ConnStatusType Psql::get_status(const PGconn *conn)
    {
        return PQstatus(conn);
    }

    std::string Psql::get_value(PGresult *res, wuk::i32 row, wuk::i32 column)
    {
        if (PQntuples(res) == 0) {
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Psql::get_value",
                "Database record not found.");
        }

        const char *value = PQgetvalue(res, row, column);
        const wuk::ulong length = PQgetlength(res, row, column);

        return std::string(value, length);
    }

    PGresult *Psql::exec_params(const std::string &sql, const std::vector<std::string> &params)
    {
        std::vector<const char *> param_values;
        std::vector<wuk::i32> param_lengths;

        for (const auto &item : params) {
            param_values.push_back(item.c_str());
            param_lengths.push_back(static_cast<wuk::i32>(item.length()));
        }

        PGresult *result = PQexecParams(this->m_conn,
                                        sql.c_str(),
                                        params.size(),
                                        nullptr,
                                        param_values.data(),
                                        param_lengths.data(),
                                        nullptr,
                                        0);

        return result;
    }

    Psql::Psql(const PsqlConnInfo &info)
    {
        this->connect_db(info);
    }

    Psql::~Psql()
    {
        PQfinish(this->m_conn);
    }

    void Psql::connect_db(const PsqlConnInfo &info)
    {
        std::string conninfo = info.get_conninfo();
        this->m_conn = PQconnectdb(conninfo.c_str());

        if (PQstatus(this->m_conn) != CONNECTION_OK) {
            PQfinish(this->m_conn);
            throw wuk::Exception(PQstatus(this->m_conn), "wuk::im::Psql::connect_db",
                PQerrorMessage(this->m_conn));
        }
    }

    void Psql::insert(const std::string &sql, const std::vector<std::string> &params)
    {
        PGresult *res = this->exec_params(sql, params);

        ExecStatusType status = this->get_status(res);
        if (status != PGRES_COMMAND_OK) {
            std::cerr << "SQL错误：" << PQresultErrorMessage(res) << std::endl;
            std::cerr << "SQL错误详情：" << PQresultErrorField(res, PG_DIAG_SQLSTATE) << std::endl;
            PQclear(res);
            throw wuk::Exception(status, "Psql::insert", PQerrorMessage(this->m_conn));
        }

        PQclear(res);
    }

    std::string Psql::query(const std::string &sql, const std::vector<std::string> &params)
    {
        PGresult *res = this->exec_params(sql, params);

        ExecStatusType status = this->get_status(res);
        if (status != PGRES_TUPLES_OK) {
            std::cerr << "SQL错误：" << PQresultErrorMessage(res) << std::endl;
            std::cerr << "SQL错误详情：" << PQresultErrorField(res, PG_DIAG_SQLSTATE) << std::endl;
            PQclear(res);
            throw wuk::Exception(status, "Psql::query", PQerrorMessage(this->m_conn));
        }

        std::string query_res = this->get_value(res, 0, 0);
        PQclear(res);

        return query_res;
    }

    std::vector<std::vector<std::string>> Psql::query_all(const std::string& sql,
                                                          const std::vector<std::string>& params)
    {
        PGresult* res = this->exec_params(sql, params);

        ExecStatusType status = this->get_status(res);
        if (status != PGRES_TUPLES_OK) {
            std::cerr << "SQL错误：" << PQresultErrorMessage(res) << std::endl;
            std::cerr << "SQL错误详情：" << PQresultErrorField(res, PG_DIAG_SQLSTATE) << std::endl;
            PQclear(res);
            throw wuk::Exception(status, "Psql::query", PQerrorMessage(this->m_conn));
        }

        std::vector<std::vector<std::string>> rows;
        for (int i = 0; i < PQntuples(res); i++) {
            std::vector<std::string> row;
            for (int j = 0; j < PQnfields(res); j++) {
                row.push_back(get_value(res, i, j));
            }
            rows.push_back(row);
        }
        PQclear(res);
        return rows;
    }
}
