//
// Created by bretzelus on 18-11-27.
//

#include "journal.hpp"
namespace xio {

   
std::map<TextAttr, std::string_view> xio::Journal::Log::_AttrText={
    {TextAttr::Code,    "code"},
    {TextAttr::Italic,  "italic"}
};


Journal::Log::~Log()
{
    mText.clear();
    std::cout << __PRETTY_FUNCTION__ << "...\n";
}

Journal::Log& Journal::Log::operator<<(TextAttr A)
{
    switch(A){
        case TextAttr::End:
        {
            TextAttr At = PopAttr();
            if(At != TextAttr::Nan)
                mText << "</" << Log::_AttrText[At] << '>';
        }
            break;
        default:
            PushAttr(A);
            mText << '<' << Log::_AttrText[A] << '>';
            break;
    }
    std::cout << __FUNCTION__ << ':' << mText() << '\n';
    // TODO: ins�rer une instruction return ici
    return *this;
}}

xio::TextAttr xio::Journal::Log::PopAttr()
{
    if(mAttrStack.empty())
        return TextAttr::Nan;
    TextAttr Att = mAttrStack.top();
    mAttrStack.pop();
    return Att;
}

void xio::Journal::Log::PushAttr(TextAttr AAttr)
{
    mAttrStack.push(AAttr);
}


xio::Journal::Log& operator^(xio::Journal::Log& l, xio::TextAttr t)
{
    //l.PopAttr(t);
    std::cout << " pop attr quelque chose...";
    return l;

}
