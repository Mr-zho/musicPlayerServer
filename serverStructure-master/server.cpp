#include <iostream>
using namespace std;
#include "stdTcpServer.h"
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include "stdShared.h"
#include "function.h"
#include "messageHandler.h"
#include "threadpool.h"
#include <json-c/json.h>

#define __WUZIQI__DEFINE_   0

#define BUFFER_SIZE 1024

void * handleClientInfo(void * arg)
{
    /* 线程分离 */
    pthread_detach(pthread_self());

    StdTcpSocketPtr clientInfo = *static_cast<StdTcpSocketPtr*>(arg);

    int readBytes = 0;
    MessageHandler handles(clientInfo);

    /* 接收的缓冲区 */
    string buffer;
    while (1)
    {
        readBytes = clientInfo->recvMessage(buffer);
        if (readBytes <= 0)
        {
            cout << "readBytes <= 0" << " connfd:" << clientInfo->getSockAttr()->connfd << endl;
            break;
        }
        else
        {
            /* 客户端有数据过来 */
            /* 执行注册好的方法 */
            handles.handleMessage(buffer);
        }

        /* 清空缓冲区内容 */
        buffer.clear();
    }

    /* 资源回收 */
    return NULL;
}

int main()
{
    /* 创建线程池对象 */
    ThreadPool pool(2, 5, 20);

    /* 创建服务器对象 */
    StdTcpServer server;

    /* 设置监听 */
    bool res = server.setListen(8080);
    if (res == false)
    {
        cout << "listen error" << endl;
        _exit(-1);
    }
    cout << "server listening..." << endl;

    int ret = 0;
    while (1)
    {
        StdTcpSocketPtr clientInfo = server.getClientSock();

#if 0
        pthread_t tid;
        ret = pthread_create(&tid, NULL, handleClientInfo, &clientInfo);
        if (ret != 0)
        {
            perror("thread create error:");
            _exit(-1);
        }
#else
        pool.addTask(handleClientInfo, new StdTcpSocketPtr(clientInfo));
#endif

        /* 休眠一下 */
        sleep(1);
    }
}