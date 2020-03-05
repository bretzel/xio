#include <xio/logbook/document_element.h>




namespace xio::logbook
{


    document_element document_element::null;

    std::map<document_element::tag, std::string> tags = {
    { document_element::tag::a           ,"a"},
    { document_element::tag::p           ,"p"},
    { document_element::tag::div         ,"div" },
    { document_element::tag::section     ,"section" },
    { document_element::tag::header      ,"header" },
    { document_element::tag::abbr        ,"abbr" },
    { document_element::tag::address     ,"address" },
    { document_element::tag::area        ,"area" },
    { document_element::tag::article     ,"article" },
    { document_element::tag::aside       ,"aside" },
    { document_element::tag::b           ,"b" },
    { document_element::tag::base        ,"base" },
    { document_element::tag::body        ,"body" },
    { document_element::tag::blockquote  ,"blockquote" },
    { document_element::tag::br          ,"br" },
    { document_element::tag::button      ,"button" },
    { document_element::tag::canvas      ,"canvas" },
    { document_element::tag::caption     ,"caption" },
    { document_element::tag::table       ,"table" },
    { document_element::tag::center      ,"center" },
    { document_element::tag::cite        ,"cite" },
    { document_element::tag::code        ,"code" },
    { document_element::tag::col         ,"col" },
    { document_element::tag::colgroup    ,"colgroup" },
    { document_element::tag::data        ,"data" },
    { document_element::tag::datalist    ,"datalist" },
    { document_element::tag::dl          ,"dl" },
    { document_element::tag::dt          ,"dt" },
    { document_element::tag::fieldset    ,"fieldset" },
    { document_element::tag::figcaption  ,"figcaption" },
    { document_element::tag::figure      ,"figure" },
    { document_element::tag::font        ,"font" },
    { document_element::tag::footer      ,"footer" },
    { document_element::tag::form        ,"form" },
    { document_element::tag::frame       ,"frame" },
    { document_element::tag::frameset    ,"frameset" },
    { document_element::tag::h1          ,"h1" },
    { document_element::tag::h2          ,"h2" },
    { document_element::tag::h3          ,"h3" },
    { document_element::tag::h4          ,"h4" },
    { document_element::tag::h5          ,"h5" },
    { document_element::tag::h6          ,"h6" },
    { document_element::tag::head        ,"head" },
    { document_element::tag::hr          ,"hr" },
    { document_element::tag::html        ,"html" },
    { document_element::tag::i           ,"i" },
    { document_element::tag::iframe      ,"iframe" },
    { document_element::tag::img         ,"img" },
    { document_element::tag::input       ,"input" },
    { document_element::tag::ins         ,"ins" },
    { document_element::tag::kbd         ,"kbd" },
    { document_element::tag::label       ,"label" },
    { document_element::tag::legend      ,"legend" },
    { document_element::tag::li          ,"li" },
    { document_element::tag::link        ,"link" },
    { document_element::tag::main        ,"main" },
    { document_element::tag::map         ,"map" },
    { document_element::tag::mark        ,"mark" },
    { document_element::tag::meta        ,"meta" },
    { document_element::tag::meter       ,"meter" },
    { document_element::tag::nav         ,"nav" },
    { document_element::tag::noscript    ,"noscript" },
    { document_element::tag::object      ,"object" },
    { document_element::tag::ol          ,"ol" },
    { document_element::tag::optgroup    ,"optgroup" },
    { document_element::tag::option      ,"option" },
    { document_element::tag::output      ,"output" },
    { document_element::tag::param       ,"param" },
    { document_element::tag::picture     ,"picture" },
    { document_element::tag::pre         ,"pre" },
    { document_element::tag::progress    ,"progress" },
    { document_element::tag::q           ,"q" },
    { document_element::tag::rp          ,"rp" },
    { document_element::tag::rt          ,"rt" },
    { document_element::tag::ruby        ,"ruby" },
    { document_element::tag::s           ,"s" },
    { document_element::tag::samp        ,"samp" },
    { document_element::tag::script      ,"script" },
    { document_element::tag::strike      ,"strike" },
    { document_element::tag::strong      ,"strong" },
    { document_element::tag::style       ,"style " },
    { document_element::tag::sub         ,"sub" },
    { document_element::tag::summary     ,"summary" },
    { document_element::tag::sup         ,"sup" },
    { document_element::tag::svg         ,"svg" },
    { document_element::tag::tbody       ,"tbody" },
    { document_element::tag::td          ,"td" },
    { document_element::tag::tmplate     ,"tmplate" },
    { document_element::tag::textarea    ,"textarea" },
    { document_element::tag::tfoot       ,"tfoot" },
    { document_element::tag::th          ,"th" },
    { document_element::tag::thead       ,"thead" },
    { document_element::tag::time        ,"time" },
    { document_element::tag::title       ,"title" },
    { document_element::tag::tr          ,"tr" },
    { document_element::tag::track       ,"track" },
    { document_element::tag::u           ,"u" },
    { document_element::tag::ul          ,"ul" },
    { document_element::tag::var         ,"var" },
    { document_element::tag::video       ,"video" },
    { document_element::tag::wbr         ,"wbr"},
    { document_element::tag::nil         ,"null" }
};


/*!

    @brief Appends a attr_vat_t pair into the attributes list of the element.

    @code de.push_attr({"class","ui button raised"});

    @author &copy; Serge Lussier (Lonesome Coder), lussier.serge@gmail.com
*/

std::size_t  document_element::push_attr(document_element::attr_val_t&& a)
{
    _attributes.push_back(std::move(a));
    return _attributes.size();
}


std::string  document_element::begin()
{
    return "implement";
}


std::string  document_element::end()
{
    return "implement";
}

document_element::~document_element()
{
    std::cout << __PRETTY_FUNCTION__ << text() << " -  \\O/\n";
}

document_element::document_element(document_element::shared a_parent, document_element::tag a_tag): _tag(a_tag), _parent(a_parent) //_parent(std::make_shared<document_element>(a_parent))
{
    _tagstr = document_element::tagname(_tag);
}

document_element::document_element(document_element::tag a_tag) : _tag(a_tag)
{
    _tagstr = document_element::tagname(_tag);
}

document_element& document_element::parent()
{
    if (!_parent) return document_element::null;
    return *_parent;
}

utils::xstr document_element::tagname(document_element::tag a_tag)
{
    return tags[a_tag];
}


document_element::operator bool()
{
    return this != &document_element::null;
}


std::string  document_element::text()
{
    _stream = "<";
    _stream << _tagstr << ">";

    return _stream();
}

}

