#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/sqlite3/sqlitedb.h>
#include <xio/utils/notification.h>


auto main() -> int {
    xio::utils::xstr str = "Hello, World!\n";

    std::cout << str();

    xio::xdb::sqlitedb db{ "logbook" };

    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );

    return 0;
}