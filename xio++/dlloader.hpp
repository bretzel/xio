/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2017  Serge Lussier <lussier.serge@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#pragma once



#include <map>
#ifdef _WIN32
    #include <Windows.h>
#else
    #define FARPROC     void*
    #define HINSTANCE   void*
    #include <dlfcn.h>
#endif
#include <xio++/object.hpp>
#include <xio++/message.hpp>


namespace xio {
namespace dl {

    #define BEGIN_EXPORT      extern "C"{
    #define EXPORT_ENDBLOC    }

    #define CREATE_PLUGIN_SYM "_create_"
    #define DELETE_PLUGIN_SYM "_delete_"
    #define EXPORT_SYM    "_export_"     
    #define EXPORT   tea::dl::plugin::interface_t EXPORT_SYM()
    #define CREATE_PLUGIN_INSTANCE tea::dl::plugin* CREATE_PLUGIN_SYM(tea::object* a_parent, const std::string &a_id)
    #define DELETE_PLUGIN_INSTANCE void DELETE_PLUGIN_SYM(tea::dl::plugin* a_plugin)

class  xio_api plugin : public object {

    friend class dlloader;
public:
    using  interface_t = std::map<std::string, FARPROC>;
    interface_t _interface;
private:
    void setinterface(const interface_t& a_xi);
    interface_t& plugin_interface() { return _interface; }

public:
    plugin() = delete;
    plugin(const plugin&) = delete;

    plugin(object* a_parent, const std::string& a_id);
    virtual ~plugin();

    template<typename _R, typename ..._A>  _R proc(const std::string& f_id, _A...a_args) {
        using F = _R(*)(plugin*, _A...a_args);

        interface_t::iterator f = _interface.find(f_id);
        if (f == _interface.end())
            throw message::push(message::xclass::error), " - Symbol '", f_id.c_str(), "': not found.";

        F fn = reinterpret_cast<F>(f->second);
        if (fn)
            return fn(this, a_args...);
        throw message::push(message::xclass::error), " - Symbol '" , f_id.c_str() , "': has no proc address.";
    }

    virtual int run() = 0;


};


class xio_api dlloader final : public object
{

    HINSTANCE _sohandle = nullptr;

    std::string id;
    static plugin::interface_t _null;

    plugin*     mplugin = nullptr;

    plugin::interface_t _interface;

public:
    dlloader() {}
    dlloader(object* a_parent, const std::string& a_id);
    ~dlloader();

    std::string findlocation();
    plugin* open();
    int     close();
    int     release();

};

}
}


