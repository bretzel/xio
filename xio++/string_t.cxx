#include "string_t.hpp"
#include <xio++/journal/logger.hpp>



using std::string;
using std::stringstream;
using std::swap;




//     _class_id(string_t)



string xio_api string_t::default_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";

static  string __nullstr__ = "";

std::ostream&  operator<<(std::ostream& in, const string_t& _s)
{
    in << _s();
    return in;
}


string_t::string_t()
{

}

string_t::string_t(const char* a_str)
{
    //SOut << __PRETTY_FUNCTION__ << ":[\"" << a_str << "\"]:\n";
    _s = a_str;
}

string_t::string_t(std::string && a_str)
{
    std::swap(_s, a_str);
    a_str.clear();
}




string_t::string_t(const string& a_str)
{
    //ffnl << a_str << "\n";
    _s = a_str;
}


string_t::string_t(const string_t& a_str)
{
    //ffnl << a_str._str << "\n";
    _s = a_str._s;
}

string_t::string_t(string_t && a_str)
{
    //ffnl << a_str._str << "\n";
    std::swap(_s, a_str._s);
}


string_t::~string_t()
{
    //ffnl << _str << "\n";
    _s.clear();
}



string_t & string_t::operator=(const string_t& a_str)
{
    //ffnl << a_str._str << "\n";
    _s = a_str._s;
    _argpos = 0;
    return *this;
}

string_t & string_t::operator=(string_t && a_str)
{
    //ffnl << a_str._str << "\n";
    std::swap(_s, a_str._s);
    _argpos = 0;
    return *this;
}


bool string_t::operator==(const string_t& a_str) const
{

    return _s == a_str._s;

}

string_t & string_t::operator=(string && a_str)
{
    //ffnl << a_str << "\n";
    swap(_s, a_str);
    _argpos = 0;
    return *this;
}

string_t & string_t::operator << (char c)
{

    if (scanarg() == std::string::npos) {
        _s += c;
        return *this;
    }
    return printf<char>(c);

}

string_t& string_t::operator=(const char* a_str)
{
    if (a_str)
        _s = a_str;
    else
        _s = "";
    _argpos = 0;
    return *this;
}


string_t & string_t::operator << (const string& a_str)
{
    //_scan_next_Arg();
    if (scanarg() == std::string::npos) {
        _s += a_str;
        return *this;
    }
    return printf<const std::string&>(a_str);
}

string_t & string_t::operator << (const string_t& a_str)
{
    //_scan_next_Arg();
    if (scanarg() == std::string::npos) {
        std::ostringstream os;
        os.precision(_precision);
        os << a_str();
        _s += os.str();
        return *this;
    }
    return printf<const std::string&>(a_str());
}

string_t & string_t::operator << (const char* a_str)
{
    //_scan_next_Arg();
    if (scanarg() == std::string::npos) {
        std::ostringstream os;
        os.precision(_precision);
        os << a_str;
        _s += os.str();
        return *this;
    }
    return printf<const char*>(a_str);
}


string_t& string_t::operator += (const string_t& a_atr)
{
    _s += a_atr._s;
    return *this;
}
string_t& string_t::operator += (const string& a_atr)
{
    _s += a_atr;
    return *this;

}

string_t & string_t::operator+=(char c)
{
    _s += c;
    return *this;
}



string_t& string_t::operator + (char c)
{
    _s += c;
    return *this;
}

string_t& string_t::operator + (const string_t& a_atr)
{
    _s += a_atr._s;
    return *this;
}
string_t& string_t::operator + (const string& a_atr)
{
    _s += a_atr;
    return *this;
}




string_t & string_t::operator>>(string& _Arg)
{
    _Arg = _s;

    return *this;
}


void string_t::clear()
{
    _s.clear();
    _argpos = (string::size_type)0;
}

std::string string_t::datetime(const std::string & str_fmt)
{
    time_t rawtime;
    struct tm * timeinfo;
    char tmb[180];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(tmb, 60, str_fmt.c_str(), timeinfo);
    std::string _s = tmb;
    return  tmb;
}


string::size_type string_t::scanarg()
{
    _argpos = _s.find('%', 0);
    return _argpos;
}

void string_t::putarg(const string& a_str)
{
    if (scanarg() == std::string::npos) {
        _s.append(a_str);
        return;
    }

    printf<const string&>(a_str);
}


