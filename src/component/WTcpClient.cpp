#pragma execution_character_set("utf-8")

#define BUF_SIZE 128
#include <iostream>
#include <thread>

#include "UWLog.h"
#include "WTcpClient.h"
#include "CanOpenUI.h"
#include "ControlMain.h"
#include "PGSQLDriveHelper.h"
#include "ConvertUtil.h"

using namespace std;

//TCP客户端接收数据
DWORD WINAPI ThreadClient_recv(__in  LPVOID lpParameter)
{
    client_info *info = (client_info *)lpParameter;

    // 取得ip和端口号
    sprintf(info->ip, inet_ntoa(info->addr.sin_addr));
    info->port = ntohs(info->addr.sin_port);
    log_debug("recv:ip=%s port=%d ",info->ip,info->port);

    char recvBuf[1024] = {0};
    int count = 0;
    while(1){
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

        if(count==0) break;//被对方关闭

        //处理接收数据 回调函数
        if(info->recvFun!=NULL)
            info->recvFun(recvBuf,count,info->acceptSocket);

        Sleep(300);
    }
    //结束连接
    closesocket(info->acceptSocket);
    return 0;
}

//与服务器建立连接
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

    //启动线程
    DWORD dwThread;
    HANDLE hThread = CreateThread(NULL,0,ThreadClient_recv,(LPVOID)client,0,&dwThread);
    if(hThread==NULL)
    {
        closesocket(client->acceptSocket);
        log_debug("Thread Creat Failed!");
    }

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
//  log_debug("%d %s",size,buf);
    int n = send(sSocket,(char *)buf, size, 0);
    log_debug("tcp_client_send %d",n);
    return n;
}

//TCP客户端接收数据
int tcp_client_recv(const SOCKET sSocket)
{
    //这里解析报文要注意：tcp客户端有两种：can总线的客户端,modbus slave的客户端
    if(!(sSocket>0)){
        return 0;
    }
//  log_debug("%d %s",size,buf);

    char buf[1024];

    int n = recv(sSocket,(char *)buf, sizeof(buf), 0);

    //根据buf接收数据解析

    log_debug("tcp_client_send %d",n);

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

int modbus_recv(char *buf,int len,SOCKET recvSocket)
{
    printf_hex((unsigned char*)buf,len);
    log_debug("buf len=%d",len);
    return 0;
}

int can_recv(char *buf,int len,SOCKET recvSocket)
{
    printf_hex((unsigned char*)buf,len);
    log_debug("buf len=%d",len);
    return 0;
}
