//
// Created by bretzel on 20-01-27.
//

#include <xio/utils/xloader.h>


#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

namespace xio::utils
{


#ifdef _WIN32
std::string xloader::locate()
{
    xstr  dll_file = "%s.dll";
    dll_file << _id;
    WIN32_FIND_DATA FindFileData;
    #ifdef _UNICODE
    if (HANDLE handle = FindFirstFile((LPCWSTR)dll_file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
    #else
    #ifdef UNICODE
        if (HANDLE handle = FindFirstFile((LPCWSTR)dll_file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
        #else
        if (HANDLE handle = FindFirstFile((LPCSTR)dll_file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
        #endif
        #endif
    FindClose(handle);
    return dll_file();
}



/*!
    @brief Opens the library at the found location into the filesystem.
    @return pointer to the Plugin base class lobject successfuly initialized and ready to use.
    @author Serge Lussier (bretzelus), lussier.serge@gmail.com
*/

xloader::code xloader::open()
{
    std::string str_location = locate();
    if (str_location.empty()) {
        return {(
            notification::push(),
            notification::type::error,' ',
            "Library file '", _id, "' ",
            notification::code::notfound,
            " Within the system paths."
        )};
        //throw  notification::Push(notification::T::error), id, " not found into valid paths.";
    }
    #if defined(_UNICODE) || defined(UNICODE)
    _handle = LoadLibrary((LPCWSTR)str_location.c_str());
    #else
    _handle = LoadLibrary((LPCSTR)str_location.c_str());

    #endif
    if (!_handle) {
        // ---------------------------------------------------------------------------------------
        //https://stackoverflow.com/a/17387176 --
        // Modified.
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string SysMsg(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);
        // ---------------------------------------------------------------------------------------
        return {(
            notification::push(),
            notification::type::error,
            notification::code::null,
            "Openning ", _id, "Library file: [",SysMsg, "]."
        )};
    }
        //throw  notification::Push(notification::T::error), id, ": dll error";

    //GetProcAddress;
    std::cerr << "Library '" << str_location << "' open, Getting __Export ProcAddress:\n";
    void* _export_fn = nullptr;
    _export_fn = (void*)GetProcAddress(_handle, EXPORT_SYM);
    if (!_export_fn) {
    LeaveWithNoInterface:
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string SysMsg(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);
        FreeLibrary(_handle);
        
        return {(
            notification::push(),
            notification::type::error,' ',
            " - Plugin ", _id, ": does not - or failed to - provides interface:  ", SysMsg
        )};
        //throw notification::Push(notification::T::error), " Plugin ", id, ": Plugin does not provides exported interface.";
    }
    std::cerr << "Library '" << str_location << "' calling __Export :\n";
    _interface = xplugin::interface_map(reinterpret_cast<xplugin::interface_map(*)()>(_export_fn)());
    if (_interface.empty()) {
        FreeLibrary(_handle);
        goto LeaveWithNoInterface;
    }

    for (auto& ix : _interface) {
        std::cerr << "Exported Sym: '" << ix.first << "'\n";
        ix.second = GetProcAddress(_handle, ix.first.c_str());
        if (!ix.second) {
            FreeLibrary(_handle);
            return {(
                notification::push(),
                notification::type::error,
                notification::code::null,
                "- Plugin '", _id, " has no `exported` symbol identified by '", ix.first,"', while listed in its exported interface."
            )};
            //throw notification::Push(notification::T::error), " Plugin ", id, ": exported symbol is unbound: [", ix.first, "]";
        }
    }
    if ((_plugin = (reinterpret_cast<xplugin * (*)(const char*)>(_interface[CREATE_SYM]))(_id.c_str())) != nullptr) {
        _plugin->set_interface(_interface);
        _interface.clear(); // Do not keep a local copy since the interface data is now owned by the Plugin Instance...
    }
    else {
        FreeLibrary(_handle);
        return { (
            notification::push(),
            notification::code::null," :" ,
            " Plugin ", _id, " has no exported __Create symbol.\n"
        )};
    }
    return { _plugin };
}

int xloader::close()
{
    if (!_handle) return 0;
    if (_plugin) release();
    FreeLibrary(_handle);
    _handle = nullptr;
    return 0;
}

int xloader::release()
{
    if (!_plugin)
        return 0;

    (void) reinterpret_cast<int(*)(xplugin*)>(_plugin->_interface[DELETE_SYM])(_plugin);
    _plugin = nullptr;
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
std::string xloader::locate()
{
    xstr::word::collection a_paths;
    std::string str_format = "%s/lib%s.so";
    std::string str_file;
    xstr file = str_format;
    const char *cpath = getenv("LD_LIBRARY_PATH");
    xstr paths;
    if(!cpath)
        paths = "./";
    else
        paths << cpath << ":.";
    
    paths.words(a_paths, ":");
    
    for(xstr::word p : a_paths)
    {
        file = str_format;
        std::string w = p();
        std::string::size_type pos = w.find(':');
        if(pos != std::string::npos)
            w.erase(pos, 1);
        std::cout << " checking path: [" << w << "]:\n";
        file << w << _id;
        str_file = file();
        std::cout << " checking path: [" << str_file << "]:\n";
        if(!access(str_file.c_str(), F_OK))
        {
            std::cout << " Location found: [" << str_file << "]:\n";
            return str_file;
        }
    }
    return "";
}

/*!
 * @brief Opens the library at the found location into the filesystem.
*
*    @return pointer to the Plugin base class lobject successfuly initialized and ready to use.
*    @author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
xloader::code xloader::open()
{
    
    std::string str_location = locate();
    if(str_location.empty())
    {
        return {(notification::push(), notification::type::error, " - ", " Plugin ", _id, " not found into valid paths.")};
        //throw notification::Push(notification::Type::error), id, " not found into valid paths.";
    }
    _handle = dlopen(str_location.c_str(), RTLD_LAZY); // Recommended
    if(!_handle)
        return {(notification::push(notification::type::error), ": ", dlerror())};
    
    void *_export_fn = dlsym(_handle, EXPORT_SYM);
    if(!_export_fn)
    {
        dlclose(_handle);
        return {(notification::push(notification::type::error), " Plugin ", _id, ": ", dlerror())};
    }
    
    _interface = reinterpret_cast<xplugin::interface_map(*)()>(_export_fn)();
    if(_interface.empty())
    {
        dlclose(_handle);
        return {(notification::push(notification::type::error), " Plugin ", _id, " does not export its interface! ")};
    }
    
    for(auto &ix : _interface)
    {
        ix.second = dlsym(_handle, ix.first.c_str());
        if(!ix.second)
        {
            dlclose(_handle);
            return {(notification::push(notification::type::error), " Plugin ", _id, ": exported symbol is unbound: [", ix.first, "]")};
        }
    }
    
    _plugin = (reinterpret_cast<xplugin *(*)()>(_interface[CREATE_SYM]))();
    _plugin->set_interface(_interface);
    _interface.clear();
    return {_plugin};
}

/*!
*    @brief Closes the library, and destroys its Plugin instance.
*
*    @author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
int xloader::close()
{
    if(!_handle)
        return 0;
    if(_plugin)
        release();
    dlclose(_handle);
    _handle = nullptr;
    return 0;
}

int xloader::release()
{
    if(!_plugin)
        return 0;
    
    (void) reinterpret_cast<int (*)(xplugin *)>(_plugin->_interface[DELETE_SYM])(_plugin);
    _plugin = nullptr;
    
    return 0;
}
#endif

}

