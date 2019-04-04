//
// Created by bretzelus on 18-11-27.
//

#include "journal.hpp"
namespace xio {

   

    Journal::Log::~Log()
    {
        mText.clear();
        std::cout << __PRETTY_FUNCTION__ << "...\n";
    }

    Journal::Log& Journal::Log::operator<<(TextAttr A)
    {
        std::cout << __PRETTY_FUNCTION__ << static_cast<int>(A) << '\n';
        // TODO: insérer une instruction return ici
        return *this;
    }
}



xio::Journal::Log& operator^(xio::Journal::Log& l, xio::TextAttr t)
{
    //l.PopAttr(t);
    std::cout << " pop attr quelque chose...";
    return l;

}