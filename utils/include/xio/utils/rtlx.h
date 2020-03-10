#pragma once

#include<xio/utils/xstr.h>
#include <map>
#include <xio/utils/expect>

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
#define EXPORT   xio::utils::rtlx::interface_map export_symbols()
#define CREATE_INSTANCE_C_IMPL xio::utils::rtlx::shared  create_instance(const std::string &aId)
#define DELETE_INSTANCE_C_IMPL void  delete_instance(xio::utils::rtlx::shared _dll_instance)


 
    /*!
        * @brief Base class of the external plugin instance for a Very,Very,VERY BASIC plugin {rtlx: "Runtime Load and Execute"} ) support.
        *
        * Derived xplugin instance are the actual plugin code that is loaded from a dynamic library file.
        *
        * @author Serge Lussier, lussier.serge@gmail.com (Poor Lonesome Coder; bretzel; bretzelus; lordvorace;...)
        *
        * @copy 2012-2020, Serge Lussier, lussier.serge@gmail.com (Poor Lonesome Coder; bretzel; bretzelus; lordvorace;...)
        * (teacc.club)
        *
        */
    class XIOUTILS_API rtlx
    {
        std::string _id;

        friend class rtloader;

    public:
        using interface_map = std::map<std::string, void*>;
        using shared = std::shared_ptr<rtlx>;
        using unique = std::unique_ptr<rtlx>;
        using weak   = std::weak_ptr<rtlx>;

    protected:
        interface_map _interface;

    public:
        rtlx() = delete;
        rtlx(const rtlx&) = delete;
        explicit rtlx(const std::string& a_id) : _id(a_id) {}
        virtual ~rtlx() {};

        template<typename _Ret, typename ..._Args> expect<_Ret> call_proc(const char* _procid, _Args..._args)
        {
            using F = _Ret(*)(rtlx::shared, _Args..._args);
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
