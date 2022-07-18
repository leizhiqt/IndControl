#pragma execution_character_set("utf-8")

#include "WTcpServer.h"
#include "UWLog.h"

#include "WTcpClient.h"
#include "ControlMain.h"
#include "PGSQLDriveHelper.h"
#include "ConvertUtil.h"
#include<vector>

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

   std::vector<SOCKET> cliens_p = *(s_info->cliens_p);
//   if(cliens_p==NULL) return 1;

    for(size_t i = 0; i < cliens_p.size(); ++i)
    {
        tcp_client_send(cliens_p[i],buf,len);
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
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        log_debug("WSAStartup failed with error: %ld", iResult);
        return -1;
    }
    log_debug("初始化socket：成功");

    s_info->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s_info->ListenSocket == INVALID_SOCKET) {
        log_debug("socket failed with error: %ld", WSAGetLastError());
        WSACleanup();
        return -2;
    }
    log_debug("建立监听，等待连接: 成功");

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

        s_info->cliens_p->push_back(info.acceptSocket);

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

//位姿系统接收
void recvXly(char *buf,int len)
{
    if(buf==NULL || len<1)
            return;

    printf_hex((unsigned char*)buf,len);
    log_debug("buf len=%d",len);

    //信号槽机制 广播websocket
    emit controlMain->webSocket->broadcast_binary(QByteArray(buf,len));

    //CanOpenUI *ui=controlMain->mWin;
    //ui->Append((char *)hexs,1);
}

//modbus接收
void recvModbusTcp(char *buf,int len)
{
    if(buf==NULL || len<1)
        return;

  //这里才是MODBUS 接收
//但是这里你不能直接转发出去撒，需要转成CAN的报文先
    //发送交换机
    //tcp_client_send((controlMain->canOpenSocket),buf,len);
}
