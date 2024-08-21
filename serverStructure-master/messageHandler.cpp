#include "messageHandler.h"
#include <json-c/json.h>

/* 构造函数 */
MessageHandler::MessageHandler(const StdTcpSocketPtr & client): m_bussinssFunc(client)
{
    /* 注册消息处理函数 */
    m_handles[REGISTER] = [this](const string& msg) { m_bussinssFunc.handleRegisterInfo(msg); };
    m_handles[LOGIN] = [this](const string& msg) { m_bussinssFunc.handleLoginInfo(msg); };
    m_handles[ONLINEMuSIC] = [this](const string& msg) { m_bussinssFunc.handleOnlineMusicInfo(msg); };

    /* todo... 添加更多处理函数 */
}

/* 析构函数 */
MessageHandler::~MessageHandler()
{

}


void MessageHandler::handleMessage(const string& msg)
{
    int type = 0;
    /* 现在msg是json字符串 */
    /* 1.将json字符串转成json对象 */
    json_object * jsonObj = json_tokener_parse(msg.c_str());
    if (jsonObj != NULL)
    {
        /* 2.根据key得到value. */
        type = json_object_get_int(json_object_object_get(jsonObj, "type"));
    }

    auto iter = m_handles.find(type);
    if (iter != m_handles.end())
    {
        /* 执行回调函数 */
        iter->second(msg);
    }
    else
    {
        // 处理未知的消息类型
        std::cout << "Unknown message type: " << type << std::endl;
    }
}