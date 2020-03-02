#pragma once

#include <xio/logbook/logbookapi.h>
#include <xio/utils/xstr.h>
#include <stack>
#include <vector>
#include <xio/utils/xreturn>
#include <xio/logbook/colors.h>
#include <xio/logbook/book.h>
#include <xio/utils/xchronos>


namespace xio::logbook
{




class logbook_api logfmt
{
protected:
    utils::xstr _text;

public:

    using return_code = utils::xreturn<utils::notification::code>;


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

    enum object {
        code,            ///< enters {basic, supported} code mode (highlighting) using highlighter
        end,         ///< leave code-mode.
        p,            ///< param&eagrave;tres : &agrave; consid&eacute;rer.
        eol,             ///< Explicit End Of Line. Thus, the proper eol code corresponding to the mode, is appended to _text.
        nil              ///< null object; used as counting/bound/end value ; nulle
    };


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
    
    virtual bool set_fmt_mode(book::mode m);

protected: virtual logfmt& operator < (logfmt::mst st);
public:


    logfmt& operator << (book::object obj);

    logfmt& operator << (logbook::webcolor c);
    virtual logfmt& eol();

    template<typename T> logfmt& operator << (const T& _d)
    {
        _text << _d;
        return  *this;
    }

    std::string operator()() { return _text(); }

private:
    book::mode _mode;
    book::attrib           _attr = book::attrib::normal;
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