// Ce qui fait royalement chier avec les iterateurs des stl, c'est que depuis l'iterateur, comment on accede � son conteneur ???????
bool string_t::skipws(string::iterator& pos)
{
    if (pos == _s.end()) // aucun moyen de savoir si l'it�rateur en est un de notre conteneur "_str" !!
        return false;
    while (isspace(*pos)) ++pos;
    return true;
}

// Ce qui fait royalement chier avec les iterateurs des stl, c'est que depuis l'iterateur, comment on accede � son conteneur ???????
bool string_t::skipws(const char* pos)
{
    if (!pos)
        return false;
    while (pos && (*pos && isspace(*pos))) ++pos;
    return true;
}



string string_t::word::operator()()
{
    string _s;
    if (begin == end)
        _s.insert(_s.begin(), begin, end + 1);
    else
        _s.insert(_s.begin(), begin, end + 1);

    return _s;
}

string string_t::cword::operator()()
{
    string _s;
    if (begin == end)
        _s.insert(_s.begin(), begin, end + 1);
    else
        _s.insert(_s.begin(), begin, end);

    return _s;
}

string::iterator string_t::scanto(string::iterator start, char c)
{
    string::iterator p = start;
    ++p;
    while ((p != _s.end()) && (*p != c)) ++p;
    return p;
}

const char* string_t::scanto(const char* start, char c)
{
    const char* p = start;
    if (!p) return nullptr;
    ++p;
    while (p && (*p != c)) ++p;
    return p;
}


bool string_t::pushword(word::list& strm, string_t::word& w, string::size_type sz)
{
    strm.push_back({ w.begin, w.begin + sz });
    //fnl << cwhite " pushed:`" cyellow << strm.back()() << cwhite "` w.end on `" cyellow << *w.end << "\n";
    w.begin = w.end;
    return w.end != _s.end();
}




/*!
    * @brief break/split/tokenize,etc... the content of this string_t into pieces.
    * @param warray  OUTPUT reference to the 'words array' containter, filled by this method.
    * @param a_delimiters separators in the form of a string of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the separators will be put into the list as they appear
    * @return number of "words/tokens" contained into the warray.
    * @notice : After several years of experience and experimentations, I have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c a_delimiters.
    */
std::size_t string_t::words(string_t::word::list& warray, const std::string& a_delimiters, bool keep_as_word)
{
    if (_s.empty()) {
        std::cout << " --> Contents is empty!";
        return (std::size_t)0;
    }
    std::string token_separators = a_delimiters.empty() ? string_t::default_separators : a_delimiters;
    //LFnl << ":\n  token_separators:[" << token_separators << "\"]:\n";

    word w;
    w.begin = w.end = _s.begin();
    //string buffer;// temporary buffer.
    skipws(w.begin);
    w.end = w.begin;
    //buffer += *w.begin;

    while (w.end != _s.end()) {

        std::string::iterator cc = w.end;
        if (isspace(*w.end)) {
            if (w.begin < w.end) {
                warray.push_back({ w.begin, (w.end++) - 1 });
                //warray.push_back({ w.begin, w.end++ });
                w.begin = w.end;
            }
            skipws(w.begin);
            w.end = w.begin;
            continue;
        }

        if (token_separators.find(*w.end) != string::npos) {
            cc = w.end;
            if (cc > w.begin) {
                --cc;
                warray.push_back({ w.begin, cc });
                
                w.begin = cc = w.end;
            }
            if (keep_as_word) {
                if ((*cc == '/') && (*(cc + 1) == '/')) {
                    ++cc; ++w.end;
                }// '//' as one token instead of having two consecutive '/' 
                warray.push_back({ w.begin,cc });
            }

            ++w.end;
            w.begin = w.end;
        }
        else
            if ((*cc == '\'') || (*cc == '"')) {
                w.begin = cc;
                ++w.begin;
                string::iterator p = scanto(w.begin, *cc);
                if (p != _s.end()) {
                    warray.push_back({ w.begin, p - 1 });
                    if (keep_as_word) warray.push_back({ p,p });
                    w.end = p;
                    ++w.end;
                    logdebugfn << " word: '" << warray.back()() << Ends;
                }
                else return warray.size();
            }
            else
            ++w.end;
    }
    if (w.end > w.begin)
        warray.push_back({ w.begin,w.end-1 });

    return warray.size();


}


