//
// Created by steacc::utilssier on 19-11-20.
//

#include <xio/utils/journal.h>

#ifdef _WIN32

#include <Windows.h>
#else
#include <unistd.h>


#endif
#include <xio/utils/xchronos.h>

namespace teacc::utils
{

std::stack<int> journal::m_spanstack;

xstr journal::m_log_header = "";

wchar_t journal::m_arrows[8] = {L'➜', // append space char because the rendering cuts next adjacent char if we have no space between the unicode and the next char...
                                L'⬅', L'⬆', L'⬇', L'⬈', L'⬉', L'⬊', L'⬋'};

xstr              journal::m_filename = "";    ///< ...filename - empty if stdout is to be used.
std::ofstream       journal::m_file_output; ///< the instance of file.
std::ostream *journal::m_file = 0;        ///< pointer to the ofstream { stdout; or file stream}.
std::map<int32_t, std::string> journal::m_code_type_dictionary;

xstr              journal::m_tstamp = "";      ///< holds the configured date-time format for normal line ouputs.
bool                journal::m_stdout = false;      ///< if i am unable to find the way to assign stdout to m_file_output, a flag set to true...
bool                journal::m_code_mode = false;
bool                journal::m_closed = false;

xstr              journal::m_fn_prefix = "";   ///< saved text attributes/style for the function name { __function__ }.
xstr              journal::m_long_fn_prefix = ""; ///< saved text attributes/style for the long function name. { __pretty_function__ }.

journal::Mode        journal::m_mode = journal::Plain;

xstr::collection        journal::m_attr_table;  ///< text attributes/style list built from the mode;
xstr::collection       journal::m_type_table;  ///< message types list including their attributes/style.
xstr::collection       journal::m_objects_prefix_table;

std::string         journal::m_eol = "\n";  ///< line break for the mode.
int                 journal::m_indent = 0;

journal::journal(const journal &l)
{
    // -- break - no copy -- and copy what ?
    if(!journal::m_file) {
        journal::setfile("");
        journal::init(journal::Html, ":", false);
        journal::resetstamp();
        return;
    }
    if(journal::m_closed)
        return;
    //     if(!journal::m_tstamp.empty())
    //         (*journal::m_file) << xstr::date_time(journal::m_tstamp) << ' ';

    //if (journal::m_indent > 0) {
    //    //m_text.clear(); // cleared in attributes definitions.
    //    m_text().insert(0, journal::m_indent * 4, ' ');

    //}
}

journal::journal()
{
    if(!journal::m_file) {
        journal::setfile("");
        journal::init(journal::Html, ":", false);
        journal::resetstamp();
        return;
    }
    //     if(!journal::m_tstamp.empty())
    //         (*journal::m_file) << xstr::date_time(journal::m_tstamp) << ' ';
    if(journal::m_closed)
        return;
    //if (journal::m_indent > 0) {
    //    //m_text.clear(); // cleared in attributes definitions.
    //    m_text().insert(0, journal::m_indent * 4, ' ');

    //}
}

journal::journal(const char *a_function_name, int aline, const char *filename)
{

    if(!journal::m_file) {
        journal::init(journal::Html, ":", false);
        journal::resetstamp();
        return;
    }
    if(journal::m_closed)
        return;
    //if(!journal::m_tstamp.empty())
    //    m_text << xstr::datetime(journal::m_tstamp) << ' ';

    //if (journal::m_indent > 0)
    //    m_text().insert(0, journal::m_indent * 4, ' ');

    if(!a_function_name) {
        m_text << journal::m_attr_table[(int) journal::Reset];
        return;
    }

    m_text << journal::m_attr_table[(int) journal::Reset];

    if(filename)
        m_text << filename << " ";
    if(aline)
        m_text << "line:" << aline << journal::m_attr_table[(int) journal::Reset] << ' ';
    if(a_function_name)
        m_text << " " << journal::m_fn_prefix() << a_function_name << journal::m_attr_table[(int) journal::Reset] << ' ';

}

journal::journal(journal::Type t, const char *a_function_name, int aline, const char *filename)
{
    if(!journal::m_file) {
        journal::init(journal::Html, ":", false);
        journal::resetstamp();
        return;
    }
    if(journal::m_closed)
        return;
    //if(!journal::m_tstamp.empty())
    //    m_text << (*xchronos()).text() << ' ';
    m_text << journal::m_type_table[(int) t] << journal::m_attr_table[(int) journal::Reset];
    if(!a_function_name) {
        return;
    }
    
    if(filename)
        m_text << filename << " ";
    if(aline)
        m_text << "line:" << aline << journal::m_attr_table[(int) journal::Reset] << ' ';
    if(a_function_name)
        m_text << " " << journal::m_fn_prefix() << a_function_name << journal::m_attr_table[(int) journal::Reset] << ' ';
}

journal::~journal()
{
    if(!m_text.empty()) {
        (*journal::m_file) << m_text();
        journal::m_file->flush();
        m_text.clear();
        return;
    }

    if(journal::m_file)
        journal::m_file->flush();
}

/*!
*    @brief creates and opens file for output.
*
*    @param afilename the complete path and name of the file to be used for output.
*    <p> if \c afilename is empty, then stdout will be used as the output file. </p>
*
*    @author &copy; 2015,2017, serge teacc::utilssier (bretzeteacc::utils), teacc::utilssier.serge@gmail.com
*
*/
void journal::setfile(const std::string &afilename)
{
    journal::m_filename = afilename;
    if(journal::m_filename.empty()) {
        journal::m_stdout = true;
        journal::m_file = &std::cout; // duh ?
        return;
    }
    journal::m_file_output.open(journal::m_filename().c_str(), std::ios::out);
    if(!journal::m_file_output.good()) {
        xstr msg = "error (#%d) openning \"%s\" for output : `%s` - falling back to std out.";
        msg.arg(errno);
        msg.arg(journal::m_filename);
        std::string e = strerror(errno);
        msg.arg(e);

        journal::m_stdout = true;
        journal::m_file = &std::cout; // duh ?
        //throw msg.c_str();
        return;
    }
    journal::m_file = &journal::m_file_output;

}

journal &journal::pushattr()
{
    journal::m_spanstack.push(0);
    return *this;
}

journal &journal::popattr()
{
    if(!journal::m_spanstack.empty()) {
        m_text << journal::attrreset();
        journal::m_spanstack.pop();
    }
    return *this;
}

bool journal::close()
{
    m_closed = true;
    bool r = true;
    if(journal::m_file_output.is_open()) {
        (*journal::m_file) << journal::m_attr_table[(int) journal::Black] << "}" << journal::m_eol << "end" << journal::m_eol;
        if(!journal::m_stdout)
            journal::m_file_output.close();
    }else {
        if(journal::m_stdout)
            (*journal::m_file) << "} end" << journal::m_eol;
        else
            r = false;
    }
    journal::m_attr_table.clear();
    journal::m_fn_prefix.clear();
    journal::m_objects_prefix_table.clear();
    journal::m_type_table.clear();

    return r;
}

std::string journal::attribute(journal::Attr att)
{
    return journal::m_attr_table[att];
}

std::string journal::text(journal::Object o)
{
    switch(o) {
        case journal::Code:return journal::m_attr_table[journal::Pre];
        case journal::EndCode:return journal::m_attr_table[journal::EndPre];
        default:return "incomming !!";

    }
    return "incomming !!";
}

std::string journal::text(journal::Attr o)
{
    return std::string();
}

std::string journal::startcode()
{
    return journal::m_attr_table[(int) journal::Pre];
}

std::string journal::codeend()
{
    return journal::m_attr_table[(int) journal::EndPre];
}

std::string journal::attrreset()
{
    return journal::m_attr_table[(int) journal::Reset];
}

/*!
*    @brief clears \c journal::m_tstamp static std::string contents.
*        this static method resets the date-time stamp and then returns an anonymous instance that is to be used as the construction of a new date-time using journal's comma operator.
*        the `stamp` is part of the prefix of each line. if \c journal::ResetStamp() if used alone without any further construction, then there will be no date-time stamp on the line.
*        @code
*            // example of usage:
*            journal::ResetStamp(), journal::hour24, ':', journal::minutes, ',', journal::seconds;
*        @endcode
*    @return an anounymous instance of journal to be used for journal::operators
*/
journal journal::resetstamp()
{
    journal::m_tstamp = "";
    return journal();
}

/*!
*
*    @brief appends time-date object to the stamp
*
*    <p> journal++ follows and uses the posix time date format to be given to std::strftime :
*
*  the characters of ordinary character sequences (including the null byte) are copied verbatim from format to s. however, the characters of conversion specifications are replaced as follows:
*  @li %a  the abbreviated name of the day of the week according to the current locale.
*  @li %A  the full name of the day of the week according to the current locale.
*  @li %b  the abbreviated month name according to the current locale.
*  @li %b  the full month name according to the current locale.
*  @li %c  the preferred date and time representation for the current locale.
*  @li %c  the century number (year/100) as a 2-digit integer. (su)
*  @li %d  the day of the month as a decimal number (range 01 to 31).
*  @li %D  equivalent to %m/%d/%y. (yecch—for americans only. americans should note that in other countries %d/%m/%y is rather common. this means that in international context this format is ambiguous and should not be used.) (su)
*  @li %e  like %d, the day of the month as a decimal number, but a leading zero is replaced by a space. (su)
*  @li %e  modifier: use alternative format, see below. (su)
*  @li %f  equivalent to %y-%m-%d (the iso 8601 date format). (c99)
*  @li %g  the iso 8601 week-based year (see notes) with century as a decimal number. the 4-digit year corresponding to the iso week number (see %v). this has the same format and value as %y, except that if the iso week number belongs to the previous or next year, that year is used insxiod. (tz)
*  @li %g  like %g, but without century, that is, with a 2-digit year (00-99). (tz)
*  @li %h  equivalent to %b. (su)
*  @li %H  the hour as a decimal number using a 24-hour clock (range 00 to 23).
*  @li %i  the hour as a decimal number using a 12-hour clock (range 01 to 12).
*  @li %j  the day of the year as a decimal number (range 001 to 366).
*  @li %k  the hour (24-hour clock) as a decimal number (range 0 to 23); single digits are preceded by a blank. (see also %h.) (tz)
*  @li %l  the hour (12-hour clock) as a decimal number (range 1 to 12); single digits are preceded by a blank. (see also %i.) (tz)
*  @li %m  the month as a decimal number (range 01 to 12).
*  @li %m  the minute as a decimal number (range 00 to 59).
*  @li %n  a newline character. (su)
*  @li %o  modifier: use alternative format, see below. (su)
*  @li %P  either "am" or "pm" according to the given time value, or the corresponding strings for the current locale. noon is treated as "pm" and midnight as "am".
*  @li %p  like %p but in lowercase: "am" or "pm" or a corresponding string for the current locale. (gnu)
*  @li %r  the time in a.m. or p.m. notation. in the posix locale this is equivalent to %i:%m:%s %p. (su)
*  @li %r  the time in 24-hour notation (%h:%m). (su) for a version including the seconds, see %t below.
*  @li %s  the number of seconds since the epoch, 1970-01-01 00:00:00 +0000 (utc). (tz)
*  @li %S  the second as a decimal number (range 00 to 60). (the range is up to 60 to allow for occasional leap seconds.)
*  @li %t  a tab character. (su)
*  @li %T  the time in 24-hour notation (%h:%m:%s). (su)
*  @li %u  the day of the week as a decimal, range 1 to 7, monday being 1. see also %w. (su)
*  @li %u  the week number of the current year as a decimal number, range 00 to 53, starting with the first sunday as the first day of week 01. see also %v and %w.
*  @li %v  the iso 8601 week number (see notes) of the current year as a decimal number, range 01 to 53, where week 1 is the first week that has at least 4 days in the new year. see also %u and %w. (su)
*  @li %w  the day of the week as a decimal, range 0 to 6, sunday being 0. see also %u.
*  @li %w  the week number of the current year as a decimal number, range 00 to 53, starting with the first monday as the first day of week 01.
*  @li %x  the preferred date representation for the current locale without the time.
*  @li %x  the preferred time representation for the current locale without the date.
*  @li %y  the year as a decimal number without a century (range 00 to 99).
*  @li %y  the year as a decimal number including the century.
*  @li %z  the +hhmm or -hhmm numeric timezone (that is, the hour and minute offset from utc). (su)
*  @li %z  the timezone name or abbreviation.
*  @li %+  the date and time in date(1) format. (tz) (not supported in glibc2.)
*  @li %%  a literal '%' character.
*
*  @param dt_obj an journal::object enumerator value / valeur &eacute;num&eacute;ratrice
*
*  <p>
*    usage:<br />
*    @code
*        // ...
*        journal::ResetStamp(), journal::hour24 , journal::minute, journal::seconds ;
*        //...
*
*    @endcode
*  @note not all format modifiers are used. / tous les modifiants format ne sont pas utilis&eacute;s
*/

journal &journal::operator,(journal::Object dt_obj)
{

    switch(dt_obj) {
        case journal::LDay:journal::m_tstamp += "%A";
            break;
        case journal::Day:journal::m_tstamp += "%d";
            break;
        case journal::YDay:journal::m_tstamp += "%j";
            break;
        case journal::MonthNum:journal::m_tstamp += "%m";
            break;
        case journal::MonthNameShort:journal::m_tstamp += "%b";
            break;
        case journal::MonthNameLong:journal::m_tstamp += "%b";
            break;
        case journal::YearShort:journal::m_tstamp += "%y";
            break;
        case journal::YearLong:journal::m_tstamp += "%Y";
            break;
        case journal::Hour:journal::m_tstamp += "%I";
            break;
        case journal::THour:journal::m_tstamp += "%T";
            break;
        case journal::AmPm:journal::m_tstamp += "%P";
            break;
        case journal::Hour24:journal::m_tstamp += "%H";
            break;
        case journal::Minute:journal::m_tstamp += "%M";
            break;
        case journal::Seconds:journal::m_tstamp += "%S";
            break;
        default: return *this;
    }

    //journal::m_tstamp << journal()[journal::reset];
    return *this;

}

journal &journal::operator,(journal::Attr attr)
{
    journal::m_tstamp += journal::m_attr_table[(int) attr];
    return *this;
}

//disabled, but left there to prevent the the compiler to make full copy....
journal &journal::operator=(const journal &other)
{

    return *this;
}

// --
bool journal::operator==(const journal &other) const
{
    return false;
}

journal &journal::operator<<(journal::Attr attr)
{
    m_text << journal::m_attr_table[(int) attr];

    return *this;
}

journal &journal::operator<<(journal::Tail til)
{
    switch(til) {
        case journal::LF:(*this) << journal::m_eol;
            (*journal::m_file) << m_text();
            m_text.clear();
            break;
        case journal::End:(*this) << journal::Reset << journal::brk();
            (*journal::m_file) << m_text();
            m_text.clear();
            break;
    }
    (*journal::m_file) << m_text();
    //m_text.clear();
    return *this;
}

journal &journal::operator<<(journal::Object obj)
{
    //m_text.clear();
    switch(obj) {
        case journal::Stamp:m_text << xstr::datetime(journal::m_tstamp) << journal::m_attr_table[(int) journal::Reset];

            break;
        case journal::FN:m_text << journal::m_attr_table[(int) journal::Reset] << journal::m_fn_prefix() << journal::m_attr_table[(int) journal::Reset];
            break;
        case journal::FName:m_text << journal::m_attr_table[(int) journal::Reset] << journal::m_long_fn_prefix() << journal::m_attr_table[(int) journal::Reset];
            break;
            // ...
            //case journal::inc:
            //	++journal::m_indent;
            //	break;
            //case journal::dec:
            //	//journal::m_indent = --journal::m_indent < 0 ? 0 : journal::m_indent;
            //	--journal::m_indent;
            //	if (journal::m_indent < 0) journal::m_indent = 0;
            //	break;
        case journal::Code:journal::m_code_mode = true;
            m_text << journal::m_attr_table[(int) journal::Pre];
            break;
        case journal::EndCode:journal::m_code_mode = false;
            m_text << journal::m_attr_table[(int) journal::EndPre];
            break;
        case journal::Para:m_text << "<p>\n";
            break;
        case journal::EndPara:m_text << "</p>";
            break;
        default:break;
    };

    return *this;
}

journal &journal::operator<<(journal::Type t)
{
    m_text << journal::m_attr_table[(int) journal::Reset] << journal::m_type_table[(int) t];

    return *this;
}

journal &journal::operator<<(journal::Arrow arr)
{
    //  ➜ ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋

    m_text << journal::m_arrows[(int) arr];

    return *this;
}

std::string journal::operator[](journal::Attr attr)
{
    return journal::m_attr_table[(int) attr];
}

std::string journal::operator[](journal::Type tclass)
{
    return journal::m_type_table[(int) tclass];
}

std::string journal::operator[](journal::Object obj)
{
    // thinking ...
    return journal::m_objects_prefix_table[(int) obj];
}

int journal::init(journal::Mode mode, const std::string &lname, bool use_pid)
{
    journal::m_mode = mode;
    std::map<journal::Mode, std::string> names;

    std::string mode_name;
    switch(journal::m_mode) {

        case journal::Mode::Ansi: mode_name = "ansi256 unicode console format output";
            journal::_initansi();
            break;
        case journal::Mode::Html: mode_name = "html format output";
            journal::_inithtml();
            break;
        case journal::Mode::None: mode_name = "unformatted output";
            journal::_initplain();
            break;
        case journal::Mode::Nsml: mode_name = "sml  format output";
            journal::_initsml();
            break;
        case journal::Mode::Plain:mode_name = "unformatted output";
            journal::_initplain();
            break;
        default: mode_name = "unformatted output";
            journal::_initplain();
            break;
    }

    // setup full date-time for the journal header:
    //journal::reset_stamp(), journal::white, journal::lday, ',',
    journal::resetstamp(), journal::LDay, ',', journal::MonthNameLong, ' ', journal::Day, ' ', journal::YearLong, "  ", journal::Hour, ':', journal::Minute, ':', journal::Seconds;
    //journal::ampm;


    journal::m_log_header.clear();
    journal::m_log_header += xstr::datetime(journal::m_tstamp);
    //std::cerr << __PRETTY_FUNCTION__ << " - important debug trace :  logger::m_stamp:" << journal::m_tstamp() << "\n";
    journal::m_log_header += " -  journal: name '%s%s%s'" + journal::m_attr_table[(int) journal::HCyan] + lname + journal::attrreset();

    if(use_pid) {
        journal::m_log_header << " pid:%s%d%s ";
        journal::m_log_header << journal::m_attr_table[(int) journal::HCyan];
        #ifdef _WIN32
        journal::m_log_header << GetCurrentProcessId();
        #else

        journal::m_log_header << getpid();
        #endif
        journal::m_log_header << journal::m_attr_table[(int) journal::Reset];
    }

    journal::m_log_header << "format: '%s%s'" << journal::m_attr_table[(int) journal::HCyan] << mode_name << journal::m_attr_table[(int) journal::Red] << journal::m_eol << journal::m_attr_table[(int) Black] << "begin" << journal::m_eol << "{" << journal::m_eol << journal::attrreset();
    //name:


    (*journal::m_file) << journal::m_log_header() << journal::attrreset();
    journal::m_log_header.clear();

    // init text attributes/style for type/class of messages:
    // info=0, err, critical, fatal, debug, excep, notice, warnings
    //journal::m_type_table.push_back(journal::m_attr_table[(int)hblue] + std::string("⚫") + " information: "); // information : light blue
    //journal::m_type_table.push_back(journal::m_attr_table[(int)red] + std::string("✘") + " error: "); // error
    //journal::m_type_table.push_back(journal::m_attr_table[(int)hred] + std::string("⚒") + " critical:  "); // critical
    //journal::m_type_table.push_back(journal::m_attr_table[(int)magenta] + std::string("☠") + " fatal:     "); // fatal hihihihi!
    //journal::m_type_table.push_back(journal::m_attr_table[(int)green] + std::string("➤") + " debug:     "); // debug
    //journal::m_type_table.push_back(journal::m_attr_table[(int)cyan] + std::string("⚡") + " exception: "); // exceptions
    //journal::m_type_table.push_back(journal::m_attr_table[(int)yellow] + std::string("✎") + " notice:    "); // notice / notes
    //journal::m_type_table.push_back(journal::m_attr_table[(int)yellow] + std::string("⚑"+l'⚠' ) + " warning:   "); // and ...warnings ... :-) ⚠
    //journal::m_type_table.push_back(journal::m_attr_table[(int)hgreen] + std::string("✔") + " succes:    "); // and ...succes ... :-)

    journal::m_type_table.push_back(journal::m_attr_table[(int) HBlue] + journal::m_attr_table[(int) journal::Italic] + "[information]:" + journal::attrreset()); // information : light blue
    journal::m_type_table.push_back(journal::m_attr_table[(int) Red] + journal::m_attr_table[(int) journal::Italic] + "[error      ]:" + journal::attrreset()); // error
    journal::m_type_table.push_back(journal::m_attr_table[(int) HRed] + journal::m_attr_table[(int) journal::Italic] + "[critical   ]:" + journal::attrreset()); // critical
    journal::m_type_table.push_back(journal::m_attr_table[(int) Magenta] + journal::m_attr_table[(int) journal::Italic] + "[fatal      ]:" + journal::attrreset()); // fatal hihihihi!
    journal::m_type_table.push_back(journal::m_attr_table[(int) Green] + journal::m_attr_table[(int) journal::Italic] + "[debug      ]:" + journal::attrreset()); // debug
    journal::m_type_table.push_back(journal::m_attr_table[(int) HCyan] + journal::m_attr_table[(int) journal::Italic] + "[exception  ]:" + journal::attrreset()); // exceptions
    journal::m_type_table.push_back(journal::m_attr_table[(int) Yellow] + journal::m_attr_table[(int) journal::Italic] + "[notice     ]:" + journal::attrreset()); // notice / notes
    journal::m_type_table.push_back(journal::m_attr_table[(int) Yellow] + journal::m_attr_table[(int) journal::Italic] + "[warning    ]:" + journal::attrreset()); // and ...warnings ... :-) ⚠
    journal::m_type_table.push_back(journal::m_attr_table[(int) HGreen] + journal::m_attr_table[(int) journal::Italic] + "[succes     ]:" + journal::attrreset()); // and ...succes ... :-)

    // now the text attributres/style for rtfn signature...
    journal::m_fn_prefix = journal()[journal::HCyan];
    journal::m_long_fn_prefix = journal()[journal::Magenta];


    // attributes for the objects

    journal::m_objects_prefix_table.push_back(journal::m_attr_table[(int) journal::Cyan]);
    journal::m_objects_prefix_table.push_back(journal::m_attr_table[(int) journal::Cyan]);
    journal::m_objects_prefix_table.push_back(journal::m_attr_table[(int) journal::HGray]);

    return 0;
}

void journal::_inithtml()
{
    journal::m_attr_table.push_back("<span style=\"color : #000;\">"); // journal::black
    journal::m_attr_table.push_back("<span style=\"color : #400000;\">"); // journal::red
    journal::m_attr_table.push_back("<span style=\"color : #005000;\">"); // journal::green
    journal::m_attr_table.push_back("<span style=\"color : #484000;\">");// journal::ambre
    journal::m_attr_table.push_back("<span style=\"color : #000650;\">"); // journal::blue
    journal::m_attr_table.push_back("<span style=\"color : #400040;\">"); // journal::magenta
    journal::m_attr_table.push_back("<span style=\"color : #003530;\">"); // journal::cyan
    journal::m_attr_table.push_back("<span style=\"color : #707070;\">"); // journal::hgray
    journal::m_attr_table.push_back("<span style=\"color : #404040;\">");  // journal::dgray
    journal::m_attr_table.push_back("<span style=\"color : #850000;\">"); // journal::hred
    journal::m_attr_table.push_back("<span style=\"color : #008500;\">"); // journal::hgreen
    journal::m_attr_table.push_back("<span style=\"color : #868000;\">"); // journal::yellow // darker for white background
    journal::m_attr_table.push_back("<span style=\"color : #001080;\">"); // journal::hblue
    journal::m_attr_table.push_back("<span style=\"color : #800980;\">"); // journal::hmagenta
    journal::m_attr_table.push_back("<span style=\"color : #007078;\">"); // journal::hcyan
    journal::m_attr_table.push_back("<span style=\"color : #fff;\">"); // journal::white
    journal::m_attr_table.push_back("<span style=\"color:black;\"></span></span>");// journal::reset
    journal::m_attr_table.push_back("<span style=\"font-style : italic;\">");// journal::italic
    journal::m_attr_table.push_back("<span style=\"text-style : bold;\">");// journal::strong
    journal::m_attr_table.push_back("<sup>");// journal::sup
    journal::m_attr_table.push_back("</sup>");// journal::_sup
    journal::m_attr_table.push_back("<sub>");// journal::sub
    journal::m_attr_table.push_back("</sub>");// journal::_sub
    journal::m_attr_table.push_back("<div class='code'><pre>\n");// journal::code
    journal::m_attr_table.push_back("</pre>\n</div>");// journal::endcode

    journal::m_eol = "<br/>";
    if(!journal::m_file->good()) {
        std::cerr << " error : " << strerror(errno) << "\n";
    }
}

/*!
    @note Je crois qu'on peut utiliser systematiquement une sequence de 256 couleurs.
 */
void journal::_initansi()
{

    journal::m_attr_table.push_back("\033[0;0m"); // journal::black
    journal::m_attr_table.push_back("\033[0;31m"); // journal::red
    journal::m_attr_table.push_back("\033[0;32m"); // journal::green
    journal::m_attr_table.push_back("\033[0;33m");// journal::ambre
    journal::m_attr_table.push_back("\033[0;34m"); // journal::blue
    journal::m_attr_table.push_back("\033[0;35m"); // journal::magenta
    journal::m_attr_table.push_back("\033[0;36m"); // journal::cyan
    journal::m_attr_table.push_back("\033[0;37m"); // journal::hgray
    journal::m_attr_table.push_back("\033[1;30m");  // journal::drakgray
    journal::m_attr_table.push_back("\033[1;31m"); // journal::hred
    journal::m_attr_table.push_back("\033[1;32m"); // journal::hgreen
    journal::m_attr_table.push_back("\033[1;33m"); // journal::yellow
    journal::m_attr_table.push_back("\033[1;34m"); // journal::hblue
    journal::m_attr_table.push_back("\033[1;35m"); // journal::hmagenta
    journal::m_attr_table.push_back("\033[1;36m"); // journal::hcyan
    journal::m_attr_table.push_back("\033[1;37m"); // journal::white
    journal::m_attr_table.push_back("\033[0m");// journal::reset
    journal::m_attr_table.push_back("\033[0m");// journal::italic
    journal::m_attr_table.push_back("\033[0m");// journal::strong
    journal::m_attr_table.push_back("\033[0m");// journal::sup
    journal::m_attr_table.push_back("\033[0m");// journal::sub
    journal::m_attr_table.push_back("");// journal::code
    journal::m_attr_table.push_back("");// journal::endcode
    journal::m_eol = "\n";
}

void journal::_initplain()
{
    journal::m_attr_table.push_back(""); // journal::black
    journal::m_attr_table.push_back(""); // journal::red
    journal::m_attr_table.push_back(""); // journal::green
    journal::m_attr_table.push_back("");// journal::ambre
    journal::m_attr_table.push_back(""); // journal::blue
    journal::m_attr_table.push_back(""); // journal::magenta
    journal::m_attr_table.push_back(""); // journal::cyan
    journal::m_attr_table.push_back(""); // journal::hgray
    journal::m_attr_table.push_back("");  // journal::drakgray
    journal::m_attr_table.push_back(""); // journal::hred
    journal::m_attr_table.push_back(""); // journal::hgreen
    journal::m_attr_table.push_back(""); // journal::yellow
    journal::m_attr_table.push_back(""); // journal::hblue
    journal::m_attr_table.push_back(""); // journal::hmagenta
    journal::m_attr_table.push_back(""); // journal::hcyan
    journal::m_attr_table.push_back(""); // journal::white
    journal::m_attr_table.push_back("");// journal::reset
    journal::m_attr_table.push_back("");// journal::italic
    journal::m_attr_table.push_back("");// journal::strong
    journal::m_attr_table.push_back("");// journal::sup
    journal::m_attr_table.push_back("");// journal::sub
    journal::m_attr_table.push_back("");// journal::code
    journal::m_attr_table.push_back("");// journal::endcode
    journal::m_eol = "\n";
}

void journal::_initsml()
{
    journal::m_attr_table.push_back(""); // journal::black
    journal::m_attr_table.push_back(""); // journal::red
    journal::m_attr_table.push_back(""); // journal::green
    journal::m_attr_table.push_back("");// journal::ambre
    journal::m_attr_table.push_back(""); // journal::blue
    journal::m_attr_table.push_back(""); // journal::magenta
    journal::m_attr_table.push_back(""); // journal::cyan
    journal::m_attr_table.push_back(""); // journal::hgray
    journal::m_attr_table.push_back("");  // journal::drakgray
    journal::m_attr_table.push_back(""); // journal::hred
    journal::m_attr_table.push_back(""); // journal::hgreen
    journal::m_attr_table.push_back(""); // journal::yellow
    journal::m_attr_table.push_back(""); // journal::hblue
    journal::m_attr_table.push_back(""); // journal::hmagenta
    journal::m_attr_table.push_back(""); // journal::hcyan
    journal::m_attr_table.push_back(""); // journal::white
    journal::m_attr_table.push_back("");// journal::reset
    journal::m_attr_table.push_back("");// journal::italic
    journal::m_attr_table.push_back("");// journal::strong
    journal::m_attr_table.push_back("");// journal::sup
    journal::m_attr_table.push_back("");// journal::sub
    journal::m_attr_table.push_back("");// journal::code
    journal::m_attr_table.push_back("");// journal::endcode
    journal::m_eol = "\n";

}

wchar_t journal::operator[](journal::Arrow ar)
{
    return journal::m_arrows[(int) ar];
}
}
