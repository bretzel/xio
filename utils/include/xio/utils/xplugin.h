#pragma once

#include<xio/utils/xstr.h>
#include <map>
#include <xio/utils/xreturn>

#ifdef _WIN32
#   include <Windows.h>
#else
#   define FARPROC     void*
#   define HINSTANCE   void*
#   include <dlfcn.h>
#endif


namespace xio::utils
{




#define BEGIN_EXPORT      extern "C"{
#define END_EXPORT    }

#define CREATE_SYM "create_instance"  // Exported symbol name string.
#define DELETE_SYM "delete_instance"
#define EXPORT_SYM "export_symbol"
#define EXPORT   xplugin::interface_map export_symbols()
#define CREATE_INSTANCE_C_IMPL xio::utils::xplugin*  create_instance(const std::string &aId)
#define DELETE_INSTANCE_C_IMPL void  delete_instance(xio::utils::xplugin* _dll_instance)


 
    /*!
        * @brief Base class of the external plugin instance for a Very,Very,VERY BASIC plugin framework support.
        *
        * Derived xplugin instance are the actual plugin code that is loaded from a dynamic library file.
        *
        * @author Serge Lussier, lussier.serge@gmail.com (Poor Lonesome Coder; bretzel; bretzelus; lordvorace;...)
        *
        * @copy 2012-2020, Serge Lussier, lussier.serge@gmail.com (Poor Lonesome Coder; bretzel; bretzelus; lordvorace;...)
        * (teacc.club)
        *
        */
    class XIOUTILS_API xplugin
    {
        std::string _id;

        friend class xloader;

    public:
        using interface_map = std::map<std::string, void*>;

    protected:
        interface_map _interface;

    public:
        xplugin() = delete;
        xplugin(const xplugin&) = delete;
        xplugin(std::string&& a_id) noexcept : _id(std::move(a_id)) {}
        virtual ~xplugin() {};

        template<typename _Ret, typename ..._Args> xreturn<_Ret> call_proc(const char* _procid, _Args..._args)
        {
            using F = _Ret(*)(xplugin*, _Args..._args);
            auto f = _interface.find(_procid);
            if (f == _interface.end())
                return { (
                    notification::push(), "proc name:\"", _procid,  "\" not found in \"", _id, "\" lib." // quick and dirty test....
                ) };

            F fn = reinterpret_cast<F>(f->second);
            if (fn)
                return fn(this, _args...);
        }

        void* get_proc(const char* _procid);

        void set_interface(const interface_map& _im)
        {
            _interface = _im;
        }

        virtual notification::code execute() = 0;
    };

}
