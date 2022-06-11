#define BUF_SIZE 128
#include <iostream>

#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>

#include "UWLog.h"
#include "WTcpClient.h"
#include "CanOpenUI.h"
#include "ControlMain.h"
#include "PGSQLDriveHelper.h"

using namespace std;

//int c_forever=0;

SOCKET sSocket=-1;
char to_host[128];
int to_port;
//std::thread thd_handle_client;//主线程

//192.168.2.187
int tcp_client_doth(char* host,int port)
{
    log_debug("tcp_client_doth ======================");
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
        return -2;
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
        return -3;
    }

//    char buf[10];

//    int ret = send(sSocket,(char *)buf, sizeof(buf), 0);
//    log_debug("tcp_client_send %d",ret);

//    log_debug("tcp_client_doth recv:%d",c_forever);
    //接收数据
//    char buf[BUF_SIZE];
//    ZeroMemory(buf, BUF_SIZE);
//    while (c_forever) {
//        n = recv(sSocket, buf, BUF_SIZE, 0);
//        log_debug("recv:%d %s",n,buf);

//        if (SOCKET_ERROR == n) {
//            printf("send failed");
//        }
//        log_debug("c_forever client");
//        Sleep(200);
//    }

    strcpy(to_host,host);
    to_port = port;
    return 0;
}

int tcp_client_send(unsigned char const *buf,size_t size)
{
    log_debug("tcp_client_send sSocket:%d",sSocket);

    if(!(sSocket>0)){
        return 0;
    }

    int ret;
    //发送数据
    unsigned char hexs[1024];
    printf_hex(hexs,buf,size);
//log_debug("tcp_client_send sSocket:%d c_forever:%d",sSocket,c_forever);
    ret = send(sSocket,(char *)buf, size, 0);
    log_debug("tcp_client_send %d",ret);

    CanOpenUI *mWin = controlMain->mWin;
    if(ret>0){
        mWin->Append((char *)hexs,2);
    }else{
        mWin->Append("下发失败",2);
    }

    PGSQLDriveHelper::getInstance()->pg_add_exec("prog_postures_send",to_host,to_port,(char *)hexs);
//    log_debug("tcp_client_send");
    return 0;
}


int start_tcp_client_th(char * host,int port)
{
//    if (thd_handle_client.joinable())
//        return 0;

//    c_forever=1;
//    thd_handle_client = std::thread(tcp_client_doth,host,port);

//    Sleep(500);

//    if (!thd_handle_client.joinable()){
//        return -1;
//    }
    tcp_client_doth(host,port);
    return 0;
}

int stop_tcp_client_th()
{
//    c_forever=0;

//    if (thd_handle_client.joinable())
//        thd_handle_client.join();

    CanOpenUI *mWin = controlMain->mWin;
    mWin->Append("stop",2);

    return 0;
}
