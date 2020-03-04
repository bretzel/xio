#include <xio/logbook/document_element.h>




namespace xio::logbook
{

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

std::string  document_element::text()
{
    return "implement";
}

}
