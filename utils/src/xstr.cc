//
// Created by lsc on 20-02-26.
//

#include <xio/utils/xstr.h>



namespace teacc::utils
{

using std::string;


string   XIOUTILS_API xstr::default_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";

string   XIOUTILS_API xstr::__nullstr__ = "";

std::ostream& operator<<(std::ostream& in, const xstr& _s)
{
    in << _s();
    return in;
}


xstr::xstr()
{
    // _s has been instanciated here as well...
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.cend();
}


xstr::xstr(const char* a_str)
{
    //SOut << __PRETTY_FUNCTION__ << ":[\"" << a_str << "\"]:\n";
    _s = a_str;
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.cend();
}

xstr::xstr(std::string&& a_str)
{
    std::swap(_s, a_str);
    a_str.clear();
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.cend();
    
}




xstr::xstr(const string& a_str)
{
    //ffnl << a_str << "\n";
    _s = a_str;
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.cend();
    
}


xstr::xstr(const xstr& a_str)
{
    //ffnl << a_str._str << "\n";
    _s = a_str._s;
    _cursor = a_str._cursor;
}

xstr::xstr(xstr&& a_str) noexcept
{
    //ffnl << a_str._str << "\n";
    std::swap(_s, a_str._s);
    _cursor = std::move(a_str._cursor);
}


xstr::~xstr()
{
    //ffnl << _str << "\n";
    _s.clear();
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.cend();
    
}



//xstr& xstr::operator=(const xstr& a_str)
//{
//    //ffnl << a_str._str << "\n";
//    _s = a_str._s;
//    _argpos = a_str._argpos;
//    _precision = a_str._precision;
//    return *this;
//}

xstr& xstr::operator=(xstr&& a_str) noexcept
{
    //ffnl << a_str._str << "\n";
    std::swap(_s, a_str._s);
    _argpos = 0;
    _cursor = std::move(a_str._cursor);
    return *this;
}


bool xstr::operator==(const xstr& a_str) const
{
    
    return _s == a_str._s;
    
}

xstr& xstr::operator=(string&& a_str)
{
    //ffnl << a_str << "\n";
    swap(_s, a_str);
    _argpos = 0;
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = --_s.end();
    
    return *this;
}

xstr& xstr::operator << (char c)
{
    
    if (scanarg() == std::string::npos) {
        _s += c;
        return *this;
    }
    return printf<char>(c);
    
}

xstr& xstr::operator=(const char* a_str)
{
    if (a_str)
        _s = a_str;
    else
        _s = "";
    _argpos = 0;
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.end();
    
    return *this;
}


xstr& xstr::operator << (const string& a_str)
{
    //_scan_next_Arg();
    if (scanarg() == std::string::npos) {
        _s += a_str;
        return *this;
    }
    return printf<const std::string&>(a_str);
}

xstr::collection xstr::to_args(int argc, char** argv)
{
    collection args;
    for (int x = 0; x < argc; x++)
    {
        args.push_back(argv[x]);
        
    }
    return args;
}

xstr& xstr::operator << (const xstr& a_str)
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

xstr& xstr::operator << (const char* a_str)
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


xstr& xstr::operator += (const xstr& a_atr)
{
    _s += a_atr._s;
    return *this;
}
xstr& xstr::operator += (const string& a_atr)
{
    _s += a_atr;
    return *this;
    
}

xstr& xstr::operator+=(char c)
{
    _s += c;
    return *this;
}



xstr& xstr::operator + (char c)
{
    _s += c;
    return *this;
}

xstr& xstr::operator + (const xstr& a_atr)
{
    _s += a_atr._s;
    return *this;
}
xstr& xstr::operator + (const string& a_atr)
{
    _s += a_atr;
    return *this;
}




xstr& xstr::operator>>(string& _Arg)
{
    _Arg = _s;
    return *this;
}

std::string xstr::makestr(const char* B, const char* E)
{
    std::string Str;
    const char* C = B;
    if ( (!B) || (!E) || (!*B) || (!*E)) return Str;
    while (C <= E) Str += *C++;
    
    return Str;
}


void xstr::clear()
{
    _s.clear();
    _argpos = (string::size_type)0;
    _cursor.b = _cursor.c = _s.cbegin();
    _cursor.e = _s.end();
    
}

std::string xstr::datetime(const std::string& str_fmt)
{
    time_t rawtime;
    struct tm* timeinfo;
    char tmb[180];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(tmb, 60, str_fmt.c_str(), timeinfo);
    std::string _s = tmb;
    return  tmb;
}


string::size_type xstr::scanarg()
{
    _argpos = _s.find('%', 0);
    return _argpos;
}

void xstr::putarg(const string& a_str)
{
    if (scanarg() == std::string::npos) {
        _s.append(a_str);
        return;
    }
    
    printf<const string&>(a_str);
}


// Ce qui fait royalement chier avec les iterateurs des stl, c'est que depuis l'iterateur, comment on accede � son conteneur ???????
bool xstr::skipws(string::iterator& pos)
{
    if ( _cursor.c == _cursor.e )
        return false;
    
    if (pos == _s.end()) // aucun moyen de savoir si l'it�rateur en est un de notre conteneur "_str" !!
        return false;
    while (isspace(*pos)) ++pos;
    return true;
}

// Ce qui fait royalement chier avec les iterateurs des stl, c'est que depuis l'iterateur, comment on accede � son conteneur ???????
bool xstr::skipws(const char* pos)
{
    if (!pos)
        return false;
    while (pos && (*pos && isspace(*pos))) ++pos;
    return true;
}



string xstr::word::operator()()
{
    string _s;
    if (B == E)
        _s.insert(_s.begin(), B, E + 1);
    else
        _s.insert(_s.begin(), B, E + 1);
    
    return _s;
}


std::string xstr::word::operator *()
{
    string _s;
    if ( B == E )
        _s.insert(_s.begin(), B, E + 1);
    else
        _s.insert(_s.begin(), B, E + 1);
    
    return _s;
    
}

std::string xstr::word::mark()
{
    xstr str;
    std::string::const_iterator start = B - pos;
    
    int l = 1;
    std::string::const_iterator cc = B;
    // localiser le debut de la ligne;
    while ( *cc && (cc > start) && (*cc != '\n') && (*cc != '\r') )
        --cc;
    // debut de la ligne ou de la source:
    if ( cc >= start )
    {
        if ( (*cc == '\n') || (*cc == '\r') )
            ++cc;
        while ( (cc != SE) && (*cc != '\n') && (*cc != '\r') )
            str += *cc++;
    }
    xstr tstr;
    
    tstr << str << '\n';
    for ( int x = 1; x < c; x++ )
        tstr << ' ';
    tstr << '^';
    return tstr();
}


void xstr::word::operator++()
{

}

void xstr::word::operator++(int)
{
}

std::string xstr::word::location()
{
    xstr str = "(%d,%d)";
    str << l << c;
    return str();
}


string::const_iterator xstr::scanto(string::const_iterator start, char c)
{
    string::const_iterator p = start;
    ++p;
    while ((p != _s.end()) && (*p != c)) ++p;
    return p;
}

const char* xstr::scanto(const char* start, char c)
{
    const char* p = start;
    if (!p) return nullptr;
    ++p;
    while (p && (*p != c)) ++p;
    return p;
}





/*!
    * @brief break/split/tokenize,etc... the content of this xstr into pieces.
    * @param wcollection  OUTPUT reference to the 'words array' containter, filled by this method.
    * @param a_delimiters separators in the form of a string of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the separators will be put into the list as they appear
    * @return number of "words/tokens" contained into the wcollection.
    * @notice : After several years of experience and experimentations, I have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c a_delimiters.
    */
std::size_t xstr::words(xstr::word::collection& wcollection, const std::string& a_delimiters, bool keep_as_word)
{
    //std::cout << __PRETTY_FUNCTION__ << ":\n";// << _s << "\n:\n";
    if (_s.empty()) {
        std::cout << " --> Contents is empty!";
        return (std::size_t)0;
    }
    _cursor.reset(_s);
    std::string token_separators = a_delimiters.empty() ? xstr::default_separators : a_delimiters;
    //std::cout << " contents after bce::reset\n ------------- \n" << _s << "\n---------------\n: [\n" << *_cursor.c << "\n]\n";
    if ( !_cursor.skip() )
    {    
        //std::cout << " --> Contents skip is false? (internal?)...\n";
        return (std::size_t)0;
    }
    word w;
    _cursor >> w;
    
    while (!_cursor.end()) {
        if ( !wcollection.empty() );
        //std::cout << __FUNCTION__ << " last inserted word: [" << wcollection.back()() << "] - _cursor on [" << *_cursor.c << "]\n";
        std::string::const_iterator cc = _cursor.c;
        if (token_separators.find(*_cursor.c) != string::npos)
        {
            cc = _cursor.c;
            //std::cout << __FUNCTION__ << " Delimiter:[" << *cc << "]\n";
            if (cc > w.B) {
                --cc;
                wcollection.push_back({w.B, cc, _cursor.e, w.l, w.c, w.pos });
                //std::cout << __FUNCTION__ << ": pushed lhs word:[" << wcollection.back()() << "];\n";
                
                _cursor >> w;
                cc = _cursor.c;
            }
            
            // '//' as one token instead of having two consecutive '/'
            if ( (*_cursor.c == '/') && (*(_cursor.c + 1) == '/') ) ++_cursor;
            
            if (keep_as_word)
            {
                wcollection.push_back({w.B, _cursor.c, _cursor.e, w.l, w.c, w.pos });
                //std::cout << __FUNCTION__ << ": pushed new token:[" << wcollection.back()() << "];\n";
            }
            ++_cursor;
            //std::cout << "        cursor eos: " << _cursor.end() << "\n";
            if ( !_cursor.end() ) _cursor >> w;
            else
            {
                //std::cout << __FUNCTION__ << " - EOS: wcollection size: " << wcollection.size() << " = leaving. Done!\n";
                return wcollection.size();
            }
            
            //std::cout << " is eos?" << _cursor.end() << "\n";
        }
        else if ((*_cursor.c == '\'') || (*_cursor.c == '"'))
        { // Quoted litteral string...
            _cursor >> w;
            //std::cout << __FUNCTION__ << " In quoted litteral:\n";
            if (keep_as_word)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the word coords anyway.
                //std::cout << __FUNCTION__ << " Creating three parts quoted tokens: \n";
                wcollection.push_back({w.B, w.B, _cursor.e, w.l, w.c, w.pos });
                //std::cout << __FUNCTION__ << " 1:[" << wcollection.back()() << "]; ";
            }
            
            string::const_iterator p = scanto(w.B + (keep_as_word? 0:1), *_cursor.c); // w.B is the starting position, _cursor.c is the quote delim.
            while ( _cursor.c < p ) ++_cursor; // compute white spaces!!!
            
            if ( keep_as_word )
            {
                // then push the litteral that is inside the quotes.
                wcollection.push_back({w.B + 1, p - 1, _cursor.e, w.l, w.c, w.pos });
                //std::cout << " 2:[" << wcollection.back()() << "]; ";
                //++_cursor; // _cursor now on the closing quote
                _cursor >> w; // Litteral is done, update w.
                wcollection.push_back({w.B, p, _cursor.e, w.l, w.c, w.pos });
                //std::cout << " 3:[" << wcollection.back()() << "]\n";
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the word.
                //std::cout << __FUNCTION__ << " Creating single part quoted token: \n";
                wcollection.push_back({w.B, _cursor.c, _cursor.e, w.l, w.c, w.pos });
                //std::cout << " :[" << wcollection.back()() << "]\n";
            }
            if ( ++_cursor ) _cursor >> w;
            else return wcollection.size();
            
        }
        else
        {
            cc = _cursor.c;
            ++cc;
            if ( cc == _cursor.e )
            {
                ++_cursor.c;
                break;
            }
            //std::cout << __FUNCTION__ << " check whitespace :[" << *cc << "]\n";
            if ( isspace(*cc) )
            {
                if ( w.B < cc )
                {
                    wcollection.push_back({w.B, cc - 1, _cursor.e, w.l, w.c, w.pos });
                    ++_cursor;
                }
                
                if ( _cursor.skip() )
                {
                    _cursor >> w;
                    continue;
                }
                return wcollection.size();
            }
            if(!_cursor.end() )
                ++_cursor; // advance offset to the next separator/white space.
        }
    }
    if (_cursor.c > w.B)
        wcollection.push_back({w.B, _cursor.c - 1, _cursor.e, w.l, w.c, w.pos });
    
