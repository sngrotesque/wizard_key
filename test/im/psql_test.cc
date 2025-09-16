#include <im/WukPsql.hh>

#include <iostream>
#include <iomanip>

int main()
{
    try {
        const char *conninfo = {
            "user=postgres "
            "port=54324 "
            "dbname=im"
        };
        wuk::im::psql::Connection conn(conninfo);
    } catch (const wuk::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
