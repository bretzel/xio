
#include <xio/utils/xplugin.h>

//
// Created by bretzel on 20-01-27.
//



namespace xio::utils
{
    void* xplugin::get_proc(const char* _procid)
    {
        auto f = _interface.find(_procid);
        if (f == _interface.end())
            return nullptr;

        return _interface[_procid];
    }

}