    return wcollection.size();
    
    
}


/*


std::size_t xstr::words(xstr::word::collection& warray, const std::string& a_delimiters, bool keep_as_word)
{
    if (_s.empty()) {
        //std::cout << " --> Contents is empty!";
        return (std::size_t)0;
    }
    std::string token_separators = a_delimiters.empty() ? xstr::default_separators : a_delimiters;
    //LFnl << ":\n  token_separators:[" << token_separators << "\"]:\n";

    word w;
    w.B = w.E = _s.begin();
    //string buffer;// temporary buffer.
    skipws(w.B);
    w.E = w.B;
    //buffer += *w.begin;

    while (w.E != _s.end()) {

        std::string::iterator cc = w.E;
        if (isspace(*w.E)) {
            if (w.B < w.E) {
                warray.push_back({ w.B, (w.E++) - 1 });
                //warray.push_back({ w.begin, w.end++ });
                w.B = w.E;
            }
            skipws(w.B);
            w.E = w.B;
            continue;
        }

        if (token_separators.find(*w.E) != string::npos) {
            cc = w.E;
            if (cc > w.B) {
                --cc;
                warray.;push_back({ w.B, cc });

                w.B = cc = w.E;
            }
            if (keep_as_word) {
                if ((*cc == '/') && (*(cc + 1) == '/')) {
                    ++cc; ++w.E;
                }// '//' as one token instead of having two consecutive '/'
                warray.push_back({ w.B,cc });
            }

            ++w.E;
            w.B = w.E;
        }
        else
        if ((*cc == '\'') || (*cc == '"')) {
            w.B = cc;
            if (keep_as_word)
                warray.push_back({ cc,cc });

            ++w.B;
            string::iterator p = scanto(w.B, *cc);
            if (p != _s.end()) {
                warray.push_back({ w.B, p - 1 });
                if (keep_as_word) warray.push_back({ p,p });
                w.E = p;
                ++w.E;
                w.B = w.E;
                //logdebugfn << " word: '" << warray.back()() << Ends;
            }
            else return warray.size();
        }
        else
            ++w.E;
    }
    if (w.E > w.B)
        warray.push_back({ w.B,w.E - 1 });

    return warray.size();


}











*/

