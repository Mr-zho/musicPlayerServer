#ifndef __MYSQLDATABASE_H__
#define __MYSQLDATABASE_H__

#include "stdDataBase.h"
#include <mysql/mysql.h>

class MySQLDatabase : public Database
{
public:
    MySQLDatabase();
    ~MySQLDatabase();

    bool connect(const std::string& connectionString) override;
    QueryResult query(const std::string& sql) override;
    bool execute(const std::string& sql) override;
    void close() override;

private:
    MYSQL* conn_;
};

#endif // __MYSQLDATABASE_H__
