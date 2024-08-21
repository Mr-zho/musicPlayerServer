#ifndef __STDDATABASE_H__
#define __STDDATABASE_H__


#include <string>
#include <vector>
#include <memory>

struct QueryResult
{
    // 定义查询结果的结构
    std::vector<std::vector<std::string>> rows;
    std::vector<std::string> columnNames;
};


class Database
{
public:
    virtual ~Database() {}

    // 连接数据库
    virtual bool connect(const std::string& connectionString) = 0;

    // 执行查询语句
    virtual QueryResult query(const std::string& sql) = 0;

    // 执行非查询语句
    virtual bool execute(const std::string& sql) = 0;

    // 关闭数据库连接
    virtual void close() = 0;
};

#endif //__STDDATABASE_H__