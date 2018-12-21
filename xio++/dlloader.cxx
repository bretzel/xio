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
 

#include "dlloader.hpp"
#include "string_t.hpp"

#ifndef _WIN32
    #include <unistd.h>
    #define _access access
#endif

namespace xio {
namespace dl {

plugin::interface_t dlloader::_null;
plugin::plugin(object* a_parent, const std::string&) :object(a_parent) {}

plugin::~plugin()
{
}

void plugin::setinterface(const plugin::interface_t& a_xi)
{
    _interface = a_xi;
}
dlloader::dlloader(object* a_parent, const std::string& a_id) :object(a_parent),
id(a_id)
{
}


dlloader::~dlloader()
{
}

#ifdef _WIN32	
std::string dlloader::findlocation()
{
    string_t  dll_file = "%s.dll";
    dll_file.arg(id);
    WIN32_FIND_DATA FindFileData;
    if (HANDLE handle = FindFirstFile((LPCSTR)dll_file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
        FindClose(handle);
    return dll_file();
}

/*!    @brief Opens the library at the found location into the filesystem.

    @return pointer to the Plugin base class object successfuly initialized and ready to use.
    @author Serge Lussier (bretzelus), lussier.serge@gmail.com
*/
plugin * dlloader::open()
{

    std::string str_location = findlocation();
    if (str_location.empty()) {
        throw  message::push(message::xclass::error), id ," not found into valid paths.";
    }
    _sohandle = LoadLibrary((LPCSTR)str_location.c_str());
    if (!_sohandle)
        throw  message::push(message::xclass::error),id,": dll error";

    //GetProcAddress;
    FARPROC _export_fn = nullptr;
    _export_fn = GetProcAddress(_sohandle, EXPORT_SYM);
    if (!_export_fn) {
        FreeLibrary(_sohandle);
        throw message::push(message::xclass::error) ," Plugin " ,id ,": Plugin does not provides exported interface.";
    }

    _interface = reinterpret_cast<plugin::interface_t(*)()>(_export_fn)();
    if (_interface.empty()) {
        FreeLibrary(_sohandle);
        throw  message::push(message::xclass::error), " Plugin " ,id ," does not export its interface! ";
    }

    for (auto& ix : _interface) {
        ix.second = GetProcAddress(_sohandle, ix.first.c_str());
        if (!ix.second) {
            FreeLibrary(_sohandle);
            throw message::push(message::xclass::error) ," Plugin ", id ,": exported symbol is unbound: [",ix.first,"]";
        }
    }
    if ((mplugin = (reinterpret_cast<plugin*(*)()>(_interface[CREATE_PLUGIN_SYM]))()) != nullptr) {
        mplugin->plugin_interface() = _interface;
        _interface.clear(); // Do not keep a local copy since the interface data is now owned by the Plugin Instance...
    }
    return mplugin;
}


/*!
    @brief Closes the library, and destroys its Plugin instance.

    @author Serge Lussier (bretzelus), lussier.serge@gmail.com
*/
int dlloader::close()
{
    if (!_sohandle) return 0;
    if (mplugin) release();
    FreeLibrary(_sohandle);
    _sohandle = nullptr;
    return 0;
}


int dlloader::release()
{
    if (!mplugin)
        return 0;

    (void) reinterpret_cast<int(*)(plugin*)>(mplugin->_interface[DELETE_PLUGIN_SYM])(mplugin);
    mplugin = nullptr;

    return 0; 
}


#else

/*!
@brief There are only two places where we can find the library :
@li Under LD_LIBRARY_PATH paths lists
@li Under the current "./" working directory.

@return the absolute path and final filename in std::string.
@author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
std::string dlloader::findlocation()
{
    string_t::word::list a_paths;
    std::string str_format = "%s/lib%s.so";
    std::string str_file;
    string_t file = str_format;
    const char* cpath = getenv("LD_LIBRARY_PATH");
    string_t paths;
    if (!cpath)
        paths = "./";
    else
        paths << cpath << ":.";

    paths.words(a_paths, ":");

    for (string_t::word p : a_paths) {
        file = str_format;
        std::string w = p();
        std::string::size_type pos = w.find(":");
        if (pos != std::string::npos)
            w.erase(pos, 1);
        std::cout << " checking path: [" << w << "]:\n";
        file.arg(w).arg(id);
        str_file = file();
        std::cout << " checking path: [" << str_file << "]:\n";
        if (!access(str_file.c_str(), F_OK)) {
            std::cout << " Location found: [" << str_file << "]:\n";
            return str_file;
        }
    }
    return "";
}


/*!
 * @brief Opens the library at the found location into the filesystem.
*
*    @return pointer to the Plugin base class object successfuly initialized and ready to use.
*    @author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
plugin * dlloader::open()
{

    std::string str_location = findlocation();
    if (str_location.empty()) {
        throw message::push(message::xclass::error) , id , " not found into valid paths.";
    }
    _sohandle = dlopen(str_location.c_str(), RTLD_LAZY); // Recommended
    if (!_sohandle)
        throw message::push(message::xclass::error) , ": " , dlerror();

    void* _export_fn = dlsym(_sohandle, EXPORT_SYM);
    if (!_export_fn) {
        dlclose(_sohandle);
        throw message::push(message::xclass::error) , " Plugin " , id , ": " , dlerror();
    }

    _interface = reinterpret_cast<plugin::interface_t(*)()>(_export_fn)();
    if (_interface.empty()) {
        dlclose(_sohandle);
        throw message::push(message::xclass::error) ," Plugin " , id , " does not export its interface! ";
    }

    for (auto& ix : _interface) {
        ix.second = dlsym(_sohandle, ix.first.c_str());
        if (!ix.second) {
            dlclose(_sohandle);
            throw  message::push(message::xclass::error), " Plugin " , id , ": exported symbol is unbound: [" , ix.first ,"]";
        }
    }

    mplugin = (reinterpret_cast<plugin*(*)()>(_interface[CREATE_PLUGIN_SYM]))();
    mplugin->setinterface(_interface);
    _interface.clear();
    return mplugin;
}


/*!
*    @brief Closes the library, and destroys its Plugin instance.
*
*    @author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
int dlloader::close()
{
    if (!_sohandle) return 0;
    if (mplugin) release();
    dlclose(_sohandle);
    _sohandle = 0;
    return 0;
}


int dlloader::release()
{
    if (!mplugin)
        return 0;

    (void) reinterpret_cast<int(*)(plugin*)>(mplugin->_interface[DELETE_PLUGIN_SYM])(mplugin);
    mplugin = nullptr;

    return 0;
}

#endif


}
}
