#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <libpq-fe.h>

#include <vector>
#include <iostream>

namespace wuk::im {
    struct PsqlConnParams {
        std::string m_host;
        wuk::u16    m_port;
        std::string m_user;
        std::string m_password;
        std::string m_dbname;
    };

    class LIBWUK_API PsqlConnInfo {
    private:
        std::string m_host;
        wuk::u16    m_port;
        std::string m_user;
        std::string m_password;
        std::string m_dbname;

    public:
        PsqlConnInfo() = default;
        PsqlConnInfo(const PsqlConnParams &params);

    public:
        PsqlConnInfo &set_host    (const std::string &value);
        PsqlConnInfo &set_port    (const wuk::u16    &value);
        PsqlConnInfo &set_user    (const std::string &value);
        PsqlConnInfo &set_password(const std::string &value);
        PsqlConnInfo &set_dbname  (const std::string &value);

        void set_conninfo(const PsqlConnParams &params) noexcept;

        std::string get_host()     const noexcept;
        wuk::u16    get_port()     const noexcept;
        std::string get_user()     const noexcept;
        std::string get_password() const noexcept;
        std::string get_dbname()   const noexcept;

        std::string get_conninfo() const noexcept;
    };

    class LIBWUK_API Psql {
    private:
        PGconn *m_conn = nullptr;

    private:
        ExecStatusType get_status(const PGresult *res);
        ConnStatusType get_status(const PGconn *conn);

        std::string get_value(PGresult *res, wuk::i32 row, wuk::i32 column);

        PGresult *exec_params(const std::string &sql, const std::vector<std::string> &params);

    public:
        Psql() = default;
        Psql(const PsqlConnInfo &info);
        ~Psql();

    public:
        void connect_db(const PsqlConnInfo &info);

    public:
        void       insert(const std::string &sql, const std::vector<std::string> &params);
        std::string query(const std::string &sql, const std::vector<std::string> &params);
        std::vector<std::vector<std::string>> query_all(const std::string& sql,
                                                        const std::vector<std::string>& params);
    };
}
#endif
