#pragma once
#include <core/WukConfig.hh>

#if WUK_SUPPORT
#include <core/WukException.hh>
#include <libpq-fe.h>

#include <vector>

namespace wuk::im {
    using string_table = std::vector<std::vector<std::string>>;

    class LIBWUK_API Psql {
    private:
        PGconn *m_conn = nullptr;

    private:
        ExecStatusType get_status(const PGresult *res) noexcept;
        ConnStatusType get_status(const PGconn *conn) noexcept;

        std::string get_value(PGresult *res, wuk::i32 row, wuk::i32 column);

        PGresult *exec_params(const std::string &sql, const std::vector<std::string> &params) noexcept;

    public:
        Psql() = default;
        Psql(const std::string &conninfo);
        ~Psql();

    public:
        void connect_db(const std::string &conninfo);

    public:
        void       insert(const std::string &sql, const std::vector<std::string> &params);
        std::string query(const std::string &sql, const std::vector<std::string> &params);
        string_table query_all(const std::string& sql, const std::vector<std::string>& params);
    };
}
#endif
