#include "stdTcpServer.h"
#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <cctype>
#include <arpa/inet.h>




/* StdTcpSocket implementation */
/* 构造函数 */
StdTcpSocket::StdTcpSocket() : m_sockAttr(std::make_unique<StdTcpSocketPrivate>())
{
    /* 通信句柄 */
    m_sockAttr->connfd = -1;
    /* 通信是否建立 */
    m_sockAttr->m_connected = false;
}

/* 析构函数 */
StdTcpSocket::~StdTcpSocket()
{
    if (m_sockAttr->connfd > 0)
    {
        /* 通信句柄 */
        close(m_sockAttr->connfd);
    }
}

/* 连接服务器 */
int StdTcpSocket::connectToServer(const char * ip, int port)
{
    /* 创建套接字 */
    m_sockAttr->connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockAttr->connfd == -1)
    {
        perror("socket error:");
        throw std::runtime_error("Socket creation failed");
    }
    

    /* 连接服务器 */
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip, &(serverAddress.sin_addr.s_addr));
    
    int ret = connect(m_sockAttr->connfd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress));
    if (ret != 0)
    {
        perror("connect error:");
        throw std::runtime_error("Connection failed");
    }

    m_sockAttr->m_connected = true;
    return 0;
}

/* 是否连接成功 */
bool StdTcpSocket::isConnected() const
{
    return m_sockAttr->m_connected;
}

/* 发送信息 */
int StdTcpSocket::sendMessage(const void * sendMsg, size_t n)
{
    /* 1. 先写包数据大小 */
    int writeBytes = write (m_sockAttr->connfd, (const void *)&n, sizeof(n));
    if (writeBytes < 0)
    {
        perror("write error:");
        return -1;
    }

    /* 2. 再写数据本身 */
    writeBytes = write(m_sockAttr->connfd, sendMsg, n);
    if (writeBytes < 0)
    {
        perror("write error:");
        return -1;
    }
    return writeBytes;
}

/* 发送信息 */
int StdTcpSocket::sendMessage(const std::string & sendMsg)
{
    return sendMessage(sendMsg.c_str(), sendMsg.size());
}

/* 接收信息 */
int StdTcpSocket::recvMessage(void * buf, size_t n)
{
    int readBytes = read(m_sockAttr->connfd, buf, n);
    return readBytes;
}

/* 接收信息 */
int StdTcpSocket::recvMessage(std::string & recvMessage)
{
    size_t size = 0;
    if (read(m_sockAttr->connfd, &size, sizeof(size)) <= 0)
    {
        perror("read error:");
        return -1;
    } 
    char * msg = new char[size + 1];
    /* 清除脏数据 */
    memset(msg, 0, sizeof(char) * (size + 1));


    size_t totalReceived = 0;
    while (totalReceived < size)
    {
        size_t received = read(m_sockAttr->connfd, msg + totalReceived, size - totalReceived);
        if (received <= 0)
        {
            perror("read error:");
            delete [] msg;
            return -1;
        }
        totalReceived += received;
    }
    /* 接收到的数据 */
    recvMessage = msg;
    delete [] msg;

    return totalReceived;
}

StdTcpSocketPrivate * StdTcpSocket::getSockAttr() const
{
    return m_sockAttr.get();
}

/* StdTcpServer implementation */
/* 构造函数 */
StdTcpServer::StdTcpServer() : m_tcpAttr(std::make_unique<StdTcpServerPrivate>())
{
    /* 监听套接字 */
    m_tcpAttr->sockfd = -1;
    /* 是否监听 */
    m_tcpAttr->m_isRunning = false;
}

/* 析构函数 */
StdTcpServer::~StdTcpServer()
{
    if (m_tcpAttr->sockfd > 0)
    {
        /* 关闭套接字句柄 */
        close(m_tcpAttr->sockfd);
    }
}

/* 设置监听 */
bool StdTcpServer::setListen(int port)
{
    /* 类内部维护端口信息. */
    this->m_port = port;

    /* 创建套接字 */
    m_tcpAttr->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_tcpAttr->sockfd == -1)
    {
        perror("socket error");
        throw std::runtime_error("Socket creation failed");
    }
   
    /* 设置端口复用 */
    int optVal = 1;
    int ret = setsockopt(m_tcpAttr->sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("bind error:");
        return false;
    }

    /* 绑定IP和端口 */
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    /* 地址族 */
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(m_port);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    /* 绑定 */
    ret = bind(m_tcpAttr->sockfd, reinterpret_cast<const sockaddr *>(&localAddress), sizeof(localAddress));
    if (ret != 0)
    {
        perror("bind error:");
        throw std::runtime_error("Bind failed");
    }
    
    // 给监听的套接字设置监听
    ret = listen(m_tcpAttr->sockfd, 10);
    if (ret != 0)
    {
        perror("listen error:");
        throw std::runtime_error("Listen failed");
    }

    /* 设置状态为:正在监听 */
    m_tcpAttr->m_isRunning = true;
    return true;
}

/* 接收连接 */
std::shared_ptr<StdTcpSocket> StdTcpServer::getClientSock()
{
    int clientConnfd = accept(m_tcpAttr->sockfd, NULL, NULL);
    if (clientConnfd == -1)
    {
        perror("accpet error:");
        throw std::runtime_error("Accept failed");
    }

    /* 程序到这个地方, 就说明有客户端进行连接 */
    cout << "clientConnfd:" << clientConnfd << endl;

    /* 通信类 */
    auto client = std::make_shared<StdTcpSocket>();
    /* 套接字 */
    client->getSockAttr()->connfd = clientConnfd;
    client->getSockAttr()->m_connected = true;

    return client;
}