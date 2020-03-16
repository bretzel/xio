#include <xio/logbook/book.h>




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

doc_element& book::element()
{
    return doc_element::null;
    // TODO: insérer une instruction return ici
}


doc_element::shared book::create_element(doc_element::shared a_parent, doc_element::tag tg)
{
    return std::make_shared<doc_element>(a_parent.get(), tg);
}


}
