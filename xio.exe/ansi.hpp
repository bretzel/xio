#pragma once


#ifdef _WIN32
#include <Windows.h>
#endif
#include <sstream>
#include <iostream>
#include <xio++/message.hpp>
namespace Ansi{

namespace Color{

using T = const char*;

constexpr T Reset     = "\033[0m";
constexpr T Red       = "\033[0;31m";
constexpr T Green     = "\033[0;32m";
constexpr T Ambre     = "\033[0;33m";
constexpr T Blue      = "\033[0;34m";
constexpr T Magenta   = "\033[0;35m";
constexpr T Cyan      = "\033[0;36m";
constexpr T White     = "\033[1;37m";
constexpr T HGray     = "\033[1;38m";
constexpr T HRed      = "\033[1;31m";
constexpr T HGreen    = "\033[1;32m";
constexpr T Yellow    = "\033[1;33m";
constexpr T HBlue     = "\033[1;34m";
constexpr T HMagenta  = "\033[1;35m";
constexpr T HCyan     = "\033[1;36m";
constexpr T Orange    = "\033[38;5;166m";
constexpr T Endl      = "\033[0m\n";


constexpr T Ifcolor  = "\033[38;5;99m";
constexpr T Objcolor = "\033[38;5;192m";

}

#define LHead  std::cout << Color::HCyan  << __FUNCTION__
#define LFnl   std::cout << Color::HCyan  << __PRETTY_FUNCTION__ << ":" <<  __LINE__ << ":"  Color::White
#define FFnl   std::cout << Color::HCyan  "[" << __FILE__  ":"  << __LINE__  << "] " Color::White <<  __FUNCTION__  << ":"  Color::HBlue
#define Fnl    std::cout << Color::HCyan  << __FUNCTION__  << ":" <<  __LINE__ <<  ": "  Color::White
#define Warn   std::cout << Color::HCyan <<  __FUNCTION__  << ":["  <<  __LINE__ << Color::Orange  "] Warning: "   CWhite
#define LNotice   std::cout << Color::HCyan  __FUNCTION__   ":["   <<  __LINE__ <<  Color::Orange "] Notice: "   CWhite



struct ColorPair{
    Color::T Bg = Color::Reset;
    Color::T Fg = Color::White;
    
};



class  CAnsi
{
#ifdef _WIN32
    HANDLE hStdIn = nullptr;
    DWORD  mode;
#endif
    bool   F = true;
    int    _argc=0;
    char** _argv = nullptr;

    //xio::interpreter interpret;
public:
    CAnsi();
    CAnsi(int argc, char** argv);
#ifdef _WIN32
    // Must separate initialization of the console and this instance with explicit init function.
    DWORD Init();
    void End();
#endif
    
    
    
    template<typename T> CAnsi const& operator <<(const T& Arg) const{
        std::ostringstream os;
        os << Arg;
        std::cout << os.str();
        return *this;
    }


    xio::message::code execute();

    virtual ~CAnsi();
};

}

