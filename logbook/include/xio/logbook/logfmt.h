#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <stack>
#include <vector>
#include <xio/utils/xreturn>

namespace xio::logbook
{
class logbook_api logfmt
{
protected:
    utils::xstr _text;

public:

    using return_code = utils::xreturn<utils::notification::code>;

    enum class attrib : uint8_t
    {
        /* html: */ normal, italic, strong, sup, _sup, sub, _sub, pre, endpre ///< styles suppl&eacute;mentaires du texte au format `html`
    };

    enum class color : uint8_t
    {
        /* colors*/ black = 0, red = 1, green = 2, amber = 3, blue = 4, magenta = 5, cyan = 6, hgray, dgray, hred, hgreen, yellow, hblue, hmagenta, hcyan, white,
        /* reset */ reset, ///< remise-&agrave;-z&eacute;ro des attributs.
    };

    enum class object : uint8_t
    {
        sfn = 0,         ///< Appends the function's short name ( __FUNCTION__ ) in place ...
        lfn,             ///< Appends the long function's name { __PRETTY_FUNCTION__/ }.
        stamp,           ///< Appends output contents of m_tstamp @see std::string::date_time(const std::string& format) ; contenu de cJournal::m_tstamp
        lday,            ///< Appends day name long format; nom complet du jour
        day,             ///< day number {week} ; num&eacute;ro du jour dans le mois
        yday,            ///< day number {year} ; num&eacute;ro du jour dans l'annee
        nmo,             ///< month number; num&eacute;ro du mois
        smn,             ///< month short name ; nom du mois court
        lmn,             ///< month long name ;nom du mois long (complet)
        syn,             ///< short year number { without century } ; ann&eacute;e, sans le si&egrave;cle
        lyn,             ///< year including centry ; ann&eacute;e incluant le si&egrave;cle
        hour,            ///< hour 0-12 style ; heure notation 1-12 + am|pm
        ampm,            ///< hour 0-12 style ; heure notation 1-12 + am|pm
        hour24,          ///< hour 24 style ; heure notation 24 heures
        thour,           ///< %T 24h format: %H:%M:%S
        minute,          ///< minutes
        seconds,         ///< seconds ; secondes
        //        inc,           ///< desactivated.   ///< indentation - increment; incr&eacute;menter l'indentation
        //        dec,                ///< indentation -  decrement ; d&eacute;cr&eacute;menter l'indentation
        code,            ///< enters {basic, supported} code mode (highlighting) using highlighter
        endcode,         ///< leave code-mode.
        para,            ///< param&eagrave;tres : &agrave; consid&eacute;rer.
        endpara,
        eol,             ///< Explicit End Of Line. Thus, the proper eol code corresponding to the mode, is appended to _text.
        nil              ///< null object; used as counting/bound/end value ; nulle
    };

    enum class mode : uint8_t
    {
        none = 0,   ///< unformated.
        plain,      ///< unformated.
        ansi,       ///< Ainsi standard format - First TDD: Using utils::notification "stack" for output.
        html,       ///< format html.
        vcell,     ///< format Video Cell Bitmap Bloc  ( ncurses ).
    }_mode = mode::none;

    /*!
        * @brief "Machine State" 'state' enums
        * <ul>
        * @li noop: Nowhere or unassigned.
        * @li begin: At the beginning state
        * @li wait: Some sub-log process called.
        * @li end: This instance has finished to build its internal text.
        *
        * @note Pour l'instant il n'est pas pr&eacute;vu une impl&eacute;mentation compl&egrave;te...
        */
    enum class mst : uint8_t
    {
        noop,
        begin,
        wait,
        end
    }_m = mst::noop;

    using collection = std::vector<logfmt>;
    using stack = std::stack<logfmt>;

    logfmt() = default;
    virtual ~logfmt();

    virtual return_code begin();
    virtual return_code end();
    virtual bool set_noop();
    virtual bool set_begin();
    virtual bool set_wait();
    virtual bool set_end();
    
    virtual bool set_fmt_mode(logfmt::mode m);

protected: virtual logfmt& operator < (logfmt::mst st);
public:

    logfmt& operator << (logfmt::color c);
    virtual logfmt& eol();

    template<typename T> logfmt& operator << (const T& _d)
    {
        _text << _d;
        return  *this;
    }

    std::string operator()() { return _text(); }

private:
    //static logfmt::mode _mode;
    attrib           _attr = attrib::normal;
};

class logbook_api logdebug : public logfmt
{};

class logbook_api loginfo : public logfmt
{};

class logbook_api logerror : public logfmt
{};

class logbook_api logfatal : public logfmt
{};

class logbook_api logwarning : public logfmt
{};

class logbook_api lognote : public logfmt
{};

class logbook_api logsucces : public logfmt
{};

class logbook_api logfail : public logfmt
{};

#define use_logfmt_attr using attr = logfmt::attrib;
}
