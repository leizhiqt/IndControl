#pragma execution_character_set("utf-8")

#include "WTcpServer.h"
#include "UWLog.h"

#include "WTcpClient.h"
#include "ControlMain.h"
#include "PGSQLDriveHelper.h"
#include "ConvertUtil.h"

int serv_dowork(server_info_t *s_info);

int tcp_server_start(server_info_t *s_info)
{
    std::thread th(serv_dowork,s_info);
    Sleep(300);
    while(th.joinable())
    th.detach();
    return -2;
}

void tcp_server_stop(server_info_t* s_info){
    s_info->s_forever=0;

    CanOpenUI *ui=controlMain->mWin;
    ui->Append("stop",1);
}

int tcp_server_broadcast(server_info_t *s_info,char *buf,int len){
    log_debug(__FUNCTION__);
    for(size_t i = 0; i < controlMain->xly_cliens.size(); ++i)
    {
        tcp_client_send((controlMain->xly_cliens[i]),buf,len);
    }
    return 0;
}

DWORD WINAPI ThreadProc(__in  LPVOID lpParameter)
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
        if(info->s_info->recvFun!=NULL)
            info->s_info->recvFun(recvBuf,count);

        Sleep(300);
    }
    //结束连接
    closesocket(info->acceptSocket);
    return 0;
}

int serv_dowork(server_info_t *s_info)
{
    //----------------------
    // Initialize Winsock.
    log_debug("Initialize Winsock %d",s_info->port);

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        log_debug("WSAStartup failed with error: %ld", iResult);
        return -1;
    }
    log_debug("初始化socket：成功");
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
//    SOCKET ListenSocket;
    s_info->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s_info->ListenSocket == INVALID_SOCKET) {
        log_debug("socket failed with error: %ld", WSAGetLastError());
        WSACleanup();
        return -2;
    }
    log_debug("建立监听，等待连接: 成功");
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = htonl(INADDR_ANY); //实际上是0
    addrServer.sin_port = htons(s_info->port);

    //绑定套接字到一个IP地址和一个端口上
    if (::bind(s_info->ListenSocket,(sockaddr *) &addrServer,sizeof(addrServer)) == SOCKET_ERROR)
    {
        log_debug("bind failed with error: %ld", WSAGetLastError());
        closesocket(s_info->ListenSocket);
        WSACleanup();
        return -3;
    }
    log_debug("绑定套接字到一个IP地址和一个端口上:成功");

    //将套接字设置为监听模式等待连接请求
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(s_info->ListenSocket, 5) == SOCKET_ERROR) {
        log_debug("listen failed with error: %ld", WSAGetLastError());
        closesocket(s_info->ListenSocket);
        WSACleanup();
        return -4;
    }
    log_debug("将套接字设置为监听模式等待连接请求:成功");

    client_info info= {0};
    info.acceptSocket=0;
    info.addr={0};
    info.len=sizeof(SOCKADDR);
    info.s_info = s_info;

    //以一个无限循环的方式，不停地接收客户端socket连接
    while(s_info->s_forever)
    {

        //请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字
        info.acceptSocket=accept(s_info->ListenSocket,(SOCKADDR*)&(info.addr),&info.len);
        if(info.acceptSocket  == INVALID_SOCKET)
        {
            s_info->s_forever = 0;
            log_debug("acceptSocket %d",info.acceptSocket);
            break; //出错
        }

        controlMain->xly_cliens.push_back(info.acceptSocket);

        //启动线程
        DWORD dwThread;
        HANDLE hThread = CreateThread(NULL,0,ThreadProc,(LPVOID)&info,0,&dwThread);
        if(hThread==NULL)
        {
            s_info->s_forever = 0;
            closesocket(info.acceptSocket);
            log_debug("Thread Creat Failed!");
            break;
        }
        CloseHandle(hThread);
        Sleep(100);
    }

    closesocket(s_info->ListenSocket);
    WSACleanup();

    return -5;
}

void recvXly(char *buf,int len)
{
    if(buf==NULL || len<1)
            return;

    emit controlMain->webSocket->broadcast_msg((char *)buf);
    //发送交换机
    tcp_client_send((controlMain->canOpenSocket),(char *)buf,len);

    //CanOpenUI *ui=controlMain->mWin;
    //ui->Append((char *)hexs,1);
}

void recvModbusTcp(char *buf,int len)
{
    if(buf==NULL || len<1)
        return;

    //发送交换机
    tcp_client_send((controlMain->canOpenSocket),buf,len);
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

       if((*(frame)<9 && *(frame)>0))
       {
            return;
       }

        if(*(frame+1)==0x01 && *(frame+2)==0x8a){
//                if(*(frame+7)==0x01)
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x01 && *(frame+2)==0x8b){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x01 && *(frame+2)==0x8c){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && *(frame+2)==0x01){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && *(frame+2)==0x02){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && (*(frame+2)==0x21 || *(frame+2)==0x22 || *(frame+2)==0x23
                                 || *(frame+2)==0x24 || *(frame+2)==0x25 || *(frame+2)==0x26
                                 || *(frame+2)==0x27)){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x03 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x04 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x05 && *(frame+2)==0x10){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x11){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x03 && *(frame+2)==0x11){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x87){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x03 && *(frame+2)==0x87){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x04 && *(frame+2)==0x87){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x04 && *(frame+2)==0x0e){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }

        if(*(frame+1)==0x02 && *(frame+2)==0x90){
//            printf_hex(frame,len);
            //tcp_client_send(frame,len);
            return;
        }
}
