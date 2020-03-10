//
// Created by bretzel on 20-01-27.
//


#pragma once
#include <xio/utils/rtlx.h>
#include <xio/utils/expect>

#include <map>

#ifdef _WIN32
    #include <Windows.h>
#else
    #define FARPROC     void*
    #define HINSTANCE   void*
    #include <dlfcn.h>
#endif

namespace xio::utils
{


class XIOUTILS_API rtloader final
{
    HINSTANCE _handle = nullptr;
    std::string _id;
    static rtlx::interface_map _null;
    rtlx::shared _plugin = nullptr;
    rtlx::interface_map _interface;
    
public:
    
    using ret = expect<rtlx::shared>;
    
    rtloader() = default;
    
    rtloader(std::string&& aId) noexcept : _id(std::move(aId))
    {}

    ~rtloader() = default;
    std::string locate();
    ret open();
    int close();
    int release();
};


}

