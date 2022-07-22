#pragma execution_character_set("utf-8")

#define BUF_SIZE 128
#include <iostream>
#include <thread>

#include "UWLog.h"
#include "WTcpClient.h"
#include "CanOpenUI.h"
#include "ControlMain.h"
#include "PGSQLDriveHelper.h"

using namespace std;

int tcp_client_doth(client_info *client)
{
    log_debug("tcp_client_do_conn %s %d ....",client->ip,client->port);
    WSADATA wsd;
    SOCKADDR_IN servAddr;

    int n;
    //初始化套接字动态库
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("WSAStartup failed\n");
        return -1;
    }

    //创建套接字
    client->acceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == client->acceptSocket) {
        printf("socker failed\n");
        log_debug("socker failed %s %d",client->ip,client->port);
        WSACleanup();
        return -2;
    }

    //设置服务器地址
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(client->ip);
    servAddr.sin_port = htons(client->port);
    int nServAddrLen = sizeof(servAddr);

    //连接服务器
    for(int i=0;i<3;i++){
        n = connect(client->acceptSocket, (LPSOCKADDR)&servAddr, nServAddrLen);
        if (SOCKET_ERROR == n) {
            log_debug("connect failed %d %s %d",SOCKET_ERROR,client->ip,client->port);
           Sleep(500);
            continue;
        }
        break;
    }

    if (SOCKET_ERROR == n) {
        log_debug("connect failed %d %s %d",SOCKET_ERROR,client->ip,client->port);
        WSACleanup();//释放套接字资源
        return -3;
    }
    log_debug("tcp_client_do_conn %s %d ok",client->ip,client->port);


    return 0;
}

int tcp_client_send(const SOCKET sSocket,const char *buf,int size)
{
    if(buf==NULL || size<1)
        return 0;

    if(!(sSocket>0)){
        return 0;
    }
//    log_debug("%d %s",size,buf);
    int n = send(sSocket,(char *)buf, size, 0);
    log_debug("tcp_client_send %d",n);

    return 0;
}

int tcp_client_recv(const SOCKET sSocket)
{
    if(!(sSocket>0)){
        return 0;
    }
//    log_debug("%d %s",size,buf);
    char buf[1024];

    int n = recv(sSocket,(char *)buf, sizeof(buf), 0);

    //根据buf 接收数据解析

    log_debug("tcp_client_send %d",n);

    return 0;
}

int start_tcp_client_th(client_info *client)
{
    return tcp_client_doth(client);
}

int stop_tcp_client_th(SOCKET *sSocket)
{
    CanOpenUI *mWin = controlMain->mWin;
    mWin->Append("stop",2);

    return 0;
}

