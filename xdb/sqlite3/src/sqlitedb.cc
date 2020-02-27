#include <xio/sqlite3/sqlitedb.h>
#include <xio/utils/xstr.h>
#include <xio/utils/xreturn>

#ifdef  _WIN32
#   include <Windows.h>
#   include <Shlwapi.h>
#endif

namespace xio::xdb
{


    sqlitedb::sqlitedb(std::string&& a_dbname) noexcept
    {
        utils::xstr str = std::move(a_dbname);
        _dbname = str();
        str << ".sqlite3";

        std::cout << __FUNCTION__ << ": db file: [" << str() << "] \n\n";
#ifdef  _WIN32
   
        if (!PathFileExistsA(str().c_str()))
        {
            utils::notification::push(), " sqlite3 open db error(", _dbname, ") - no such database file.\n";
            return;
        }
#endif
        int res = sqlite3_open(str().c_str(), &_db);
        if(res != SQLITE_OK)
        {
            std::cout << sqlite3_errmsg(_db) << "\n";
            utils::notification::push(), " sqlite3 open db error(", res, ") - ", sqlite3_errmsg(_db);
            return;
        }
        
        std::cout << "(" << res << ")" <<  sqlite3_errmsg(_db) << "\n";
    }

    sqlitedb::~sqlitedb()
    {
        if (_db)
        {
            sqlite3_close(_db);
            _db = nullptr;
            _dbname.clear();
        }
    }

}