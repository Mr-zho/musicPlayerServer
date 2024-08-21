#include "mysqlDataBase.h"
#include <iostream>

MySQLDatabase::MySQLDatabase() : conn_(mysql_init(nullptr)) {}

MySQLDatabase::~MySQLDatabase()
{
    close();
}

bool MySQLDatabase::connect(const std::string& connectionString)
{
    // 解析 connectionString (例如 "host=localhost;user=root;password=my_password;db=my_db")
    std::string host = "localhost"; // 示例解析
    std::string user = "root";
    std::string pass = "my_password";
    std::string db   = "my_db";

    if (!mysql_real_connect(conn_, host.c_str(), user.c_str(), pass.c_str(), db.c_str(), 0, nullptr, 0))
    {
        std::cerr << "Failed to connect to MySQL database: " << mysql_error(conn_) << std::endl;
        return false;
    }
    return true;
}

QueryResult MySQLDatabase::query(const std::string& sql)
{
    QueryResult result;
    if (mysql_query(conn_, sql.c_str()))
    {
        std::cerr << "Failed to execute query: " << mysql_error(conn_) << std::endl;
        return result;
    }

    MYSQL_RES* res = mysql_store_result(conn_);
    if (!res)
    {
        std::cerr << "Failed to retrieve query result: " << mysql_error(conn_) << std::endl;
        return result;
    }

    int numFields = mysql_num_fields(res);
    MYSQL_ROW row;
    MYSQL_FIELD* fields = mysql_fetch_fields(res);

    // 获取列名
    for (int i = 0; i < numFields; ++i)
    {
        result.columnNames.push_back(fields[i].name);
    }

    // 获取行数据
    while ((row = mysql_fetch_row(res)))
    {
        std::vector<std::string> rowData;
        for (int i = 0; i < numFields; ++i)
        {
            rowData.push_back(row[i] ? row[i] : "NULL");
        }
        result.rows.push_back(rowData);
    }

    mysql_free_result(res);
    return result;
}

bool MySQLDatabase::execute(const std::string& sql)
{
    if (mysql_query(conn_, sql.c_str()))
    {
        std::cerr << "Failed to execute SQL: " << mysql_error(conn_) << std::endl;
        return false;
    }
    return true;
}

void MySQLDatabase::close()
{
    if (conn_)
    {
        mysql_close(conn_);
        conn_ = nullptr;
    }
}
