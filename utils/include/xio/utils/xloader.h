//
// Created by bretzel on 20-01-27.
//


#pragma once
#include <xio/utils/xplugin.h>
#include <xio/utils/xreturn.h>

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


class XIOUTILS_API xloader final
{
    HINSTANCE _handle = nullptr;
    std::string _id;
    static xplugin::interface_map _null;
    xplugin* _plugin = nullptr;
    xplugin::interface_map _interface;
    
public:
    
    using code = xreturn<xplugin*>;
    
    xloader() = default;
    
    xloader(std::string&& aId) noexcept : _id(std::move(aId))
    {}

    ~xloader() = default;
    std::string locate();
    code open();
    int close();
    int release();
};


}

