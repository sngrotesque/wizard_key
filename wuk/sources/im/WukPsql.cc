#include <im/WukPsql.hh>

namespace wuk::im {
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
        /**
         * 	CONNECTION_OK,
	     * 	CONNECTION_BAD,
         * 	
         * 	CONNECTION_STARTED,           正在等待连接。
         * 	CONNECTION_MADE,              连接正常；等待发送。
         * 	CONNECTION_AWAITING_RESPONSE, 正在等待来自数据库主进程的响应。
         * 	CONNECTION_AUTH_OK,           已接收身份验证；等待后端启动。
         * 	CONNECTION_SETENV,            此状态不再使用。
         * 	CONNECTION_SSL_STARTUP,       正在执行SSL握手。
         * 	CONNECTION_NEEDED,            内部状态：需要connect()。
         * 	CONNECTION_CHECK_WRITABLE,    正在检查会话是否为读写。
         * 	CONNECTION_CONSUME,           消耗任何额外的消息。
         * 	CONNECTION_GSS_STARTUP,       正在协商GSSAPI。
         * 	CONNECTION_CHECK_TARGET,      内部状态：正在检查目标服务器属性。
         * 	CONNECTION_CHECK_STANDBY,     正在检查服务器是否处于待机模式。
         * 	CONNECTION_ALLOCATED,         正在等待连接尝试启动。
         */
        return PQstatus(conn);
    }

    std::string Psql::get_value(PGresult *res, wuk::i32 row, wuk::i32 column)
    {
        wuk::i32 n_row = PQntuples(res);
        wuk::i32 n_col = PQnfields(res);

        if (n_row == 0) {
            return {};
        }

        bool row_invalidity = ((row    < 0) || (row    >= n_row));
        bool col_invalidity = ((column < 0) || (column >= n_col));

        if (row_invalidity || col_invalidity) {
            PQclear(res);
            throw wuk::Exception(wuk::Error::ERR, "wuk::im::Psql::get_value",
                "Exceeding the allowed range.");
        }

        // 此项如果为空值
        if (PQgetisnull(res, row, column)) {
            return {};
        }

        const char     *value = PQgetvalue(res, row, column);
        const wuk::i32 length = PQgetlength(res, row, column);

        return std::string(value, length);
    }

    PGresult *Psql::exec_params(const std::string &sql, const std::vector<std::string> &params)
    {
        std::vector<const char *> param_values;
        std::vector<wuk::i32> param_lengths;
        // std::vector<wuk::i32> param_formats(9);
        // param_formats[2] = 1;
        // param_formats[3] = 1;

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
                                        // param_formats.data(),
                                        nullptr,
                                        0);

        return result;
    }

    Psql::Psql(const std::string &conninfo)
    {
        this->connect_db(conninfo);
    }

    Psql::~Psql()
    {
        PQfinish(this->m_conn);
    }

    void Psql::connect_db(const std::string &conninfo)
    {
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
            PQclear(res);
            throw wuk::Exception(status, "Psql::query", PQerrorMessage(this->m_conn));
        }

        std::string query_res = this->get_value(res, 0, 0);
        PQclear(res);

        return query_res;
    }

    string_table Psql::query_all(const std::string& sql, const std::vector<std::string>& params)
    {
        PGresult* res = this->exec_params(sql, params);

        ExecStatusType status = this->get_status(res);
        if (status != PGRES_TUPLES_OK) {
            PQclear(res);
            throw wuk::Exception(status, "Psql::query", PQerrorMessage(this->m_conn));
        }

        string_table rows;
        for (int i = 0; i < PQntuples(res); i++) {
            std::vector<std::string> row;
            for (int j = 0; j < PQnfields(res); j++) {
                row.push_back(this->get_value(res, i, j));
            }
            rows.push_back(std::move(row));
        }

        PQclear(res);
        return rows;
    }
}