int xstr::filter(const xstr::collection& a_exp)
{
    if (!a_exp.size()) return 0;
    auto i = a_exp.cbegin();
    
    // array::front is the init of the match at the beginning of this string.
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
    auto end = a_exp.cend();
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




std::string xstr::sizef(uint64_t sz)
{
    float fsize = (float)sz;
    xstr us = "";
    int currentunit = 0;
    xstr units[] = { "b", "kb", "mb", "gb", "tb" };
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
std::string xstr::extract_substr(const std::string &first_lhs, const std::string &first_rhs)
{
    std::size_t lhs_pos = _s.find(first_lhs);
    if(lhs_pos == std::string::npos) return "";
    std::size_t rhs_pos = _s.find(first_rhs);
    if(rhs_pos == std::string::npos) return "";
    
    return _s.substr(lhs_pos, rhs_pos-lhs_pos);
}


/*!
 * @brief Try to extract the "template" type from the \c func_desc that normally shall be given by __PRETTY_FUNCTION__ at the call location.
 *
 * template parameter assumed to be "T" ...for now.
 *
 * @param func_desc
 *
 * @note This function has no control of the proper format and (function) prototype data.
 * @return the resulting type text
 */
std::string xstr::type_of_T(string &&func_desc)// , const std::string& _T) -> _T is the class to search.
{
    xstr text = std::move(func_desc);
    xstr::word::collection w;
    
    std::size_t count = text.words(w);
    for(auto s : w)
    {
        std::cout << s();
    }
    std::cout << '\n';
    return "finish to implement, please\n";
}


std::string xstr::prv_format::operator()()
{
    std::ostringstream str;
    str << "\nFlag    :" << _f << "\n";
    str << "Width     :" << (int)_w << "\n";
    str << "Precision :" << (int)_p << "\n";
    str << "delta     :" << (int)_delta << "\n";
    str << "spec      :" << (char)_s << "\n";
    return str.str();
}





xstr::_bce::_bce(const std::string& a_s)
{
    b = c = a_s.cbegin();
    e = a_s.cend();
}

bool xstr::_bce::skip()
{
    if(end())
        return false;
    
    while ( isspace(*c) )
    {
        switch ( *c )
        {
            case 10:
            {
                if ( (++c) >= e ) return false;
                if ( *c == 13 ) ++c;
                ++l;
                col = 1;
            }
                break;
            case 13:
            {
                if ( (++c) >= e ) return false;
                if ( *c == 10 ) ++c;
                ++l;
                col = 1;
            }
                break;
            case '\t':
                ++c; ++col;
                break;
            default:
                ++c; ++col;
                break;
        }
    }
    return c < e;
}

bool xstr::_bce::end()
{
    return c >= e;
}

bool xstr::_bce::operator++()
{
    if ( c >= e ) return false;
    ++c; ++col;
    if ( c >= e ) return false;
    return skip();
}

bool xstr::_bce::operator++(int)
{
    if ( c >= e ) return false;
    ++c; ++col;
    if ( c >= e ) return false;
    return skip();
}

xstr::_bce& xstr::_bce::operator>>(xstr::word& w)
{
    w.B = c;
    w.l = l;
    w.c = col;
    w.pos = i = (uint64_t)(c-b);
    return *this;
}



}
