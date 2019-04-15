//
// Created by bretzelus on 18-11-27.
//

#include "journal.hpp"
namespace xio {


    Journal* Journal::_Instance = nullptr;


    w3c::W3ClassMap Journal::_W3cMetro = {
         {W3CMetro::/*w3cMetro*/LightGreen,   {"w3-metro-light-green"  ,"#99b433"}},
         {W3CMetro::/*w3cMetro*/Green,        {"w3-metro-green"        ,"#00a300"}},
         {W3CMetro::/*w3cMetro*/DarkGreen,    {"w3-metro-dark-green"   ,"#1e7145"}},
         {W3CMetro::/*w3cMetro*/Magenta,      {"w3-metro-magenta"      ,"#ff0097"}},
         {W3CMetro::/*w3cMetro*/LightPurple,  {"w3-metro-light-purple" ,"#9f00a7"}},
         {W3CMetro::/*w3cMetro*/Purple,       {"w3-metro-purple"       ,"#7e3878"}},
         {W3CMetro::/*w3cMetro*/DarkPurple,   {"w3-metro-dark-purple"  ,"#603cba"}},
         {W3CMetro::/*w3cMetro*/Darken,       {"w3-metro-darken"       ,"#1d1d1d"}},
         {W3CMetro::/*w3cMetro*/Teal,         {"w3-metro-teal"         ,"#00aba9"}},
         {W3CMetro::/*w3cMetro*/LightBlue,    {"w3-metro-light-blue"   ,"#eff4ff"}},
         {W3CMetro::/*w3cMetro*/Blue,         {"w3-metro-blue"         ,"#2d89ef"}},
         {W3CMetro::/*w3cMetro*/DarkBlue,     {"w3-metro-dark-blue"    ,"#2b5797"}},
         {W3CMetro::/*w3cMetro*/Yellow,       {"w3-metro-yellow"       ,"#ffc40d"}},
         {W3CMetro::/*w3cMetro*/Orange,       {"w3-metro-orange"       ,"#e3a21a"}},
         {W3CMetro::/*w3cMetro*/DarkOrange,   {"w3-metro-dark-orange"  ,"#da532c"}},
         {W3CMetro::/*w3cMetro*/Red,          {"w3-metro-red"          ,"#ee1111"}},
         {W3CMetro::/*w3cMetro*/DarkRed,      {"w3-metro-dark-red"     ,"#b91d47"}}
    };


   
    std::map<TextAttr, std::string_view> xio::Journal::Log::_AttrText={
        {TextAttr::Code,    "code"},
        {TextAttr::Italic,  "italic"}
    };


    Journal::Log::~Log()
    {
        ///@todo Commit internal buffer `mText` here.

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


    Journal::Journal()
    {
        if (Journal::_Instance) return;
        Journal::_Instance = this;
        mLoggers["/Book/Logs"] = Log();
    }

    Journal::~Journal()
    {
        std::cout << __FUNCTION__ << ":Closing, clear loggers...";
        mLoggers.clear();
    }

    std::string_view Journal::operator[](W3CMetro K)
    {
        return Journal::_W3cMetro[K].first;
    }

    Journal::Log& Journal::operator[](const std::string& APath)
    {
        return mLoggers[APath];
    }

    Journal& Journal::Instance()
    {
        if (!Journal::_Instance)
            Journal::_Instance  = new Journal();
        return *Journal::_Instance;
    }

    Journal& Journal::Init()
    {
        if (!Journal::_Instance)
            throw "Oops! Journal::Init called before its constructor (use Journal::Instance() to Instanciate the Journal)";
        /// ... 
        return *Journal::_Instance;
    }

    int Journal::Close()
    {
        if (Journal::_Instance)
        {
            delete Journal::_Instance;
            Journal::_Instance = nullptr;
        }
        return 0;
    }

    Journal::Book::Logs::~Logs()
    {
    }

    TextAttr Journal::Log::PopAttr()
    {
        if (mAttrStack.empty())
            return TextAttr::Nan;
        TextAttr Att = mAttrStack.top();
        mAttrStack.pop();
        return Att;
    }

    void Journal::Log::PushAttr(TextAttr AAttr)
    {
        mAttrStack.push(AAttr);
    }

}
