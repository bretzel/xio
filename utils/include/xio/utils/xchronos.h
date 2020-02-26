//
// Created by bretzel on 20-01-27.
//

//#ifndef PLSC_CHRONOST_H
//#define PLSC_CHRONOST_H

#pragma once

#include <xio/utils/xstr.h>

#include <chrono>

namespace xio::utils
{
/*!
 * @brief Own time management class.
 * @author &copy; 2018, 2019, Serge Lussier teacc.club, lussier.serge@gmail.com
 *
 * @todo Satisfy my ambitions to implement \c chronos_t and \c date_t arithmetics just like that PHP lang. !! ouahahahah!
 *
 */

class XIOUTILS_API xchronos
{
    std::chrono::system_clock::time_point _stamp;
    std::time_t _clock = 0L;
    xstr _text;
    std::string _fmt = "%T";//"%H:%M:%S";

public:
    
    xchronos() = default;
    xchronos(xchronos&& xs) noexcept;
    xchronos(const xchronos &xs);
    ~xchronos();
    xchronos &operator*();
    std::string text(const std::string &a_format_str = "");
    xchronos &operator=(xchronos &&xs) noexcept;
    xchronos &operator=(const xchronos &xs);
};


}

//#endif //LSC_CHRONOST_H
