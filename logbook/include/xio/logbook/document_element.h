#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <map> // - Oh-yeah!
#include <xio/utils/xreturn>
#include <utility>
#include <memory>





namespace xio::logbook

{


/*!
    @brief Trying to implement rough html doc tag element... 

*/
class document_element
{
public:
    /*!
        @brief html document element tags: ( ex.: 'xio::logbook::document_element::tag::div' )
    */
    enum class tag :uint8_t {
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
        h1, h2, h3, h4, h5, h6,
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

    using nodeptr_t         = std::shared_ptr<document_element>;
    using collection        = std::vector<document_element::nodeptr_t>;
    using attr_val_t        = std::pair<std::string, std::string>;
    using attr_collection_t = std::vector<attr_val_t>;
private:
    utils::xstr _stream;
    utils::xstr _tag;

    attr_collection_t _attributes;

    document_element::collection _nodes;

    struct {
        uint8_t bloc    : 1; ///< this element is a bloc - then has children elements
        uint8_t closed  : 1;
        uint8_t attr    : 1; ///< has attributes. 
        uint8_t indent  : 4; ///< 0..15 niveaux d'intentations... Avec ca si y'en a pas asser...
    }_params;

    document_element::tag tag;

public:
    std::string      text();
    std::string      begin();
    std::string      end();
    std::size_t      push_attr(document_element::attr_val_t&&);

};

}
