#include "sqliteDataBase.h"
#include <iostream>

SQLiteDatabase::SQLiteDatabase() : db_(nullptr) {}

SQLiteDatabase::~SQLiteDatabase()
{
    close();
}

bool SQLiteDatabase::connect(const std::string& connectionString)
{
    if (sqlite3_open(connectionString.c_str(), &db_) != SQLITE_OK)
    {
        std::cerr << "Failed to open SQLite database: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    return true;
}

QueryResult SQLiteDatabase::query(const std::string& sql)
{
    QueryResult result;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare SQL query: " << sqlite3_errmsg(db_) << std::endl;
        return result;
    }

    // 获取列名
    int columnCount = sqlite3_column_count(stmt);
    for (int i = 0; i < columnCount; ++i)
    {
        result.columnNames.push_back(sqlite3_column_name(stmt, i));
    }

    // 获取行数据
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::vector<std::string> row;
        for (int i = 0; i < columnCount; ++i)
        {
            const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.push_back(text ? text : "");
        }
        result.rows.push_back(row);
    }

    sqlite3_finalize(stmt);
    return result;
}

bool SQLiteDatabase::execute(const std::string& sql)
{
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Failed to execute SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void SQLiteDatabase::close()
{
    if (db_)
    {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}
