#pragma once

#include <xio++/xio++.hpp>
//#include <time.h>
#include <xio++/string_t.hpp>
#include <ctime>
#include <chrono>



namespace xio {

class xio_api time_stamp
{
    std::chrono::system_clock::time_point _stamp;
    std::time_t _clock;
    string_t  _text;
    std::string _fmt = "%T";//"%H:%M:%S";
public:

    time_stamp();
    time_stamp(time_stamp&& xs);
    time_stamp(const time_stamp& xs);

    ~time_stamp();

    time_stamp& operator *();
    std::string text(const std::string& a_format_str = "");

    time_stamp& operator=(time_stamp&& xs);
    time_stamp& operator=(const time_stamp& xs);


};

}

