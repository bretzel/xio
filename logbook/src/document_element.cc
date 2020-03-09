#include <xio/logbook/document_element.h>




namespace xio::logbook
{


    doc_element doc_element::null;

    std::map<doc_element::tag, std::string> tags = {
    { doc_element::tag::a           ,"a"},
    { doc_element::tag::p           ,"p"},
    { doc_element::tag::div         ,"div" },
    { doc_element::tag::section     ,"section" },
    { doc_element::tag::header      ,"header" },
    { doc_element::tag::abbr        ,"abbr" },
    { doc_element::tag::address     ,"address" },
    { doc_element::tag::area        ,"area" },
    { doc_element::tag::article     ,"article" },
    { doc_element::tag::aside       ,"aside" },
    { doc_element::tag::b           ,"b" },
    { doc_element::tag::base        ,"base" },
    { doc_element::tag::body        ,"body" },
    { doc_element::tag::blockquote  ,"blockquote" },
    { doc_element::tag::br          ,"br" },
    { doc_element::tag::button      ,"button" },
    { doc_element::tag::canvas      ,"canvas" },
    { doc_element::tag::caption     ,"caption" },
    { doc_element::tag::table       ,"table" },
    { doc_element::tag::center      ,"center" },
    { doc_element::tag::cite        ,"cite" },
    { doc_element::tag::code        ,"code" },
    { doc_element::tag::col         ,"col" },
    { doc_element::tag::colgroup    ,"colgroup" },
    { doc_element::tag::data        ,"data" },
    { doc_element::tag::datalist    ,"datalist" },
    { doc_element::tag::dl          ,"dl" },
    { doc_element::tag::dt          ,"dt" },
    { doc_element::tag::fieldset    ,"fieldset" },
    { doc_element::tag::figcaption  ,"figcaption" },
    { doc_element::tag::figure      ,"figure" },
    { doc_element::tag::font        ,"font" },
    { doc_element::tag::footer      ,"footer" },
    { doc_element::tag::form        ,"form" },
    { doc_element::tag::frame       ,"frame" },
    { doc_element::tag::frameset    ,"frameset" },
    { doc_element::tag::h1          ,"h1" },
    { doc_element::tag::h2          ,"h2" },
    { doc_element::tag::h3          ,"h3" },
    { doc_element::tag::h4          ,"h4" },
    { doc_element::tag::h5          ,"h5" },
    { doc_element::tag::h6          ,"h6" },
    { doc_element::tag::head        ,"head" },
    { doc_element::tag::hr          ,"hr" },
    { doc_element::tag::html        ,"html" },
    { doc_element::tag::i           ,"i" },
    { doc_element::tag::iframe      ,"iframe" },
    { doc_element::tag::img         ,"img" },
    { doc_element::tag::input       ,"input" },
    { doc_element::tag::ins         ,"ins" },
    { doc_element::tag::kbd         ,"kbd" },
    { doc_element::tag::label       ,"label" },
    { doc_element::tag::legend      ,"legend" },
    { doc_element::tag::li          ,"li" },
    { doc_element::tag::link        ,"link" },
    { doc_element::tag::main        ,"main" },
    { doc_element::tag::map         ,"map" },
    { doc_element::tag::mark        ,"mark" },
    { doc_element::tag::meta        ,"meta" },
    { doc_element::tag::meter       ,"meter" },
    { doc_element::tag::nav         ,"nav" },
    { doc_element::tag::noscript    ,"noscript" },
    { doc_element::tag::object      ,"object" },
    { doc_element::tag::ol          ,"ol" },
    { doc_element::tag::optgroup    ,"optgroup" },
    { doc_element::tag::option      ,"option" },
    { doc_element::tag::output      ,"output" },
    { doc_element::tag::param       ,"param" },
    { doc_element::tag::picture     ,"picture" },
    { doc_element::tag::pre         ,"pre" },
    { doc_element::tag::progress    ,"progress" },
    { doc_element::tag::q           ,"q" },
    { doc_element::tag::rp          ,"rp" },
    { doc_element::tag::rt          ,"rt" },
    { doc_element::tag::ruby        ,"ruby" },
    { doc_element::tag::s           ,"s" },
    { doc_element::tag::samp        ,"samp" },
    { doc_element::tag::script      ,"script" },
    { doc_element::tag::strike      ,"strike" },
    { doc_element::tag::strong      ,"strong" },
    { doc_element::tag::style       ,"style " },
    { doc_element::tag::sub         ,"sub" },
    { doc_element::tag::summary     ,"summary" },
    { doc_element::tag::sup         ,"sup" },
    { doc_element::tag::svg         ,"svg" },
    { doc_element::tag::tbody       ,"tbody" },
    { doc_element::tag::td          ,"td" },
    { doc_element::tag::tmplate     ,"tmplate" },
    { doc_element::tag::textarea    ,"textarea" },
    { doc_element::tag::tfoot       ,"tfoot" },
    { doc_element::tag::th          ,"th" },
    { doc_element::tag::thead       ,"thead" },
    { doc_element::tag::time        ,"time" },
    { doc_element::tag::title       ,"title" },
    { doc_element::tag::tr          ,"tr" },
    { doc_element::tag::track       ,"track" },
    { doc_element::tag::u           ,"u" },
    { doc_element::tag::ul          ,"ul" },
    { doc_element::tag::var         ,"var" },
    { doc_element::tag::video       ,"video" },
    { doc_element::tag::wbr         ,"wbr"},
    { doc_element::tag::nil         ,"null" }
};


/*!

    @brief Appends a attr_vat_t pair into the attributes list of the element.

    @code de.push_attr({"class","ui button raised"});

    @author &copy; Serge Lussier (Lonesome Coder), lussier.serge@gmail.com
*/

std::size_t  doc_element::push_attr(doc_element::attr_val_t&& a)
{
    _attributes.push_back(std::move(a));
    return _attributes.size();
}


doc_element::iterator  doc_element::begin()
{
    return _nodes.begin();
}


doc_element::iterator  doc_element::end()
{
    return _nodes.end();
}

doc_element::~doc_element()
{
    std::cout << __PRETTY_FUNCTION__ << text() << " -  \\O/\n";
}

doc_element::doc_element(doc_element::shared a_parent, doc_element::tag a_tag): _tag(a_tag), _parent(a_parent) //_parent(std::make_shared<document_element>(a_parent))
{
    _tagstr = doc_element::tagname(_tag);
}

doc_element::doc_element(doc_element::tag a_tag) : _tag(a_tag)
{
    _tagstr = doc_element::tagname(_tag);
}

doc_element& doc_element::parent()
{
    if (!_parent) return doc_element::null;
    return *_parent;
}

utils::xstr doc_element::tagname(doc_element::tag a_tag)
{
    return tags[a_tag];
}


doc_element::operator bool()
{
    return this != &doc_element::null;
}


std::string  doc_element::text()
{
    _stream = "<";
    _stream << _tagstr << ">";

    return _stream();
}



doc_element::shared doc_element::create(doc_element::shared a_parent, doc_element::tag tg)
{
    return std::make_shared<doc_element>(a_parent, tg);
}

}