/*
 *
 * if (_s.empty()) {
        std::cout << " --> Contents is empty!";
        return (std::size_t)0;
    }
    std::string token_separators = a_delimiters.empty() ? string_t::default_separators : a_delimiters;
    //LFnl << ":\n  token_separators:[" << token_separators << "\"]:\n";

    word w;
    w.begin = w.end = _s.begin();
    //string buffer;// temporary buffer.
    skipws(w.begin);
    w.end = w.begin;
    //buffer += *w.begin;
    while (w.end != _s.end()) {


        if (isspace(*w.end)) {
            if (w.begin < w.end) {
                warray.push_back({ w.begin, (w.end++) - 1 });
                w.begin = w.end;
            }
            skipws(w.begin);
            w.end = w.begin;
            continue;
        }

        if (token_separators.find(*w.end) != string::npos) {
            std::string::iterator cc = w.end;
            if (cc > w.begin) {
                --cc;
                warray.push_back({ w.begin, cc });

                w.begin = cc = w.end;
            }
            if (keep_as_word) {
                if ((*cc == '/') && (*(cc + 1) == '/')) {
                    ++cc; ++w.end;
                }// '//' as one token instead of having two consecutive '/'
                warray.push_back({ w.begin,cc });
            }
            if ((*cc == '\'') || (*cc == '"')) {
                w.begin = cc;
                ++w.begin;
                string::iterator p = scanto(w.begin, *cc);
                if (p != _s.end()) {
                    warray.push_back({ w.begin, p - 1 });
                    if (keep_as_word) warray.push_back({ p,p });
                    w.end = p;
                }
                else return warray.size();

            }
            ++w.end;
            w.begin = w.end;
        }
        else ++w.end;
    }
    if (w.end > w.begin)
        warray.push_back({ w.begin,w.end-1 });

 *
 *
 *
 */
// msef::code string_t::for_each(string_t::cword::list & wl, msef::code(*pf)(const std::string &))
// {
//     if (!pf) return msef::code::undefined;
//     if (wl.empty())
//         return msef::code::empty;
// 
//     for (auto str : wl)
//         if (msef::code c; (c = pf(str())) != msef::code::accepted) return c;
//     return msef::code::accepted;
// }
// 
// msef::code string_t::for_each(string_t::word::list & wl, msef::code(*pf)(const std::string &))
// {
//     if (!pf) return msef::code::undefined;
//     if (wl.empty())
//         return msef::code::empty;
// 
//     for (auto str : wl)
//         if (msef::code c; (c = pf(str())) != msef::code::accepted) return c;
//     return msef::code::accepted;
// }


bool string_t::filter(const string_t::list& a_exp)
{
    if (!a_exp.size()) return true;
    string_t::citerator i = a_exp.cbegin();

    // array::front is the start of the match at the beginning of this string.
    // if ...front() is empty, then there is no required match at the beginning of this string
    // so proceed to the next substring to find.
    // if the last is not empty, then match the last substring.
    std::string::size_type pos = 0;
    if (!(*i).empty()) {
        //lfnl << cwhite << "must begins with [" << cyellow << (*i) << cwhite << "]:\n";
        pos = _s.find((*i));
        if (pos != 0) // no match
            return false;
    }
    else;
    //fnl << cwhite << "no requirement for the beginning\n";
    ++i;
    citerator end = a_exp.cend();
    --end;
    if (!(*i).empty()) {
        while (i < end) {
            if ((*i).empty()) {
                ++i;
                continue;
            }
            pos = _s.find((*i), pos);
            if (pos != std::string::npos) {
                ++pos;
                ++i;
                continue;
            }
            return false;
        }
    }
    if (!(*end).empty()) {
        std::size_t sz = (*end).size();
        pos = _s.find((*end), pos);
        if (pos != std::string::npos) {
            if ((pos + sz) != _s.size()) return false;
        }
    }
    return true;
}




std::string string_t::str_size(uint64_t sz)
{
    float fsize = sz;
    string_t us = "";
    int currentunit = 0;
    string_t units[] = { "b", "kb", "mb", "gb", "tb" };
    int unitssize = sizeof(units) / sizeof(units[0]);

    while (fsize > 1024.0 && currentunit < unitssize - 1) {
        fsize /= 1024.0;
        currentunit++;
    }

    us = std::fixed;
    us += std::setprecision(2);
    us += fsize;
    us += " ";
    us += units[currentunit];

    return us;
}




xio_api  std::string string_t::format_t::operator()()
{
    std::ostringstream str;
    str << "\nFlag    :" << _f << "\n";
    str << "Width     :" << (int)_w << "\n";
    str << "Precision :" << (int)_p << "\n";
    str << "delta     :" << (int)_delta << "\n";
    str << "spec      :" << (char)_s << "\n";
    return str.str();
}


