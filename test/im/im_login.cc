#include <openssl/evp.h>
#include <openssl/rand.h>

#include <libpq-fe.h>

#include <config/WukException.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <format>

constexpr wU32 salt_length = 16;
constexpr wU32 hash_length = 32;

namespace im {
    struct _user_info {
        wI64 uid;
        std::string name;
        wuk::Buffer salt;
        wuk::Buffer pwd_hash;
        double created;
        bool active;
    };
    typedef _user_info UserInfo;

    wuk::WukTime wuk_time;

    wI64 generator_uid()
    {
        wI64 n{0};
        RAND_bytes(reinterpret_cast<wByte *>(&n), sizeof(n));
        return n & 0xffffffff;
    }

    wuk::Buffer generator_salt()
    {
        wByte salt[salt_length] {0};
        RAND_bytes(salt, salt_length);
        wuk::Buffer result(salt, salt_length);
        return result;
    }

    wuk::Buffer generator_hash(std::string password, wuk::Buffer salt)
    {
        wByte hash_value[hash_length] {0};
        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                          salt.get_data(), salt.get_length(),
                          12408, EVP_sha256(),
                          hash_length, hash_value);
        wuk::Buffer result(hash_value, hash_length);
        return result;
    }

    double get_current_time()
    {
        return wuk_time.time();
    }

    UserInfo create_account(std::string name, std::string password)
    {
        UserInfo result {0};
        result.uid = generator_uid();
        result.name = name;
        result.salt = generator_salt();
        result.pwd_hash = generator_hash(password, result.salt);
        result.created = get_current_time();
        result.active = true;

        return result;
    }
}

std::string float2string(double value)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(5) << value;
    return ss.str();
}

wuk::Buffer get_pwd_hash_by_uid(PGconn* conn, wI64 uid)
{
    const char* query = "SELECT pwd_hash FROM users WHERE uid = $1::bigint";

    std::string uid_str = std::to_string(uid);
    const char* param_values[] = {
        uid_str.c_str()
    };

    PGresult* res = PQexecParams(conn,
                                 query,
                                 1,              // 参数数量
                                 nullptr,        // 参数类型（自动推断）
                                 param_values,   // 参数值
                                 nullptr,        // 参数长度（文本模式可为 nullptr）
                                 nullptr,        // 参数格式（nullptr 表示文本）
                                 0);             // 返回结果格式：0 = 文本

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "查询失败: " << PQerrorMessage(conn);
        PQclear(res);
        return {};
    }

    if (PQntuples(res) == 0) {
        std::cerr << "未找到 uid = " << uid << " 的记录。\n";
        PQclear(res);
        return {};
    }

    wByte *value = reinterpret_cast<wByte *>(PQgetvalue(res, 0, 0)); // 第0行第0列
    wSize length = PQgetlength(res, 0, 0);

    wuk::Buffer result(value, length);

    PQclear(res);
    return result;
}

void exec_sql(const im::UserInfo &info, const std::string &table_name)
{
    std::string server_ip;
    std::string password;

    std::cout << "Please enter password: ";
    std::cin >> password;
    std::cout << "Please enter IP: ";
    std::cin >> server_ip;

    std::stringstream conninfo;
    conninfo    << "host="     << server_ip  << " "
                << "port="     << "54324"    << " "
                << "dbname="   << "im"       << " "
                << "user="     << "postgres" << " "
                << "password=" << password   << " "
                << "sslmode="  << "require";

    PGconn *conn = PQconnectdb(conninfo.str().c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "连接失败: " << PQerrorMessage(conn);
        PQfinish(conn);
        return;
    }

    std::stringstream sql;
    sql <<  "INSERT INTO " << table_name << " (uid, name, salt, pwd_hash, created, active) "
        <<  "VALUES ($1::bigint, $2::varchar, $3::char(32), $4::char(64), $5::float8, $6::bool)";

    const char *param_values[] = {
        std::to_string(info.uid).c_str(),
        info.name.c_str(),
        info.salt.hex().c_str(),
        info.pwd_hash.hex().c_str(),
        float2string(info.created).c_str(),
        (info.active) ? "TRUE" : "FALSE"
    };

    PGresult *res = PQexecParams(conn,
                                sql.str().c_str(),
                                (sizeof(param_values) / sizeof(param_values[0])),
                                nullptr,
                                param_values,
                                nullptr,
                                nullptr,
                                0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "插入失败: " << PQerrorMessage(conn);
        PQclear(res);
        PQfinish(conn);
        return;
    }

    std::cout << "插入成功。\n";
    PQclear(res);
    PQfinish(conn);
}

class PostgreSQL {
private:
    PGconn *m_conn = nullptr;
    PGresult *m_res = nullptr;

private:
    void handler_error(wI32 err, const std::string &func, const std::string &msg)
    {
        PQclear(this->m_res);
        PQfinish(this->m_conn);
        throw wuk::Exception(err, func, msg);
    }

public:
    PostgreSQL( const std::string &host,
                const wuk::u16    &port,
                const std::string &dbname,
                const std::string &user,
                const std::string &password)
    {
        std::stringstream conninfo;
        conninfo << "host="     << host     << " "
                 << "port="     << port     << " "
                 << "dbname="   << dbname   << " "
                 << "user="     << user     << " "
                 << "password=" << password << " "
                 << "sslmode="  << "require";

        this->m_conn = PQconnectdb(conninfo.str().c_str());

        if (PQstatus(this->m_conn) != CONNECTION_OK) {
            this->handler_error(PQstatus(this->m_conn), "PostgreSQL::PostgreSQL",
                PQerrorMessage(this->m_conn));
        }
    }

    ~PostgreSQL()
    {
        PQclear(this->m_res);
        PQfinish(this->m_conn);
    }

    std::string get_value(PGresult *res, wuk::i32 row, wuk::i32 column)
    {
        if (PQntuples(res) == 0) {
            this->handler_error(wuk::Error::ERR, "PostgreSQL::get_value",
                "Database record not found.");
        }

        const char *value = PQgetvalue(res, row, column);
        const wuk::ulong length = PQgetlength(res, row, column);

        return std::string(value, length);
    }

public:
    void exec_sql(const std::string &sql, const std::vector<std::string> &params)
    {
        std::vector<const char *> param_values;
        std::vector<wuk::i32> param_lengths;

        for (const auto &item : params) {
            param_values.push_back(item.c_str());
            param_lengths.push_back(static_cast<wuk::i32>(item.length()));
        }

        this->m_res = PQexecParams(this->m_conn,
                                   sql.c_str(),
                                   params.size(),
                                   nullptr,
                                   param_values.data(),
                                   param_lengths.data(),
                                   nullptr,
                                   0);

        if (PQresultStatus(this->m_res) != PGRES_COMMAND_OK) {
            this->handler_error(PQresultStatus(this->m_res), "PostgreSQL::exec_sql",
                PQerrorMessage(this->m_conn));
        }
    }

    std::string query_sql()
    {
        
    }
};

int main()
{
    // im::UserInfo info = im::create_account("test_user_0007", "12345678");
    // exec_sql(info, "users");

    return 0;
}
