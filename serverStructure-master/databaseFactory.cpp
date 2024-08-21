#include "databaseFactory.h"


DatabaseFactory& DatabaseFactory::getInstance()
{
    static DatabaseFactory instance;
    return instance;
}

std::shared_ptr<Database> DatabaseFactory::getDatabase(DatabaseType type)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!database_)
    {
        switch (type)
        {
        case DatabaseType::SQLite:
            database_ = std::make_shared<SQLiteDatabase>();
            break;
        case DatabaseType::MySQL:
            database_ = std::make_shared<MySQLDatabase>();
            break;
        }
    }
    return database_;
}
