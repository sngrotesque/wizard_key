#include <db/WukPsql.hh>
#include <WukRandom.hh>
#include <WukMisc.hh>
#include <WukTime.hh>

#include <iostream>
#include <iomanip>

using namespace wuk::misc;

std::string create_conninfo()
{
    std::string host("47.79.146.143");
    std::string port("54324");
    std::string user("test");
    std::string password("psql_test_user");
    std::string dbname("im");

    if (host.empty()) {
        std::cout << log_utf8("请输入服务器地址：");
        std::getline(std::cin, host);
    }
    if (password.empty()) {
        std::cout << log_utf8("请输入服务器密码：");
        std::getline(std::cin, password);
    }

    std::string conninfo = fmt::format(
        "host={0} port={1} user={2} password={3} dbname={4}",
        host, port, user, password, dbname
    );

    return conninfo;
}

void libpq_test()
{
    const char _charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::vector<char> charset(_charset, _charset + (sizeof(_charset) - 1));

    wuk::Random random;
    wuk::Time time;
    wuk::i32 n_rows;
    wuk::i32 n_cols;
    PGconn *conn = nullptr;
    PGresult *res = nullptr;

    auto random_string = [&](wuk::i32 min_val, wuk::i32 max_val) -> std::string
    {
        wuk::ulong length = random.randint(min_val, max_val);

        std::string result(length, '\0');

        for (wuk::ulong i = 0; i < length; ++i) {
            result[i] = random.choice(charset);
        }

        return result;
    };

    // 连接数据库
    std::string conninfo = create_conninfo();
    conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "ERROR 111\n";
        std::cout << log_utf8(fmt::format("连接失败：{0}", PQerrorMessage(conn)));
        PQfinish(conn);
        return;
    }

    // 执行查询
    res = PQexec(conn, "SELECT * from test;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "ERROR 222\n";
        std::cout << log_utf8(fmt::format("查询失败：{0}", PQresultErrorMessage(res)));
        PQclear(res);
        PQfinish(conn);
        return;
    }

    // 获取行列数
    n_rows = PQntuples(res);
    n_cols = PQnfields(res);

    std::cout << log_utf8(fmt::format("查询结果包含{0}行，{1}列。", n_rows, n_cols)) << std::endl;

    // 遍历结果集
    for (wuk::i32 col = 0; col < n_cols; ++col) {
        std::cout << log_utf8(PQfname(res, col)) << (((col+1) != n_cols) ? " " : "\n");
    }

    // 打印所有结果
    for (wuk::i32 row = 0; row < n_rows; ++row) {
        for (wuk::i32 col = 0; col < n_cols; ++col) {
            std::cout << log_utf8(PQgetvalue(res, row, col)) << (((col+1) != n_cols) ? " " : "\n");
        }
    }

    for (wuk::i32 count = 0; count < 16; ++count) {
        // 清理结果资源，以便于下面的插入操作
        PQclear(res);

        // 插入数据
        std::string sql = fmt::format(
            "INSERT INTO test (uid, name, created) VALUES ({0}, '{1}', {2:.3f})",
            random.randint(0, 100000000), random_string(6, 16), time.time<double>()
        );
        res = PQexec(conn, sql.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cout << "ERROR 333\n";
            std::cout << log_utf8(fmt::format("插入失败：{0}", PQresultErrorMessage(res)));
            PQclear(res);
            PQfinish(conn);
            return;
        }
    }

    // 清理资源
    PQclear(res);

    // 执行一个无实际结果的查询
    res = PQexec(conn, "SELECT uid, name from test where uid = -1;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "ERROR 222\n";
        std::cout << log_utf8(fmt::format("查询失败：{0}", PQresultErrorMessage(res)));
        PQclear(res);
        PQfinish(conn);
        return;
    }

    // 获取行列数
    n_rows = PQntuples(res);
    n_cols = PQnfields(res);

    std::cout << log_utf8(fmt::format("查询结果包含{0}行，{1}列。", n_rows, n_cols)) << std::endl;

    // 清理资源
    PQclear(res);
    PQfinish(conn);
}

void wuk_psql_test()
{
    try {
        std::string conninfo = create_conninfo();
        wuk::db::psql::Connection conn(conninfo);
        wuk::db::psql::Work work(std::move(conn));
        wuk::db::psql::Result res;

        // 查询操作
        res = work.exec("SELECT created FROM test WHERE name = 'G6g5xtffu';");

        std::vector<std::string> rows = res[0];
        std::cout << log_utf8(fmt::format("获取的结果数量：{0}", rows.size())) << std::endl;

        for (auto &item : rows) {
            std::cout << item << "\t";
        }
        std::cout << std::endl;

        // 插入操作
        std::vector<wuk::db::psql::Param> params;
        params.push_back(wuk::db::psql::Param{"12345678", 0});
        params.push_back(wuk::db::psql::Param{"test_user", 0});

        res = work.exec("INSERT INTO test (uid, name) VALUES ($1, $2);",
                        params,
                        wuk::db::psql::ResultFormat::TEXT);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

int main()
{
    // libpq_test();
    wuk_psql_test();

    return 0;
}
