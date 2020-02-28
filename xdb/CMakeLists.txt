

option(BUILD_SQLITEDB "Enable building sqlitedb" ON)
option(BUILD_MYSQLDB  "Enable building mysql support" OFF)
option(BUILD_PGSQL    "Enable building postgresql support" OFF)

if(BUILD_SQLITEDB)
    add_subdirectory(sqlite3)
endif()
if(BUILD_MYSQLDB)
    add_subdirectory(mysqldb)
endif()
if(BUILD_PGSQL)
    add_subdirectory(pgsqldb)
endif()

