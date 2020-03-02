#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <map> // - Oh-yeah!
#include <xio/utils/xreturn>

//#include <xio/sqlitedb/sqlitedb.h>

namespace xio::logbook
{

/*!
    @brief The Log Book ...  
    
    @author Serge Lussier lussier.serge@gmail.com

    @note logbook is a single instance class!
*/
class logbook_api book
{

    // Testons le stockage static private:

    static std::string          _dbname; // pas besoin d'exporter | no need to export. \O/ !!
//    static xio::xdb::sqlitedb   _dbdata;

public:

    


    //private:
    //    static std::map<logbook::color, std::string> _web_colors;

    using result = utils::xreturn<utils::notification::code>;

    book() = default;
    ~book();
    static std::string dbname(); // DOH!

    book::result open();


};

}