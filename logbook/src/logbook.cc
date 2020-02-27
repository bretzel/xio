#include <xio/logbook/logbook.h>




namespace xio::logbook
{



/*

    logbook Database 
    ---------------------------------------------------------------
    
    database schema:
    
    [table] mode_id
        [column]colo_id[/column]
        [column]value  [/column]
        //[column]style  [/column]
    [/table]


    logfmt: 
 
    enum class color : uint8_t
    {
      black = 0, red = 1, green = 2, amber = 3, blue = 4, magenta = 5, cyan = 6, hgray, dgray, hred, hgreen, yellow, hblue, hmagenta, hcyan, white,
      reset, ///< remise-&agrave;-z&eacute;ro des attributs.
    };


    logfmt <-> database schema:

    [logfmt::mode  attribute] <-> std::string/xio::utils::xstr <-> [logbookapi] <-> std::string/xio::utils::xstr <-> [database::table name]
    [logfmt::color attribute] <-> std::string/xio::utils::xstr <-> [logbookapi] <-> std::string/xio::utils::xstr <-> [database::table name::column name]


    -------------------------------------------------------------------------------------------------------------------------------------------------

*/


std::string logbook::_dbname = "logbook";



logbook::~logbook()
{

}

std::string logbook::dbname()
{
    return logbook::_dbname;
}


}
