#define BUF_SIZE 128
#include <iostream>

#include<WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#include "UWLog.h"

using namespace std;
#pragma comment(lib, "WS2_32.lib")

int tcp_send();
int tcp(char * host,int port);


SOCKET sSocket;


std::thread thd_handle_can;//主线程

int start_tcp(char * host,int port)
{
    thd_handle_can = std::thread(tcp,host,port);

//    if (thd_handle_receive.joinable())
//        thd_handle_receive.join();
return 0;
}

//192.168.2.187
int tcp(char* host,int port)
{
    log_debug("======================\n");

    WSADATA wsd;
    SOCKADDR_IN servAddr;

    int retVal;
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
    log_debug("1======================\n");
    //设置服务器地址
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("192.168.2.152");
    servAddr.sin_port = htons(port);
    int nServAddrLen = sizeof(servAddr);
    log_debug("2======================\n");
    //连接服务器
    //log_debug("2====================== %s %d\n",host,port);


    for(int i=0;i<5;i++){
        retVal = connect(sSocket, (LPSOCKADDR)&servAddr, nServAddrLen);
        if (SOCKET_ERROR == retVal) {
            log_debug("connect failed %d\n",SOCKET_ERROR);
           Sleep(500);
            continue;
        }
        break;
    }

    if (SOCKET_ERROR == retVal) {
        log_debug("connect failed\n");
//            closesocket(sSocket);
        WSACleanup();//释放套接字资源
        return -1;
    }

    log_debug("3======================\n");
    //发送数据
    char buf[BUF_SIZE];
    ZeroMemory(buf, BUF_SIZE);
    strcpy(buf, "hello server");
    log_debug("4======================\n");
    while (1) {
        retVal = send(sSocket, buf, BUF_SIZE, 0);
        log_debug("send ======================\n");
        if (SOCKET_ERROR == retVal) {
            printf("send failed");
        }

        Sleep(1000*3);
    }


    return 0;
}



int tcp_send()
{
    //发送数据
    char buf[BUF_SIZE];
    ZeroMemory(buf, BUF_SIZE);
    strcpy(buf, "hello server");
    int retVal = send(sSocket, buf, BUF_SIZE, 0);
    printf("send ok=%d!\n",retVal);
    return 0;
}
