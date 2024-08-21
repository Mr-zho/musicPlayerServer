#ifndef __STDTCPSERVER_H__
#define __STDTCPSERVER_H__

#include <iostream>
using namespace std;
#include <string>
#include <memory>


// Forward declarations
struct StdTcpSocketPrivate;
struct StdTcpServerPrivate;

// Definition of the private structures
struct StdTcpSocketPrivate
{
    /* 通信句柄 */
    int connfd;
    /* 通信是否建立成功 */
    bool m_connected;
};

struct StdTcpServerPrivate
{
    /* 监听句柄 */
    int sockfd;
    /* 服务器是否正在监听 */
    bool m_isRunning;
};

/* 通信类 */
class StdTcpSocket
{
public:
    /* 构造函数 */
    StdTcpSocket();

    /* 析构函数 */
    ~StdTcpSocket();

public:
    /* 连接服务器 */
    int connectToServer(const char * ip, int port);
    /* 是否连接成功 */
    bool isConnected() const;
    /* 发送信息 */
    int sendMessage(const std::string & sendMsg);
    /* 发送信息 */
    int sendMessage(const void * sendMsg, size_t n);
    /* 接收信息 */
    int recvMessage(std::string & recvMessage);
    /* 接收信息 */
    int recvMessage(void * buf, size_t n);
    /* 设置属性 */
    StdTcpSocketPrivate * getSockAttr() const;
private:
    /* 智能指针 */
    std::unique_ptr<StdTcpSocketPrivate> m_sockAttr;
};
/* 类型别名 */
using StdTcpSocketPtr = std::shared_ptr<StdTcpSocket>;


class StdTcpServer
{
public:
    /* 构造函数 */
    StdTcpServer();

    /* 析构函数 */
    ~StdTcpServer();
public:
    /* 设置监听 */
    bool setListen(int port);

    /* 接收连接 */
    std::shared_ptr<StdTcpSocket> getClientSock();
private:
    /* 端口 */
    int m_port;
    /* 服务器的属性 */
    std::unique_ptr<StdTcpServerPrivate> m_tcpAttr;
};


#endif //__STDTCPSERVER_H__