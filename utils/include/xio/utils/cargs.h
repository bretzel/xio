//
// Created by bretzel on 20-04-13.
//

//#ifndef LIBXIO_CARGS_H
//#define LIBXIO_CARGS_H
#pragma once

#include <xio/utils/xstr.h>
#include <xio/utils/expect.h>
#include <xio/utils/journal.h>

namespace teacc::utils
{

/*!
 * @brief Simple support for command-line arguments processing template class
 *
 * I've started to get curious about command-line args processing, and this class is what I think can be interresting to write a simple
 * command-line args with callbacks in a single header file
 * But this requires to delacre a class and define the callback functions with the switches.
 *
 * ...But still, it works! :-)
 *
 * @author  &copy; 2012,2020, Serge Lussier (lussier.serge@gmail.com)
 *
 *
 *
 */
template<typename ClassT> class cargs
{
    
    ClassT*             _obj = nullptr;
    //xstr::collection    _args_text;
    using proc_fn = result_code (ClassT::*)(const std::string&);
    
public:

    struct arg
    {
        
        using collection = std::vector<arg>;
        // ------------ Attribnutes -----------------------------------------------------------------------
        std::string _name;
        bool        _opt = true;
        char        _s   = 0;
        int         _argc = -1; ///< -1 = unlimited arguments to this arg; 0 = no args; >0 = 1+ args
        std::string _desc; ///< Description text;
        // ------------------------------------------------------------------------------------------------
        cargs<ClassT>::proc_fn _proc = nullptr;
        
        xstr::collection _args;
        
        arg() = default;
        arg(const std::string& _argname, char s, bool opt, int _arg_count,cargs<ClassT>::proc_fn pfn, const std::string& _descr):
        _name(std::move(_argname)), _s(s), _opt(opt), _argc(_arg_count), _proc(pfn), _desc(_descr){}
    
        arg(char s, bool opt, int _arg_count, cargs<ClassT>::proc_fn pfn, const std::string &_descr) :
        _name(""), _s(s), _opt(opt), _argc(_arg_count), _proc(pfn), _desc(_descr){}

        arg(const arg& _a)
        {
            _name = _a._name;
            _opt = _a._opt;
            _s = _a._s;
            _argc = _a._argc;
            _proc = _a._proc;
            _desc = _a._desc;
        }
        arg(arg &&_a) noexcept
        {
            _name = std::move(_a._name);
            _opt = std::move(_a._opt);
            _s = std::move(_a._s);
            _argc = std::move(_a._argc);
            _proc = std::move(_a._proc);
            _desc = std::move(_a._desc);
        }
    
        arg& operator =(const arg& _a)
        {
            _name = _a._name;
            _opt = _a._opt;
            _s = _a._s;
            _proc = _a._proc;
            _argc = _a.argc;
            _desc = _a._desc;
            return *this;
        }
        
        arg& operator =(arg&& _a) noexcept
        {
            _name = std::move(_a._name);
            _opt = std::move(_a._opt);
            _s = std::move(_a._s);
            _proc = std::move(_a._proc);
            _argc = std::move(_a.argc);
            _desc = std::move(_a._desc);
            return *this;
        }
        ~arg()
        {
            _args.clear();
        }
        std::string operator()()
        {
            utils::xstr str;
            str
            << "{\n"
            << "         long switch:" << _name << "\n"
            << "            short ID:" << _s <<  "\n"
            << "            optional:" << (_opt ? "yes" : "no") << "\n"
            << "required num of args:" << _argc << '\n'
            << "                text:" << _desc << "\n}";
            return str();
        }
        
    };
    
    typename arg::collection _args;
    typename arg::collection::iterator _c;
    
    cargs() = default;
    
    cargs&  operator << (const arg& _a) noexcept
    {
        _args.push_back(_a);
        return *this;
    }
    
    
    auto begin() { return _args.begin(); }
    auto end()   { return _args.end(); }
    
    auto find(const std::string& _in_args)
    {
        std::string token;
        auto ci = begin();
        if(_in_args.find_first_of("--",0) == 0)
            token = "--" + _in_args;
        else if(_in_args.find_first_of('-', 0) == 0)
            token = "-" + _in_args;
        else
            token = _in_args;

        while(ci != end())
        {
        
        }
    }
    result_code process(int argc, char** argv)
    {
        if(_args.empty())
            return {( teacc::utils::notification::push(), "Command line arguments processor is empty")};
        if(argc < 2)
            return {(teacc::utils::notification::push(), "No command line arguments given (argc = ", argc, ')')};
        
        _c = _args.begin();
        int i = 1; // First argument;
        
        do
        {
            
            logdebugfn << ": " << utils::journal::White << i << "(" << std::strlen(argv[i]) << "): '" << utils::journal::Yellow << argv[i++] << utils::journal::White << "'\n" << ends;
            arg& a = find(argv[i]);
            
            
        }while(i<argc);
        
        logdebugpfn << " dump arguments:" << ends;
        for(auto argument : _args)
        {
            loginfo << utils::journal::brk() <<  argument() << ends;
        }
        //...
        
        return notification::code::implement;
    }
    
};


}
//#endif //LIBXIO_CARGS_H
