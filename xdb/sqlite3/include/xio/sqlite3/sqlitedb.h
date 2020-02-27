#pragma once

#include <xio/sqlite3/sqlite3api.h>
#include <sqlite3.h>
#include <string>


namespace xio::xdb
{



class xiosqlite3_api sqlitedb
{
    sqlite3*        _db = nullptr;
    std::string     _dbname;


public:
    sqlitedb() = default;
    sqlitedb(std::string&& a_dbname) noexcept;

    ~sqlitedb();

};



}
