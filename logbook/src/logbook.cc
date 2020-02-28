#include <xio/logbook/logbook.h>




namespace xio::logbook
{



/*

    logbook Database 
    ---------------------------------------------------------------
    
    database schema:
    
    <table name="color">
        <Primary Key="id"/> 
        <column name="name"/>
        <column name="html"/>
        <column name="ansi"/>
    </table>


    logfmt: 
 
    enum class color : uint8_t
    {
      black = 0, red = 1, green = 2, amber = 3, blue = 4, magenta = 5, cyan = 6, hgray, dgray, hred, hgreen, yellow, hblue, hmagenta, hcyan, white,
      reset, ///< remise-&agrave;-z&eacute;ro des attributs.
    };


    logfmt <-> database schema:

    [logfmt::mode  attribute] <-> std::string/xio::utils::xstr <-> [logbookapi] <-> std::string/xio::utils::xstr <-> [database::color_table::column.name{html,ansi}]
    [logfmt::color attribute] <-> std::string/xio::utils::xstr <-> [logbookapi] <-> std::string/xio::utils::xstr <-> [database::color_table::column.name[id].value{html,ansi}]

    
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
