#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <map> // - Oh-yeah!
#include <xio/sqlitedb/sqlitedb.h>

namespace xio::logbook
{

class logbook_api logbook
{

    // Testons le stockage static private:

    static std::string          _dbname; // pas besoin d'exporter | no need to export. \O/ !!
    static xio::xdb::sqlitedb   _dbdata;

public:

    logbook() = default;
    ~logbook();
    static std::string dbname(); // DOH!

};

}