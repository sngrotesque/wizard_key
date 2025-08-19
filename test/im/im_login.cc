#include <config/WukException.hh>
#include <crypto/WukChaCha20.hh>
#include <WukBuffer.hh>
#include <WukTime.hh>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <libpq-fe.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <format>
#include <atomic>
#include <mutex>

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
    struct _user_info_db {
        std::string uid;
        std::string name;
        std::string salt;
        std::string hash;
        std::string created;
        std::string active;
    };
    typedef _user_info UserInfo;
    typedef _user_info_db UserInfoDB;

    static wuk::Time wuk_time;

    wI64 generate_uid()
    {
        wI64 n{0};
        RAND_bytes(reinterpret_cast<wByte *>(&n), sizeof(n));
        return n & 0xffffffff;
    }

    wuk::Buffer generate_salt()
    {
        wByte salt[salt_length] {0};
        RAND_bytes(salt, salt_length);
        wuk::Buffer result(salt, salt_length);
        return result;
    }

    wuk::Buffer generate_hash(std::string password, wuk::Buffer salt)
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
        return wuk_time.time<double>();
    }

    UserInfo create_account(std::string name, std::string password)
    {
        UserInfo result {0};

        result.uid = generate_uid();
        result.name = name;
        result.salt = generate_salt();
        result.pwd_hash = generate_hash(password, result.salt);
        result.created = get_current_time();
        result.active = true;

        return result;
    }
}

std::string float2string(double v)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(5) << v;
    return ss.str();
}

/**
 * http://postgres.cn/docs/16/libpq.html        libpq - C 库；客户端接口
 * http://postgres.cn/docs/16/libpq-exec.html   命令执行函数
 */
class WukPSQL {
private:
    PGconn *m_conn = nullptr;

private:
    std::string get_value(PGresult *res, wuk::i32 row, wuk::i32 column)
    {
        if (PQntuples(res) == 0) {
            throw wuk::Exception(wuk::Error::ERR, "WukPSQL::get_value",
                "Database record not found.");
        }

        const char *value = PQgetvalue(res, row, column);
        const wuk::ulong length = PQgetlength(res, row, column);

        return std::string(value, length);
    }

    ExecStatusType get_status(const PGresult *res)
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

    ConnStatusType get_status(const PGconn *conn)
    {
        return PQstatus(conn);
    }

    PGresult *exec_params(const std::string &sql, const std::vector<std::string> &params)
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

public:
    WukPSQL(const std::string &host,
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
            PQfinish(this->m_conn);
            throw wuk::Exception(PQstatus(this->m_conn), "WukPSQL::WukPSQL",
                PQerrorMessage(this->m_conn));
        }
    }

    ~WukPSQL()
    {
        PQfinish(this->m_conn);
    }

public:
    void insert(const std::string &sql, const std::vector<std::string> &params)
    {
        PGresult *res = this->exec_params(sql, params);

        ExecStatusType status = this->get_status(res);
        if (status != PGRES_COMMAND_OK) {
            std::cerr << "SQL错误：" << PQresultErrorMessage(res) << std::endl;
            std::cerr << "SQL错误详情：" << PQresultErrorField(res, PG_DIAG_SQLSTATE) << std::endl;
            PQclear(res);
            throw wuk::Exception(status, "WukPSQL::insert", PQerrorMessage(this->m_conn));
        }

        PQclear(res);
    }

    std::string query(const std::string &sql, const std::vector<std::string> &params)
    {
        PGresult *res = this->exec_params(sql, params);

        ExecStatusType status = this->get_status(res);
        if (status != PGRES_TUPLES_OK) {
            std::cerr << "SQL错误：" << PQresultErrorMessage(res) << std::endl;
            std::cerr << "SQL错误详情：" << PQresultErrorField(res, PG_DIAG_SQLSTATE) << std::endl;
            PQclear(res);
            throw wuk::Exception(status, "WukPSQL::query", PQerrorMessage(this->m_conn));
        }

        std::string query_res = this->get_value(res, 0, 0);
        PQclear(res);

        return query_res;
    }
};



void psql_test(const std::string &ip, const std::string &pw, const wuk::u16 &port)
{
    WukPSQL psql(ip, port, "im", "postgres", pw);

    std::string sql(
        "INSERT INTO users (uid, name, salt, pwd_hash, created, active) "
        "VALUES ($1::bigint, $2::varchar(32), $3::char(32), "
        "$4::char(64), $5::double precision, $6::boolean)"
    );
    im::UserInfo info = im::create_account("sngrotesque", "12345678");

    std::vector<std::string> params;
    params.reserve(6);
    params.push_back(std::to_string(info.uid));
    params.push_back(info.name);
    params.push_back(info.salt.hex());
    params.push_back(info.pwd_hash.hex());
    params.push_back(float2string(info.created));
    params.push_back(info.active ? "TRUE" : "FALSE");

    psql.insert(sql, params);
}

int main()
{
    try {
        psql_test("", "", 0);

    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
