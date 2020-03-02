#pragma once


#include <xio/logbook/logfmt.h>



namespace xio::logbook

{

class logbook_api writer
{
public:

   
    enum class tagtype : uint8_t
    {
        single,
        bloc
    };


    enum class object : uint8_t {
        code,            ///< enters {basic, supported} code mode (highlighting) using highlighter
        endcode,         ///< leave code-mode.
        para,            ///< param&eagrave;tres : &agrave; consid&eacute;rer.
        endpara,
        eol,             ///< Explicit End Of Line. Thus, the proper eol code corresponding to the mode, is appended to _text.
        nil              ///< null object; used as counting/bound/end value ; nulle
    };


};

}
