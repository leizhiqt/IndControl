#pragma execution_character_set("utf-8")

#include "WTcpServer.h"
#include "UWLog.h"
#include "ConvertUtil.h"

#include "WTcpClient.h"
#include "ModBusTcp.h"
#include "ControlMain.h"

#include <thread>
#include <unistd.h>

int serv_dowork(void* lpParameter);

int tcp_server_start(server_info_t *s_info)
{
    std::thread th(serv_dowork,s_info);
    usleep(300);
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

    for(size_t i = 0; i < cliens_p.size(); ++i)
    {
        tcp_client_send(cliens_p[i],buf,len);
    }
    return 0;
}

int ThreadProc(void* lpParameter)
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
        if(info->s_info->recvFun!=NULL){

            info->s_info->recvFun(recvBuf,count,info->acceptSocket);
        }

        usleep(300);
    }
    //结束连接
    closesocket(info->acceptSocket);
    return 0;
}

int serv_dowork(void* lpParameter)
{
    server_info_t *s_info=(server_info_t *)lpParameter;

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        log_debug("WSAStartup failed with error: %ld", iResult);
        return -1;
    }

    //log_debug("初始化socket：成功");

    s_info->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s_info->ListenSocket == INVALID_SOCKET) {
        log_debug("socket failed with error: %ld", WSAGetLastError());
        WSACleanup();
        return -2;
    }

    //log_debug("建立监听，等待连接: 成功");

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

    //log_debug("绑定套接字到一个IP地址和一个端口上:成功");

    //将套接字设置为监听模式等待连接请求
    if (listen(s_info->ListenSocket, 5) == SOCKET_ERROR) {
        log_debug("listen failed with error: %ld", WSAGetLastError());
        closesocket(s_info->ListenSocket);
        WSACleanup();
        return -4;
    }

    //log_debug("将套接字设置为监听模式等待连接请求:成功");

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
        std::thread th(ThreadProc,(void *)&info);
        usleep(500);
        th.detach();

        sleep(1);
    }

    closesocket(s_info->ListenSocket);
    WSACleanup();

    return -5;
}

/* 接收到位姿系统发来的数据 */
void recvXly(char *buf,int len,SOCKET recvSocket)
{
    if(buf==NULL || len<1)
            return;

    printf_hex((unsigned char*)buf,len);

    //信号槽机制 广播websocket
    emit controlMain->webSocket->broadcast_binary(QByteArray(buf,len));
}

/* 接收到modbus发来的数据 */
void recvModbusTcp(char *buf,int len,SOCKET recvSocket)
{
    if(buf==NULL || len<1)
        return;

    //如果功能码是06(修改)
    if(buf[7] == 0x06)
    {
        char *nth_byte = buf+8;
        ntoh_16(nth_byte);
        //得到数据修改地址
        uint16_t ret_inx= *((uint16_t *)(nth_byte));
        //替换相应数据
        slave_reg.r_slave[2*ret_inx]=buf[10];
        slave_reg.r_slave[2*ret_inx+1]=buf[11];

        //printf_hex((unsigned char *)buf,len);
        //log_debug("===================== %04x %d %02x %02x",ret_inx,ret_inx,slave_reg.r_slave[ret_inx],slave_reg.r_slave[ret_inx+1]);
        //printf_hex((unsigned char *)slave_reg.r_slave,10);

        //给操作台的响应桢(同请求帧相同)
        tcp_client_send(recvSocket,(char *)buf,len);

        //以下去INI文件中匹配操作台报文
        //得到对应的canOpen报文,然后通过tcpClient发给给can总线(22004端口)
        char hexs[512]={0};
        sprintf_hex(hexs,(unsigned char *)(buf+2),len-2);
        QString cmdstr = "ControlSet/";
        QString hexstr(hexs);
        hexstr.remove(QRegExp("\\s"));
        cmdstr.append(hexstr.toUpper());

        std::string stdcmd=cmdstr.toStdString();
        std::map<std::string,QByteArray>::iterator iter = Conf::getInstance()->conf_can_packs.find(stdcmd);
        if(iter != Conf::getInstance()->conf_can_packs.end())
        {
            //获得命令内容
            //log_debug("is find %s",cmdstr.toLatin1().data());
            QByteArray qbuf = iter->second;
            char *buf = qbuf.begin();
            //printf_hex((unsigned char *)buf,qbuf.length());
            tcp_client_send(controlMain->can_client.acceptSocket,buf,qbuf.length());
        }
        else{
            log_debug("not find %s",cmdstr.toLatin1().data());
        }
        return ;
    }

    if(buf[7] == 0x03){
        int in = 4;
        int rlen =(buf[11]*2 + in);//返回的数据个数
        int slen = rlen+5;
        memset(slave_reg.slave_buf,'\0',sizeof(slave_reg.slave_buf));
        memcpy(slave_reg.slave_buf,buf,5);//校验信息，MODBUS协议
        slave_reg.slave_buf[5]=rlen;//数据长度
        slave_reg.slave_buf[6]=buf[6];//功能码
        slave_reg.slave_buf[7]=buf[7];//ID
        slave_reg.slave_buf[8]=rlen-in;//数据个数
        if(buf[11]%2==0){
            memcpy(slave_reg.slave_buf+9,slave_reg.r_slave,slave_reg.slave_buf[8]);//偶数
        }else{
            memset(slave_reg.slave_buf+9,'\0',slave_reg.slave_buf[8]);
        }
        //这里是给操作台的响应桢
        tcp_client_send(recvSocket,(char *)slave_reg.slave_buf,slen);
        return ;
    }
}
