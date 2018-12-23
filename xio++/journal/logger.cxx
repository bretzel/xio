#include "logger.hpp"
#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif


namespace xio {
std::stack<int> logger::m_spanstack;


string_t logger::m_log_header = "";

wchar_t logger::m_arrows[8] = {
    L'➜', // append space char because the rendering is cut if we have no space between the unicode and the next char...
    L'⬅',
    L'⬆',
    L'⬇',
    L'⬈',
    L'⬉',
    L'⬊',
    L'⬋'
};



string_t             logger::m_filename = "";    ///< ...filename - empty if stdout is to be used.
std::ofstream       logger::m_file_output; ///< the instance of file.
std::ostream*       logger::m_file = 0;        ///< pointer to the ofstream { stdout; or file stream}.
std::map<int32_t, std::string> logger::m_code_type_dictionary;

string_t            logger::m_tstamp = "";      ///< holds the configured date-time format for normal line ouputs.
bool                logger::m_stdout = false;      ///< if i am unable to find the way to assign stdout to m_file_output, a flag set to true...
bool                logger::m_code_mode = false;
bool                logger::m_closed = false;


string_t             logger::m_fn_prefix = "";   ///< saved text attributes/style for the function name { __function__ }.
string_t             logger::m_long_fn_prefix = ""; ///< saved text attributes/style for the long function name. { __pretty_function__ }.

logger::Mode    	logger::m_mode = logger::Plain;

string_t::list	    logger::m_attr_table;  ///< text attributes/style list built from the mode;
string_t::list       logger::m_type_table;  ///< message types list including their attributes/style.
string_t::list       logger::m_objects_prefix_table;

std::string         logger::m_eol = "\n";  ///< line break for the mode.
int                 logger::m_indent = 0;



logger::logger(const logger& l)
{
    // -- break - no copy -- and copy what ?
    if (!logger::m_file) {
        logger::setfile("");
        logger::init(logger::Html, ":", false);
        logger::resetstamp();
        return;
    }
    if (logger::m_closed) return;
    //     if(!logger::m_tstamp.empty())
    //         (*logger::m_file) << string_t::date_time(logger::m_tstamp) << ' ';

    //if (logger::m_indent > 0) {
    //    //m_text.clear(); // cleared in attributes definitions.
    //    m_text().insert(0, logger::m_indent * 4, ' ');

    //}
}




logger::logger()
{
    if (!logger::m_file) {
        logger::setfile("");
        logger::init(logger::Html, ":", false);
        logger::resetstamp();
        return;
    }
    //     if(!logger::m_tstamp.empty())
    //         (*logger::m_file) << string_t::date_time(logger::m_tstamp) << ' ';
    if (logger::m_closed) return;
    //if (logger::m_indent > 0) {
    //    //m_text.clear(); // cleared in attributes definitions.
    //    m_text().insert(0, logger::m_indent * 4, ' ');

    //}
}




logger::logger(const char* a_function_name, int aline, const char* filename)
{
    
    if (!logger::m_file) {
        logger::init(logger::Html, ":", false);
        logger::resetstamp();
        return;
    }
    if (logger::m_closed) return;
    if (!logger::m_tstamp.empty())
        m_text << string_t::datetime(logger::m_tstamp) << ' ';

    //if (logger::m_indent > 0)
    //    m_text().insert(0, logger::m_indent * 4, ' ');

    if (!a_function_name) {
        m_text << logger::m_attr_table[(int)logger::Reset];
        return;
    }

    m_text << logger::m_attr_table[(int)logger::Reset];

    if (filename)
        m_text << filename << " ";
    if (aline)
        m_text << "line:" << aline << logger::m_attr_table[(int)logger::Reset] << ' ';
    if (a_function_name)
        m_text << " " << logger::m_fn_prefix() << a_function_name << logger::m_attr_table[(int)logger::Reset] << ' ';

}

logger::logger(logger::Type t, const char* a_function_name, int aline, const char* filename)
{
    if (!logger::m_file) {
        logger::init(logger::Html, ":", false);
        logger::resetstamp();
        return;
    }
    if (logger::m_closed) return;
    if (!logger::m_tstamp.empty())
        m_text << (*time_stamp()).text() << ' ';
    m_text << logger::m_type_table[(int)t] << logger::m_attr_table[(int)logger::Reset];
    if (!a_function_name) {
        return;
    }
    if (filename)
        m_text << filename << " ";
    if (aline)
        m_text << "line:" << aline << logger::m_attr_table[(int)logger::Reset] << ' ';
    if (a_function_name)
        m_text << " " << logger::m_fn_prefix() << a_function_name << logger::m_attr_table[(int)logger::Reset] << ' ';
}

logger::~logger()
{
    if (!m_text.empty()) {
        (*logger::m_file) << m_text();
        logger::m_file->flush();
        m_text.clear();
        return;
    }

    if (logger::m_file)
        logger::m_file->flush();
}


/*!
*    @brief creates and opens file for output.
*
*    @param afilename the complete path and name of the file to be used for output.
*    <p> if \c afilename is empty, then stdout will be used as the output file. </p>
*
*    @author &copy; 2015,2017, serge lussier (bretzelus), lussier.serge@gmail.com
*
*/
void logger::setfile(const std::string& afilename)
{
    logger::m_filename = afilename;
    if (logger::m_filename.empty()) {
        logger::m_stdout = true;
        logger::m_file = &std::cout; // duh ?
        return;
    }
    logger::m_file_output.open(logger::m_filename().c_str(), std::ios::out);
    if (!logger::m_file_output.good()) {
        string_t msg = "error (#%d) openning \"%s\" for output : `%s` - falling back to std out.";
        msg.arg(errno);
        msg.arg(logger::m_filename);
        std::string e = strerror(errno);
        msg.arg(e);

        logger::m_stdout = true;
        logger::m_file = &std::cout; // duh ?
                                        //throw msg.c_str();
        return;
    }
    logger::m_file = &logger::m_file_output;

}

logger& logger::pushattr()
{
    logger::m_spanstack.push(0);
    return *this;
}

logger& logger::popattr()
{
    if (!logger::m_spanstack.empty()) {
        m_text << logger::attrreset();
        logger::m_spanstack.pop();
    }
    return *this;
}

bool logger::close()
{
    m_closed = true;
    bool r = true;
    if (logger::m_file_output.is_open()) {
        (*logger::m_file) << logger::m_attr_table[(int)logger::Black] << "}" << logger::m_eol << "end" << logger::m_eol;
        if (logger::m_mode == logger::Mode::Html)
            (*logger::m_file) << "</body>\n</html>\n";
        if (!logger::m_stdout)
            logger::m_file_output.close();
    }
    else {
        if (logger::m_stdout)
            (*logger::m_file) << "} end" << logger::m_eol;
        else
            r = false;
    }
    logger::m_attr_table.clear();
    logger::m_fn_prefix.clear();
    logger::m_objects_prefix_table.clear();
    logger::m_type_table.clear();

    return r;
}

std::string logger::attribute(logger::Attr att)
{
    return logger::m_attr_table[att];
}

std::string logger::text(logger::Object o)
{
    switch (o) {
    case logger::Code:
        return logger::m_attr_table[logger::Pre];
    case logger::EndCode:
        return logger::m_attr_table[logger::EndPre];
    default:
        return "incomming !!";

    }
    return "incomming !!";
}

std::string logger::text(logger::Attr o)
{
    return std::string();
}

std::string logger::startcode()
{
    return logger::m_attr_table[(int)logger::Pre];
}

std::string logger::codeend()
{
    return logger::m_attr_table[(int)logger::EndPre];
}

std::string logger::attrreset()
{
    return logger::m_attr_table[(int)logger::Reset];
}


/*!
*    @brief clears \c logger::m_tstamp static std::string contents.
*        this static method resets the date-time stamp and then returns an anonymous instance that is to be used as the construction of a new date-time using logger's comma operator.
*        the `stamp` is part of the prefix of each line. if \c logger::ResetStamp() if used alone without any further construction, then there will be no date-time stamp on the line.
*        @code
*            // example of usage:
*            logger::ResetStamp(), logger::hour24, ':', logger::minutes, ',', logger::seconds;
*        @endcode
*    @return an anounymous instance of logger to be used for logger::operators
*/
logger logger::resetstamp()
{
    logger::m_tstamp = "";
    return logger();
}




/*!
*
*    @brief appends time-date object to the stamp
*
*    <p> logger++ follows and uses the posix time date format to be given to std::strftime :
*
*  the characters of ordinary character sequences (including the null byte) are copied verbatim from format to s. however, the characters of conversion specifications are replaced as follows:
*  @li %a  the abbreviated name of the day of the week according to the current locale.
*  @li %A  the full name of the day of the week according to the current locale.
*  @li %b  the abbreviated month name according to the current locale.
*  @li %B  the full month name according to the current locale.
*  @li %c  the preferred date and time representation for the current locale.
*  @li %C  the century number (year/100) as a 2-digit integer. (su)
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
*  @param dt_obj an logger::object enumerator value / valeur &eacute;num&eacute;ratrice
*
*  <p>
*    usage:<br />
*    @code
*        // ...
*        logger::ResetStamp(), logger::hour24 , logger::minute, logger::seconds ;
*        //...
*
*    @endcode
*  @note not all format modifiers are used. / tous les modifiants format ne sont pas utilis&eacute;s
*/

logger& logger::operator , (logger::Object dt_obj)
{

    switch (dt_obj) {
    case logger::LDay:
        logger::m_tstamp += "%A";
        break;
    case logger::Day:
        logger::m_tstamp += "%d";
        break;
    case logger::YDay:
        logger::m_tstamp += "%j";
        break;
    case logger::MonthNum:
        logger::m_tstamp += "%m";
        break;
    case logger::MonthNameShort:
        logger::m_tstamp += "%b";
        break;
    case logger::MonthNameLong:
        logger::m_tstamp += "%B";
        break;
    case logger::YearShort:
        logger::m_tstamp += "%y";
        break;
    case logger::YearLong:
        logger::m_tstamp += "%Y";
        break;
    case logger::Hour:
        logger::m_tstamp += "%I";
        break;
    case logger::THour:
        logger::m_tstamp += "%T";
        break;
    case logger::AmPm:
        logger::m_tstamp += "%P";
        break;
    case logger::Hour24:
        logger::m_tstamp += "%H";
        break;
    case logger::Minute:
        logger::m_tstamp += "%M";
        break;
    case logger::Seconds:
        logger::m_tstamp += "%S";
        break;
    default: return *this;
    }

    //logger::m_tstamp << logger()[logger::reset];
    return *this;

}

logger& logger::operator,(logger::Attr attr)
{
    logger::m_tstamp += logger::m_attr_table[(int)attr];
    return *this;
}

//disabled, but left there to prevent the the compiler to make full copy....
logger& logger::operator=(const logger& other)
{

    return *this;
}

// --
bool logger::operator==(const logger& other) const
{
    return false;
}


logger& logger::operator<<(logger::Attr attr)
{
    m_text << logger::m_attr_table[(int)attr];

    return *this;
}

logger& logger::operator<<(logger::Tail til)
{
    switch (til) {
    case logger::LF:
        m_text << logger::m_eol;
        (*logger::m_file) << m_text();
        m_text.clear();
        break;
    case logger::End:
        m_text << logger::m_eol;
        (*logger::m_file) << m_text();
        m_text.clear();
        break;
    }
    (*logger::m_file) << m_text();
    //m_text.clear();
    return *this;
}




logger& logger::operator<<(logger::Object obj)
{
    //m_text.clear();
    switch (obj) {
    case logger::Stamp:
        m_text << string_t::datetime(logger::m_tstamp) << logger::m_attr_table[(int)logger::Reset];

        break;
    case logger::FN:
        m_text << logger::m_attr_table[(int)logger::Reset] << logger::m_fn_prefix() << logger::m_attr_table[(int)logger::Reset];
        break;
    case logger::FName:
        m_text << logger::m_attr_table[(int)logger::Reset] << logger::m_long_fn_prefix() << logger::m_attr_table[(int)logger::Reset];
        break;
        // ...
    //case logger::inc:
    //	++logger::m_indent;
    //	break;
    //case logger::dec:
    //	//logger::m_indent = --logger::m_indent < 0 ? 0 : logger::m_indent;
    //	--logger::m_indent;
    //	if (logger::m_indent < 0) logger::m_indent = 0;
    //	break;
    case logger::Code:
        logger::m_code_mode = true;
        m_text << logger::m_attr_table[(int)logger::Pre];
        break;
    case logger::EndCode:
        logger::m_code_mode = false;
        m_text << logger::m_attr_table[(int)logger::EndPre];
        break;
    case logger::Para:
        m_text << "<p>\n";
        break;
    case logger::EndPara:
        m_text << "</p>";
        break;
    default:
        break;
    };

    return *this;
}


logger& logger::operator<<(logger::Type t)
{
    m_text << logger::m_attr_table[(int)logger::Reset] << logger::m_type_table[(int)t];

    return *this;
}

logger& logger::operator << (logger::Arrow arr)
{
    //  ➜ ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋

    m_text << logger::m_arrows[(int)arr];

    return *this;
}

std::string logger::operator[](logger::Attr attr)
{
    return logger::m_attr_table[(int)attr];
}

std::string logger::operator[](logger::Type tclass)
{
    return logger::m_type_table[(int)tclass];
}

std::string logger::operator[](logger::Object obj)
{
    // thinking ...
    return logger::m_objects_prefix_table[(int)obj];
}



int logger::init(logger::Mode mode, const std::string& lname, bool use_pid)
{
    logger::m_mode = mode;
    std::map<logger::Mode, std::string> names;

    std::string mode_name;
    switch (logger::m_mode) {

    case logger::Mode::Ansi: mode_name = "ansi256 unicode console format output"; logger::_initansi();  break;
    case logger::Mode::Html: mode_name = "html format output"; logger::_inithtml();  break;
    case logger::Mode::None: mode_name = "unformatted output"; logger::_initplain(); break;
    case logger::Mode::Nsml: mode_name = "sml  format output"; logger::_initsml();   break;
    case logger::Mode::Plain:mode_name = "unformatted output"; logger::_initplain(); break;
    default: mode_name = "unformatted output"; logger::_initplain(); break;
    }

    // setup full date-time for the logger header:
    //logger::reset_stamp(), logger::white, logger::lday, ',',
    logger::resetstamp(), logger::LDay, ',',
        logger::MonthNameLong, ' ',
        logger::Day, ' ',
        logger::YearLong, "  ",
        logger::Hour, ':',
        logger::Minute, ':',
        logger::Seconds;
    //logger::ampm;


    logger::m_log_header.clear();
    logger::m_log_header << string_t::datetime(logger::m_tstamp);
    logger::m_log_header << " -  logger: name '%s%s%s'" <<
        logger::m_attr_table[(int)logger::HCyan] <<
        lname <<
        logger::attrreset();

    if (use_pid) {
        logger::m_log_header << " pid:%s%d%s ";
        logger::m_log_header << logger::m_attr_table[(int)logger::HCyan];
        #ifdef _WIN32
        logger::m_log_header << GetCurrentProcessId();
        #else
        logger::m_log_header.arg(getpid());
        #endif
        logger::m_log_header << logger::m_attr_table[(int)logger::Reset];
    }

    logger::m_log_header << "format: '%s%s'" <<
        logger::m_attr_table[(int)logger::HCyan] <<
        mode_name <<
        logger::m_attr_table[(int)logger::Red] <<
        logger::m_eol <<
        logger::m_attr_table[(int)Black] << "begin" << logger::m_eol <<
        "{" <<
        logger::m_eol << logger::attrreset();
    //name:


    (*logger::m_file) << logger::m_log_header() << logger::attrreset();
    logger::m_log_header.clear();

    // init text attributes/style for type/class of messages:
    // info=0, err, critical, fatal, debug, excep, notice, warnings
    //logger::m_type_table.push_back(logger::m_attr_table[(int)hblue] + std::string("⚫") + " information: "); // information : light blue
    //logger::m_type_table.push_back(logger::m_attr_table[(int)red] + std::string("✘") + " error: "); // error
    //logger::m_type_table.push_back(logger::m_attr_table[(int)hred] + std::string("⚒") + " critical:  "); // critical
    //logger::m_type_table.push_back(logger::m_attr_table[(int)magenta] + std::string("☠") + " fatal:     "); // fatal hihihihi!
    //logger::m_type_table.push_back(logger::m_attr_table[(int)green] + std::string("➤") + " debug:     "); // debug
    //logger::m_type_table.push_back(logger::m_attr_table[(int)cyan] + std::string("⚡") + " exception: "); // exceptions
    //logger::m_type_table.push_back(logger::m_attr_table[(int)yellow] + std::string("✎") + " notice:    "); // notice / notes
    //logger::m_type_table.push_back(logger::m_attr_table[(int)yellow] + std::string("⚑"+L'⚠' ) + " warning:   "); // and ...warnings ... :-) ⚠
    //logger::m_type_table.push_back(logger::m_attr_table[(int)hgreen] + std::string("✔") + " succes:    "); // and ...succes ... :-)

    logger::m_type_table.push_back(logger::m_attr_table[(int)HBlue] + logger::m_attr_table[(int)logger::Italic] +  "[information]:" + logger::attrreset()); // information : light blue
    logger::m_type_table.push_back(logger::m_attr_table[(int)Red] + logger::m_attr_table[(int)logger::Italic] +    "[error      ]:" + logger::attrreset()); // error
    logger::m_type_table.push_back(logger::m_attr_table[(int)HRed] + logger::m_attr_table[(int)logger::Italic] +   "[critical   ]:" + logger::attrreset()); // critical
    logger::m_type_table.push_back(logger::m_attr_table[(int)Magenta] + logger::m_attr_table[(int)logger::Italic] +"[fatal      ]:" + logger::attrreset()); // fatal hihihihi!
    logger::m_type_table.push_back(logger::m_attr_table[(int)Green] + logger::m_attr_table[(int)logger::Italic] +  "[debug      ]:" + logger::attrreset()); // debug
    logger::m_type_table.push_back(logger::m_attr_table[(int)HCyan] + logger::m_attr_table[(int)logger::Italic] +  "[exception  ]:" + logger::attrreset()); // exceptions
    logger::m_type_table.push_back(logger::m_attr_table[(int)Yellow] + logger::m_attr_table[(int)logger::Italic] + "[notice     ]:" + logger::attrreset()); // notice / notes
    logger::m_type_table.push_back(logger::m_attr_table[(int)Yellow] + logger::m_attr_table[(int)logger::Italic] + "[warning    ]:" + logger::attrreset()); // and ...warnings ... :-) ⚠
    logger::m_type_table.push_back(logger::m_attr_table[(int)HGreen] + logger::m_attr_table[(int)logger::Italic] + "[succes     ]:" + logger::attrreset()); // and ...succes ... :-)

    // now the text attributres/style for function signature...
    logger::m_fn_prefix = logger()[logger::HCyan];
    logger::m_long_fn_prefix = logger()[logger::Magenta];


    // attributes for the objects

    logger::m_objects_prefix_table.push_back(logger::m_attr_table[(int)logger::Cyan]);
    logger::m_objects_prefix_table.push_back(logger::m_attr_table[(int)logger::Cyan]);
    logger::m_objects_prefix_table.push_back(logger::m_attr_table[(int)logger::HGray]);

    return 0;
}

void logger::_inithtml()
{
    logger::m_attr_table.push_back("<span style=\"color : #000;\">"); // logger::black
    logger::m_attr_table.push_back("<span style=\"color : #400000;\">"); // logger::red
    logger::m_attr_table.push_back("<span style=\"color : #005000;\">"); // logger::green
    logger::m_attr_table.push_back("<span style=\"color : #484000;\">");// logger::ambre
    logger::m_attr_table.push_back("<span style=\"color : #000650;\">"); // logger::blue
    logger::m_attr_table.push_back("<span style=\"color : #400040;\">"); // logger::magenta
    logger::m_attr_table.push_back("<span style=\"color : #003530;\">"); // logger::cyan
    logger::m_attr_table.push_back("<span style=\"color : #707070;\">"); // logger::hgray
    logger::m_attr_table.push_back("<span style=\"color : #404040;\">");  // logger::dgray
    logger::m_attr_table.push_back("<span style=\"color : #850000;\">"); // logger::hred
    logger::m_attr_table.push_back("<span style=\"color : #008500;\">"); // logger::hgreen
    logger::m_attr_table.push_back("<span style=\"color : #868000;\">"); // logger::yellow // darker for white background
    logger::m_attr_table.push_back("<span style=\"color : #001080;\">"); // logger::hblue
    logger::m_attr_table.push_back("<span style=\"color : #800980;\">"); // logger::hmagenta
    logger::m_attr_table.push_back("<span style=\"color : #007078;\">"); // logger::hcyan
    logger::m_attr_table.push_back("<span style=\"color : #fff;\">"); // logger::white
    logger::m_attr_table.push_back("</span>");// logger::reset
    logger::m_attr_table.push_back("<span style=\"font-style : italic;\">");// logger::italic
    logger::m_attr_table.push_back("<span style=\"text-style : bold;\">");// logger::strong
    logger::m_attr_table.push_back("<sup>");// logger::sup
    logger::m_attr_table.push_back("</sup>");// logger::_sup
    logger::m_attr_table.push_back("<sub>");// logger::sub
    logger::m_attr_table.push_back("</sub>");// logger::_sub
    logger::m_attr_table.push_back("<div class='code'><pre>\n");// logger::code
    logger::m_attr_table.push_back("</pre>\n</div>");// logger::endcode

    logger::m_eol = "<br/>";

    string_t sheet =
        "<html>\n"
        "<head>\n"
        "    <style>\n"
        "        body{ background-color:#fff;}\n"
        "        pre{ margin:0px; }\n"
        "        .code{\n"
        "            /*min-height : 90px;\n*/"
        "            margin        :0px;"
        "            display    : bloc;\n"
        "            width  : auto;\n"
        "            /*font-weight: bold;*/\n"
        "            font-size  : 12pt;\n"
        "            /*padding    : 5px 5px 5px 5px;*/\n"
        "            background-color: #fff;\n"
        "        }\n    </style>\n    </head>\n<body>\n";
    (*logger::m_file) << sheet() << std::endl;
    if (!logger::m_file->good()) {
        std::cerr << " error : " << strerror(errno) << "\n";
    }
}


/*!
    @note Je crois qu'on peut utiliser systematiquement une sequence de 256 couleurs.
 */
void logger::_initansi()
{
    
    logger::m_attr_table.push_back("\033[0;0m"); // logger::black
    logger::m_attr_table.push_back("\033[0;31m"); // logger::red
    logger::m_attr_table.push_back("\033[0;32m"); // logger::green
    logger::m_attr_table.push_back("\033[0;33m");// logger::ambre
    logger::m_attr_table.push_back("\033[0;34m"); // logger::blue
    logger::m_attr_table.push_back("\033[0;35m"); // logger::magenta
    logger::m_attr_table.push_back("\033[0;36m"); // logger::cyan
    logger::m_attr_table.push_back("\033[0;37m"); // logger::hgray
    logger::m_attr_table.push_back("\033[1;30m");  // logger::drakgray
    logger::m_attr_table.push_back("\033[1;31m"); // logger::hred
    logger::m_attr_table.push_back("\033[1;32m"); // logger::hgreen
    logger::m_attr_table.push_back("\033[1;33m"); // logger::yellow
    logger::m_attr_table.push_back("\033[1;34m"); // logger::hblue
    logger::m_attr_table.push_back("\033[1;35m"); // logger::hmagenta
    logger::m_attr_table.push_back("\033[1;36m"); // logger::hcyan
    logger::m_attr_table.push_back("\033[1;37m"); // logger::white
    logger::m_attr_table.push_back("\033[0m");// logger::reset
    logger::m_attr_table.push_back("\033[0m");// logger::italic
    logger::m_attr_table.push_back("\033[0m");// logger::strong
    logger::m_attr_table.push_back("\033[0m");// logger::sup
    logger::m_attr_table.push_back("\033[0m");// logger::sub
    logger::m_attr_table.push_back("");// logger::code
    logger::m_attr_table.push_back("");// logger::endcode
    logger::m_eol = "\n";
}

void logger::_initplain()
{
    logger::m_attr_table.push_back(""); // logger::black
    logger::m_attr_table.push_back(""); // logger::red
    logger::m_attr_table.push_back(""); // logger::green
    logger::m_attr_table.push_back("");// logger::ambre
    logger::m_attr_table.push_back(""); // logger::blue
    logger::m_attr_table.push_back(""); // logger::magenta
    logger::m_attr_table.push_back(""); // logger::cyan
    logger::m_attr_table.push_back(""); // logger::hgray
    logger::m_attr_table.push_back("");  // logger::drakgray
    logger::m_attr_table.push_back(""); // logger::hred
    logger::m_attr_table.push_back(""); // logger::hgreen
    logger::m_attr_table.push_back(""); // logger::yellow
    logger::m_attr_table.push_back(""); // logger::hblue
    logger::m_attr_table.push_back(""); // logger::hmagenta
    logger::m_attr_table.push_back(""); // logger::hcyan
    logger::m_attr_table.push_back(""); // logger::white
    logger::m_attr_table.push_back("");// logger::reset
    logger::m_attr_table.push_back("");// logger::italic
    logger::m_attr_table.push_back("");// logger::strong
    logger::m_attr_table.push_back("");// logger::sup
    logger::m_attr_table.push_back("");// logger::sub
    logger::m_attr_table.push_back("");// logger::code
    logger::m_attr_table.push_back("");// logger::endcode
    logger::m_eol = "\n";
}

void logger::_initsml()
{
    logger::m_attr_table.push_back(""); // logger::black
    logger::m_attr_table.push_back(""); // logger::red
    logger::m_attr_table.push_back(""); // logger::green
    logger::m_attr_table.push_back("");// logger::ambre
    logger::m_attr_table.push_back(""); // logger::blue
    logger::m_attr_table.push_back(""); // logger::magenta
    logger::m_attr_table.push_back(""); // logger::cyan
    logger::m_attr_table.push_back(""); // logger::hgray
    logger::m_attr_table.push_back("");  // logger::drakgray
    logger::m_attr_table.push_back(""); // logger::hred
    logger::m_attr_table.push_back(""); // logger::hgreen
    logger::m_attr_table.push_back(""); // logger::yellow
    logger::m_attr_table.push_back(""); // logger::hblue
    logger::m_attr_table.push_back(""); // logger::hmagenta
    logger::m_attr_table.push_back(""); // logger::hcyan
    logger::m_attr_table.push_back(""); // logger::white
    logger::m_attr_table.push_back("");// logger::reset
    logger::m_attr_table.push_back("");// logger::italic
    logger::m_attr_table.push_back("");// logger::strong
    logger::m_attr_table.push_back("");// logger::sup
    logger::m_attr_table.push_back("");// logger::sub
    logger::m_attr_table.push_back("");// logger::code
    logger::m_attr_table.push_back("");// logger::endcode
    logger::m_eol = "\n";

}

wchar_t logger::operator[](logger::Arrow ar)
{
    return logger::m_arrows[(int)ar];
}

}
