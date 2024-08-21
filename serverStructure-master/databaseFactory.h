#ifndef __DATABASEFACTORY_H__
#define __DATABASEFACTORY_H__

#include "stdDataBase.h"
#include "sqliteDataBase.h"
#include "mysqlDataBase.h"
#include <memory>
#include <mutex>

enum class DatabaseType
{
    SQLite,
    MySQL
};

class DatabaseFactory
{
public:
    // 获取单例实例
    static DatabaseFactory& getInstance();

    // 禁用拷贝构造和赋值操作符
    DatabaseFactory(const DatabaseFactory&) = delete;
    DatabaseFactory& operator=(const DatabaseFactory&) = delete;

public:
    std::shared_ptr<Database> getDatabase(DatabaseType type);
private:
    DatabaseFactory() = default;
    ~DatabaseFactory() = default;

    std::shared_ptr<Database> database_;
    std::mutex mutex_; // 用于保护数据库实例的线程安全
};

#endif // __DATABASEFACTORY_H__
