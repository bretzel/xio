#pragma once

#include <xio/sqlitedb/sqlite3api.h>
#include <sqlite3.h>
#include <string>
#include <xio/utils/xreturn.h>


namespace xio::xdb
{



class xiosqlite3_api sqlitedb
{
    sqlite3*        _db = nullptr;
    std::string     _dbname;
    // TESTING & DEV : No PATH is supported in db file name yet. File must be in the current working directory.

public:
    sqlitedb() = default;
    sqlitedb(std::string&& a_dbname) noexcept;

    ~sqlitedb();

    using code = utils::xreturn<utils::notification::code>;

    sqlitedb::code open();
    sqlitedb::code close();
    sqlitedb::code create();
    //sqlitedb::code destroy();
};



}
