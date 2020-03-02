#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <map> // - Oh-yeah!
#include <xio/utils/xreturn>
#include <utility>
#include <memory>
//#include <xio/sqlitedb/sqlitedb.h>

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

    class logbook_api doc_element
    {
        enum class tag:uint8_t {
            a,
            p,
            div,
            section,
            header,
            abbr,
            address,
            area,
            article,
            aside,
            b,
            base,
            body,
            blockquote,
            br,
            button,
            canvas,
            caption,
            table,
            center,
            cite,
            code,
            col,
            colgroup,
            data,
            datalist,
            dl,
            dt,
            fieldset,
            figcaption,
            figure,
            font,
            footer,
            form,
            frame,
            frameset,
            h1,h2,h3,h4,h5,h6,
            head,
            hr,
            html,
            i,
            iframe,
            img,
            input,
            ins,
            kbd,
            label,
            legend,
            li,
            link,
            main,
            map,
            mark,
            meta,
            meter,
            nav,
            noscript,
            object,
            ol,
            optgroup,
            option,
            output,
            param,
            picture,
            pre,
            progress,
            q,
            rp,
            rt,
            ruby,
            s,
            samp,
            script,
            strike,
            strong,
            style,
            sub,
            summary,
            sup,
            svg,
            table,
            tbody,
            td,
            tmplate,
            textarea,
            tfoot,
            th,
            thead,
            time,
            title,
            tr,
            track,
            u,
            ul,
            var,
            video,
            wbr
        };

        
        public:   
        using nodeptr_t = std::shared_ptr<doc_element>;
        private:
        using collection = std::vector<doc_element::nodeptr_t>;
        using attr_val_t = std::pair<std::string, std::string>;
        using attr_list_t = std::vector<attr_val_t>;

        bool             bloc;
        doc_element::tag tag;
        public:
        std::string      text();
        std::string      begin();
        std::string      end();

         
        doc_element::collection _children;

    public:


    };

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

    using result = utils::xreturn<utils::notification::code>;

    book() = default;
    ~book();
    static std::string dbname(); // DOH!

    book::result open();


};

}