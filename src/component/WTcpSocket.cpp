#define BUF_SIZE 128
#include <iostream>

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>

#include "UWLog.h"
#include "WTcpSocket.h"

using namespace std;
#pragma comment(lib, "WS2_32.lib")

SOCKET sSocket;
std::thread thd_handle_client;//主线程

//192.168.2.187
int tcp_client_doth(char* host,int port)
{
    log_debug("======================");
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
    sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == sSocket) {
        printf("socker failed\n");
        WSACleanup();
        return -1;
    }

    //设置服务器地址
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(host);
    servAddr.sin_port = htons(port);
    int nServAddrLen = sizeof(servAddr);
    //连接服务器
    //log_debug("2====================== %s %d\n",host,port);
    for(int i=0;i<5;i++){
        n = connect(sSocket, (LPSOCKADDR)&servAddr, nServAddrLen);
        if (SOCKET_ERROR == n) {
            log_debug("connect failed %d\n",SOCKET_ERROR);
           Sleep(500);
            continue;
        }
        break;
    }

    if (SOCKET_ERROR == n) {
        log_debug("connect failed\n");
//            closesocket(sSocket);
        WSACleanup();//释放套接字资源
        return -1;
    }

    log_debug("3======================");
    //接收数据
    char buf[BUF_SIZE];
//    ZeroMemory(buf, BUF_SIZE);
    while (1) {
        n = recv(sSocket, buf, BUF_SIZE, 0);
        log_debug("recv:%d %s",n,buf);

        if (SOCKET_ERROR == n) {
            printf("send failed");
        }

        Sleep(1000*3);
    }


    return 0;
}

int tcp_client_send(unsigned char const *buf,size_t size)
{
    //发送数据
//    char buf[BUF_SIZE];
//    ZeroMemory(buf, BUF_SIZE);
//    strcpy(buf, "hello server");
    printf_hex(buf,size);
    int retVal = send(sSocket,(char *)buf, size, 0);
    printf("send ok=%d!\n",retVal);
    return 0;
}


int start_tcp_client_th(char * host,int port)
{
    if (thd_handle_client.joinable())
        return 0;

    thd_handle_client = std::thread(tcp_client_doth,host,port);

//    if (thd_handle_receive.joinable())
//        thd_handle_receive.join();
    return 0;
}
