#ifndef __MESSAGEHANDLE_H__
#define __MESSAGEHANDLE_H__

#include <unordered_map>
#include <memory>
#include <functional>
#include <iostream>
#include "stdShared.h"
#include "stdTcpServer.h"
#include "function.h"

/* 映射函数表 */
class MessageHandler
{
public:
    using HandlerFunction = std::function<void(const string&)>;

    /* 构造函数 */
    MessageHandler(const StdTcpSocketPtr & client);

    /* 析构函数 */
    ~MessageHandler();
public:
    /* 处理信息 */
    void handleMessage(const string& msg);

private:
    Function m_bussinssFunc;
    /* 消息类型到处理函数的映射 */
    std::unordered_map<int, HandlerFunction> m_handles;
};



#endif // __MESSAGEHANDLE_H__