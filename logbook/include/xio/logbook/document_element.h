#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <map> // - Oh-yeah!
#include <xio/utils/expect>
#include <utility>
#include <memory>
#include <cstdint>





namespace teacc::logbook
{


/*!
    @brief Trying to implement rough html doc tag element... 

*/
class logbook_api doc_element
{
public:
    /*!
        @brief html document element tags: ( ex.: 'teacc::logbook::document_element::tag::div' )
    */
    enum class tag : uint16_t  {
        a           ,
        p           ,
        div         ,
        section     ,
        header      ,
        abbr        ,
        address     ,
        area        ,
        article     ,
        aside       ,
        b           ,
        base        ,
        body        ,
        blockquote  ,
        br          ,
        button      ,
        canvas      ,
        caption     ,
        table       ,
        center      ,
        cite        ,
        code        ,
        col         ,
        colgroup    ,
        data        ,
        datalist    ,
        dl          ,
        dt          ,
        fieldset    ,
        figcaption  ,
        figure      ,
        font        ,
        footer      ,
        form        ,
        frame       ,
        frameset    ,
        h1          , 
        h2          , 
        h3          , 
        h4          , 
        h5          , 
        h6          ,
        head        ,
        hr          ,
        html        ,
        i           ,
        iframe      ,
        img         ,
        input       ,
        ins         ,
        kbd         ,
        label       ,
        legend      ,
        li          ,
        link        ,
        main        ,
        map         ,
        mark        ,
        meta        ,
        meter       ,
        nav         ,
        noscript    ,
        object      ,
        ol          ,
        optgroup    ,
        option      ,
        output      ,
        param       ,
        picture     ,
        pre         ,
        progress    ,
        q           ,
        rp          ,
        rt          ,
        ruby        ,
        s           ,
        samp        ,
        script      ,
        strike      ,
        strong      ,
        style       ,
        sub         ,
        summary     ,
        sup         ,
        svg         ,
        tbody       ,
        td          ,
        tmplate     ,
        textarea    ,
        tfoot       ,
        th          ,
        thead       ,
        time        ,
        title       ,
        tr          ,
        track       ,
        u           ,
        ul          ,
        var         ,
        video       ,
        wbr         ,
        nil
    };

    using shared            = std::shared_ptr<doc_element>;
    using collection        = std::vector<doc_element::shared>;
    using attr_val_t        = std::pair<std::string, std::string>;
    using attr_collection_t = std::vector<attr_val_t>;
    using iterator          = collection::iterator;

    static doc_element::shared create(doc_element*, doc_element::tag tg);
private:
    utils::xstr _stream;
    utils::xstr _tagstr;

    attr_collection_t _attributes;

    doc_element::collection _nodes;

    struct {
        uint8_t bloc    : 1; ///< this element is a bloc - then has children elements
        uint8_t closed  : 1;
        uint8_t attr    : 1; ///< has attributes. 
        uint8_t indent  : 4; ///< 0..15 niveaux d'intentations... Avec ca si y'en a pas asser...
    }_params = { 0 };

    doc_element::tag _tag = tag::nil;
    doc_element* _parent = nullptr;




protected:
    
    void append_child(doc_element::shared a_child);
    doc_element::iterator query_child(doc_element* a_child);
    void detach();
    void detach_child(doc_element* el);

public:

    doc_element() = default;
    ~doc_element();

    doc_element(doc_element* a_parent,doc_element::tag a_tag);
    doc_element(doc_element::tag a_tag);

    doc_element* parent();

    static utils::xstr tagname(doc_element::tag a_tag);
    static doc_element null;
    operator bool();
    std::string      text();
    doc_element::iterator     begin();
    doc_element::iterator     end();
    std::size_t      push_attr(doc_element::attr_val_t&&);

};

}
