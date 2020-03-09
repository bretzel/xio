#pragma once

#include <xio/logbook/logbookapi.h>
#include <map>
#include <xio/utils/xstr.h>
#include <xio/utils/expect>
#include <xio/logbook/logfmt.h>
//#include <xio/logbook/document_element.h>

namespace xio::logbook
{



class logfmt;
class logdebug;
class loginfo;
class logfail;
class logfatal;
class logerror;
class logwaring;
class logsucces;
class lognote;
//class document_element;

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

    
    enum class object : uint8_t
    {
        
        title,           ///< Titre au sens abstrait et generique.
        global_stream,   ///< Pool chronologique de toutes les entrees
        // ...
        //        inc,           ///< desactivated.   ///< indentation - increment; incr&eacute;menter l'indentation
        //        dec,                ///< indentation -  decrement ; d&eacute;cr&eacute;menter l'indentation
    };
    
    enum class mode : uint8_t
    {
        none = 0,   ///< unformated.
        plain,      ///< unformated.
        ansi,       ///< Ainsi standard format - First TDD: Using utils::notification "stack" for output.
        html,       ///< format html.
        vcell,     ///< format Video Cell Bitmap Bloc  ( ncurses ).
    }_mode = mode::none;


    enum class attrib : uint8_t
    {
        /* html: */ normal, italic, strong, sup, _sup, sub, _sub, pre, endpre ///< styles suppl&eacute;mentaires du texte au format `html`
    };


    //private:
    //    static std::map<logbook::color, std::string> _web_colors;

    using result = utils::expect<utils::notification::code>;

    book() = default;
    ~book();
    std::string dbname(); // DOH!

    book::result open();
    doc_element& element();

    static doc_element::shared create_element(doc_element::shared parent_element, doc_element::tag tg);

};





}
