#include <xio/logbook/logbook.h>




namespace xio::logbook
{






using utils::notification;



std::string book::_dbname = "logbook";
//xio::xdb::sqlitedb   logbook::_dbdata;


book::~book()
{

}

std::string book::dbname()
{
    return book::_dbname;
}

book::result book::open()
{
    

    return notification::code::implement;
}


}
