#include "tdd.h"
#include <xio/utils/xstr.h>
#include <xio/sqlitedb/sqlitedb.h>
#include <xio/utils/notification.h>
#include <xio/logbook/logbook.h>




auto main() -> int {
    xio::utils::xstr str = "Hello, World!\n";
    
    str << " logbook dbname()='" << xio::logbook::logbook::dbname() << "'\n";

    std::cout << str();

    xio::xdb::sqlitedb db{ "logbook" };
    xio::xdb::sqlitedb::code c = db.open();


    xio::utils::notification::clear(
        [](xio::utils::notification& n) 
        {
            std::cout << n() << '\n';
        }
    );

    return 0;
}