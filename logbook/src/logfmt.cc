#include <xio/logbook/logfmt.h>


namespace xio::logbook
{

    

    logfmt::~logfmt()
    {
        _text.clear();
    }


    bool logfmt::set_noop()
    {
        _m = mst::noop;
        return false;
    }


    bool logfmt::set_begin()
    {
        _m = mst::begin;
        return false;
    }


    bool logfmt::set_wait()
    {
        _m = mst::wait;
        return false;
    }


    bool logfmt::set_end()
    {
        _m = mst::end;
        return false;
    }

    bool logfmt::set_fmt_mode(book::mode m)
    {
        _mode = m;
        return true;
    }


    logfmt& logfmt::operator<(logfmt::mst st)
    {
        ///@todo Check, validate
        _m = st;
        //...
        return *this;
    }
    /*!
    
        @brief insert object format.
    
    */
    logfmt& logfmt::operator<<(book::object obj)
    {
        // TODO: insérer une instruction return ici
    }

    logfmt& logfmt::operator<<(webcolor c)
    {
        
        _text << __PRETTY_FUNCTION__ << ": not implemented yet!";
        return *this;
    }


    logfmt::return_code logfmt::begin()
    {
        ///@todo Check current state; Check

        set_begin();
        _text.clear();    
        return utils::notification::code::ok;
    }


    logfmt::return_code logfmt::end()
    {
        return utils::notification::code::ok;
    }

    logfmt& logfmt::eol()
    {
        _text << '\n';
        return *this;
    }
}
