//
// Created by bretzelus on 18-11-27.
//

#include "journal.hpp"
namespace xio {





    w3c::NoW3CSSMap Journal::_W3cMetro = {
         {W3CMetro::w3cMetroLightGreen,   {"w3-metro-light-green"  ,"#99b433"}},
         {W3CMetro::w3cMetroBreen,        {"w3-metro-green"        ,"#00a300"}},
         {W3CMetro::w3cMetroDarkGreen,    {"w3-metro-dark-green"   ,"#1e7145"}},
         {W3CMetro::w3cMetroMagenta,      {"w3-metro-magenta"      ,"#ff0097"}},
         {W3CMetro::w3cMetroLightPurple,  {"w3-metro-light-purple" ,"#9f00a7"}},
         {W3CMetro::w3cMetroPurple,       {"w3-metro-purple"       ,"#7e3878"}},
         {W3CMetro::w3cMetroDarkPurple,   {"w3-metro-dark-purple"  ,"#603cba"}},
         {W3CMetro::w3cMetroDarken,       {"w3-metro-darken"       ,"#1d1d1d"}},
         {W3CMetro::w3cMetroTeal,         {"w3-metro-teal"         ,"#00aba9"}},
         {W3CMetro::w3cMetroLightBlue,    {"w3-metro-light-blue"   ,"#eff4ff"}},
         {W3CMetro::w3cMetroBlue,         {"w3-metro-blue"         ,"#2d89ef"}},
         {W3CMetro::w3cMetroDarkBlue,     {"w3-metro-dark-blue"    ,"#2b5797"}},
         {W3CMetro::w3cMetroYellow,       {"w3-metro-yellow"       ,"#ffc40d"}},
         {W3CMetro::w3cMetroOrange,       {"w3-metro-orange"       ,"#e3a21a"}},
         {W3CMetro::w3cMetroDarkOrange,   {"w3-metro-dark-orange"  ,"#da532c"}},
         {W3CMetro::w3cMetroRed,          {"w3-metro-red"          ,"#ee1111"}},
         {W3CMetro::w3cMetroDarkRed,      {"w3-metro-dark-red"     ,"#b91d47"}}
    };


   
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
    }

    std::string Journal::Log::operator[](W3CMetro K)
    {
        std::string left = "<span class=\"";
        return left + Journal::_W3cMetro[K].first.data() + ";\">";
        
    }


    std::string_view Journal::operator[](W3CMetro K)
    {
        return Journal::_W3cMetro[K].first;
    }

}

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
