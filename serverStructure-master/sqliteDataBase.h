#ifndef __SQLITEDATABASE_H__
#define __SQLITEDATABASE_H__

#include "stdDataBase.h"
#include <sqlite3.h>

class SQLiteDatabase : public Database
{
public:
    SQLiteDatabase();
    ~SQLiteDatabase();

    bool connect(const std::string& connectionString) override;
    QueryResult query(const std::string& sql) override;
    bool execute(const std::string& sql) override;
    void close() override;

private:
    sqlite3* db_;
};

#endif // __SQLITEDATABASE_H__
