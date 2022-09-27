﻿#pragma execution_character_set("utf-8")

#define BUF_SIZE 128
#include <iostream>
#include <thread>
#include <unistd.h>

#include "UWLog.h"
#include "WTcpClient.h"
#include "CanOpenUI.h"
#include "ControlMain.h"
#include "ConvertUtil.h"
#include "UTypes.h"
#include "SelfMovingtail.h"
#include "ProtocolXly.h"

using namespace std;
//int nStart = 0;

//TCP客户端接收数据
int ThreadClient_recv(void* lpParameter)
{
    client_info *info = (client_info *)lpParameter;
    char recvBuf[1024] = {0};
    int count = 0;
    while(1){
        //向协同控制器请求数据
        if(info->sendFun!=NULL){
            info->sendFun(info->acceptSocket);
        }
        memset(recvBuf,'\0',sizeof(recvBuf));
        count = recv(info->acceptSocket, recvBuf, sizeof(recvBuf), 0);
        if (count == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break; //表示没有数据了
            }
            return 0; //遇见其他错误
        }
        if(count==0) {
            break;//被对方关闭
        }

        //处理接收数据 回调函数
        if(info->recvFun!=NULL){
            info->recvFun(recvBuf,count,info->acceptSocket);
        }
<<<<<<< Updated upstream
log_debug("Step2: ThreadClient_recv Complete");
        usleep(300);
=======
        Sleep(300);
>>>>>>> Stashed changes
    }
    //结束连接
    closesocket(info->acceptSocket);
    return 0;
}

//与服务器建立连接
int tcp_client_doth(client_info *client)
{
    log_debug("tcp_client_do_conn [%s] [%d] ....",client->ip,client->port);
    WSADATA wsd;
    SOCKADDR_IN servAddr;

    int ret=-1;

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
    ret = connect(client->acceptSocket, (LPSOCKADDR)&servAddr, nServAddrLen);
    if (SOCKET_ERROR == ret) {
        WSACleanup();//释放套接字资源
        return -3;
    }

    //启动线程
    std::thread th(ThreadClient_recv,client);
    usleep(1000*30);
    th.detach();
    log_debug("tcp_client_do_conn %s %d ok",client->ip,client->port);

    return 0;
}

//TCP客户端发送数据
int tcp_client_send(const SOCKET sSocket,const char *buf,int size)
{
    if(buf==NULL || size<1)
        return 0;
    if(!(sSocket>0)){
        return 0;
    }
    int n = send(sSocket,(char *)buf, size, 0);
    return n;
}

//TCP客户端接收数据
int tcp_client_recv(const SOCKET sSocket)
{
    if(!(sSocket>0)){
        return 0;
    }
    char buf[4096];
    int n = recv(sSocket,(char *)buf, sizeof(buf), 0);
    return n;
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

//接收到Modbus协同控制器报文
int modbus_recv(char *buf,int len,SOCKET recvSocket)
{
log_debug("接收到协同控制报文，数据长度: %d", len);

    printf_hex((uchar_8 *)buf,len);

    if(buf==NULL || len<1)
        return 0;
<<<<<<< Updated upstream
    if(controlMain->webSocket==NULL || !controlMain->webSocket->isRunning()) return -1;
=======

    QByteArray bytes = QByteArray(buf,len);

>>>>>>> Stashed changes
    //处理报文并推送给JAVA
    if(controlMain->webSocket != NULL){
        emit controlMain->webSocket->broadcast_binary_move(bytes);
    }
    return 0;
}

//给Modbus协同控制器发送报文请求数据
int modbus_send(SOCKET recvSocket)
{
log_debug("向协同控制器请求数据");

    static uint16_t selfMoveCount=0x0500;//这里只有modbus master用的计数器
    //获得命令内容
    std::map<std::string,QByteArray>::iterator iter = Conf::getInstance()->conf_can_packs.find("ControlSet/SelfMoveData");
    if(iter != Conf::getInstance()->conf_can_packs.end())
    {
        //ID号加1
        selfMoveCount ++;
        if(selfMoveCount == 0xFFFF){
            selfMoveCount = 0x0500;
        }

        QByteArray qbuf = iter->second;
        char *buf = qbuf.begin();

        char *send_p = (char *)malloc(qbuf.length() + 2);
        char *p = (char *)&selfMoveCount;
        *(send_p+0) = *(char*)(p+1);
        *(send_p+1) = *(char*)(p+0);

        memcpy(send_p+2,buf,qbuf.length());

        tcp_client_send(recvSocket,send_p,qbuf.length()+2);

        free(send_p);//堆上释放空间
    }
    return 0;
}

//接收到can总线转来的报文
int can_recv(char *buf,int len,SOCKET recvSocket)
{
log_debug("接收到掘进机工况数据，数据长度: %d", len);
    if(buf == NULL || len < 1)
        return 0;

    QByteArray bytes = QByteArray(buf,len);

    //处理报文并推送给JAVA
    if(controlMain->webSocket != NULL){
        emit controlMain->webSocket->broadcast_binary_can(bytes);
    }
    return 0;
}
