#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include<thread>

#include "UWLog.h"
#include "WTcpServer.h"
#include "WTcpClient.h"
#include "ControlMain.h"
#include "PGSQLDriveHelper.h"

// Need to link with Ws2_32.lib
#pragma execution_character_set("utf-8")

int serv_dowork(int port);

int s_forever;

SOCKET ListenSocket;
std::thread thread_handle_tcpserver;//主线程

typedef struct _client_info
{
  SOCKET acceptSocket;
  SOCKADDR_IN  addr;
  char ip[128];
  unsigned int port;
  int len;
} client_info;


void recv_xly(unsigned char *frame,size_t len);

int tcp_server_start(int port){

    if (thread_handle_tcpserver.joinable())
        return 0;

    s_forever=1;
    thread_handle_tcpserver = std::thread(serv_dowork,port);

    Sleep(300);

    if (thread_handle_tcpserver.joinable()){
        return 0;
    }

    return -2;
}

void tcp_server_stop(){
    s_forever=0;
    if (thread_handle_tcpserver.joinable())
        thread_handle_tcpserver.join();

    CanOpenUI *ui=controlMain->mWin;
    ui->Append("stop",1);
}

DWORD WINAPI ThreadProc(
    __in  LPVOID lpParameter
    )
{
    client_info info = *(client_info *)lpParameter;
//    SOCKET AcceptSocket=(SOCKET) lpParameter;

    // 取得ip和端口号
   sprintf(info.ip, inet_ntoa(info.addr.sin_addr));
   info.port = ntohs(info.addr.sin_port);
//   info.sock = new_fd;
    log_debug("recv:ip=%s port=%d ",info.ip,info.port);
    //接收缓冲区的大小是50个字符
    char recvBuf[51];
    unsigned char hexs[256];
    while(s_forever){
        int count =recv(info.acceptSocket ,recvBuf,50,0);
        if(count==0)break;//被对方关闭
//        if(count==SOCKET_ERROR)break;//错误count<0
//        int sendCount,currentPosition=0;
//        while( count>0 && (sendCount=send(AcceptSocket ,recvBuf+currentPosition,count,0))!=SOCKET_ERROR)
//        {
//            count-=sendCount;
//            currentPosition+=sendCount;
//        }
//        if(sendCount==SOCKET_ERROR)break;
//        ui->ModBusView->append(hexs);
        recv_xly((unsigned char*)recvBuf,count);

        printf_hex(hexs,(unsigned char*)recvBuf,count);

        //接收到的数据
//        PGSQLDriveHelper::getInstance()->pg_add_exec((char *)hexs);
        PGSQLDriveHelper::getInstance()->pg_add_exec("prog_postures_recv",info.ip,info.port,(char *)hexs);

        CanOpenUI *ui=controlMain->mWin;
        ui->Append((char *)hexs,1);

        log_debug("recv:%d %s %d ",info.acceptSocket,hexs,strlen((char *)hexs));
    }
    //结束连接
    closesocket(info.acceptSocket);
    return 0;
}

int serv_dowork(int port)
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        log_debug("WSAStartup failed with error: %ld", iResult);
        return -1;
    }
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
//    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        log_debug("socket failed with error: %ld", WSAGetLastError());
        WSACleanup();
        return -2;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = htonl(INADDR_ANY); //实际上是0
    addrServer.sin_port = htons(port);


    //绑定套接字到一个IP地址和一个端口上
    if (bind(ListenSocket,(SOCKADDR *) & addrServer, sizeof (addrServer)) == SOCKET_ERROR) {
        log_debug("bind failed with error: %ld", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return -3;
    }

    //将套接字设置为监听模式等待连接请求
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(ListenSocket, 5) == SOCKET_ERROR) {
        log_debug("listen failed with error: %ld", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return -4;
    }

    client_info info;
    info.acceptSocket=0;
    info.addr={0};
    info.len=sizeof(SOCKADDR);

//    SOCKADDR_IN addrClient;
//    int len=sizeof(SOCKADDR);
    //以一个无限循环的方式，不停地接收客户端socket连接
    while(s_forever)
    {
        //请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字
        info.acceptSocket=accept(ListenSocket,(SOCKADDR*)&(info.addr),&info.len);
        if(info.acceptSocket  == INVALID_SOCKET)
        {
            s_forever = 0;
            log_debug("acceptSocket %d",info.acceptSocket);
            break; //出错
        }
        //启动线程
        DWORD dwThread;
        HANDLE hThread = CreateThread(NULL,0,ThreadProc,(LPVOID)&info,0,&dwThread);
        if(hThread==NULL)
        {
            s_forever = 0;
            closesocket(info.acceptSocket);
            log_debug("Thread Creat Failed!");
            break;
        }
        CloseHandle(hThread);
    }

    closesocket(ListenSocket);
    WSACleanup();

    return -5;
}


void recv_xly(unsigned char *frame,size_t len)
{
//    unsigned char frame[13];
//    memset(frame,'\0',sizeof(frame));
//    frame[0]=0x06;
//    frame[1]=0x00;
//    frame[2]=0x00;
//    frame[3]=0x01;
//    frame[4]=0x8a;

//    frame[5]=0x00;//b0
//    frame[6]=0x00;//b1
//    frame[7]=0x0;//b2
//    frame[8]=0x01;//b3
//    frame[9]=0x00;//b4
//    frame[10]=0x00;//b5
//    frame[11]=0x00;//b6
//    frame[12]=0x00;//b7

       if((*frame<9 && *frame>0))
       {
            return;
       }

        if(*(frame+1)==0x01 && *(frame+2)==0x8a){
//                if(*(frame+7)==0x01)
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x01 && *(frame+2)==0x8b){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x01 && *(frame+2)==0x8c){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && *(frame+2)==0x01){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && *(frame+2)==0x02){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && (*(frame+2)==0x21 || *(frame+2)==0x22 || *(frame+2)==0x23
                                 || *(frame+2)==0x24 || *(frame+2)==0x25 || *(frame+2)==0x26
                                 || *(frame+2)==0x27)){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x03 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x04 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x11){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x03 && *(frame+2)==0x11){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x87){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x03 && *(frame+2)==0x87){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x04 && *(frame+2)==0x87){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x04 && *(frame+2)==0x0e){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x90){
//            printf_hex(frame,len);
            tcp_client_send(frame,len);
            return;
        }
}


