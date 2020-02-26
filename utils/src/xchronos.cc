//
// Created by bretzel on 20-01-27.
//

#include <xio/utils/xchronos>


namespace xio::utils
{

xchronos::xchronos(xchronos&& xs) noexcept
{
    
    _stamp = xs._stamp;
    _clock = xs._clock;
    _text  = xs._text;
    _fmt   = xs._fmt;
}

xchronos::xchronos(const xchronos& xs)
{
    _stamp = xs._stamp;
    _clock = xs._clock;
    _text  = xs._text;
    _fmt   = xs._fmt;
}


xchronos::~xchronos()
{}

xchronos& xchronos::operator*()
{
    _text.clear();
    _stamp = std::chrono::system_clock::now();
    return *this;
}

std::string xchronos::text(const std::string& a_format_str)
{
    if (a_format_str.empty())
    {
        if (!_text.empty())  // Do not waste precious cpu for repeating
            return _text();
    }
    
    std::string fmt = a_format_str.empty() ? _fmt : a_format_str;
    
    auto _tm = std::chrono::system_clock::to_time_t(_stamp);
    
    _text << std::put_time(std::localtime(&_tm), fmt.c_str());// "%Y-%m-%d %X");
    return _text();
}

xchronos& xchronos::operator=(xchronos&& xs) noexcept
{
    _stamp = xs._stamp;
    _clock = xs._clock;
    _text  = xs._text;
    _fmt   = xs._fmt;
    return *this;
}

xchronos& xchronos::operator=(const xchronos& xs)
{
    _stamp = xs._stamp;
    _clock = xs._clock;
    _text  = xs._text;
    _fmt   = xs._fmt;
    return *this;
}


/*!
    *
    *    @brief appends time-date object to the stamp
    *
    *    <p> Logger++ follows and uses the posix time date format to be given to std::strftime :
    *
    *  the characters of ordinary character sequences (including the null byte) are copied verbatim from format to s. however, the characters of conversion specifications are replaced as follows:
    *  @li %a  the abbreviated name of the day of the week according to the current locale.
    *  @li %A  the full name of the day of the week according to the current locale.
    *  @li %b  the abbreviated month name according to the current locale.
    *  @li %b  the full month name according to the current locale.
    *  @li %c  the preferred date and time representation for the current locale.
    *  @li %c  the century number (year/100) as a 2-digit integer. (su)
    *  @li %d  the day of the month as a decimal number (range 01 to 31).
    *  @li %D  equivalent to %m/%d/%y. (yecch�for americans only. americans should note that in other countries %d/%m/%y is rather common. this means that in international context this format is ambiguous and should not be used.) (su)
    *  @li %e  like %d, the day of the month as a decimal number, but a leading zero is replaced by a space. (su)
    *  @li %e  modifier: use alternative format, see below. (su)
    *  @li %f  equivalent to %y-%m-%d (the iso 8601 date format). (c99)
    *  @li %g  the iso 8601 week-based year (see notes) with century as a decimal number. the 4-digit year corresponding to the iso week number (see %v). this has the same format and value as %y, except that if the iso week number belongs to the previous or next year, that year is used insTead. (tz)
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
    *  @param dt_obj an Logger::object enumerator value / valeur &eacute;num&eacute;ratrice
    *
    *  <p>
    *    usage:<br />
    *    @code
    *        // ...
    *        Logger::ResetStamp(), Logger::hour24 , Logger::minute, Logger::seconds ;
    *        //...
    *
    *    @endcode
    *  @note not all format modifiers are used. / tous les modifiants de format ne sont pas utilis&eacute;s
    */


}
