#pragma execution_character_set("utf-8")

#include "WTcpServer.h"
#include "UWLog.h"
#include "ConvertUtil.h"

#include "WTcpClient.h"
#include "ModBusTcp.h"
#include "ControlMain.h"

#include <thread>
#include <unistd.h>
#include <string.h>

int serv_dowork(void* lpParameter);

int tcp_server_start(server_info_t *s_info)
{
    std::thread th(serv_dowork,s_info);
    usleep(1000*30);
    th.detach();
    return 0;
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

    //取得ip和端口号
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
                log_debug("");
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
        usleep(1000*30);
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
    log_debug("位姿系统发来数据");
    if(buf==NULL || len<1)
            return;
    if(recvSocket == NULL){
        log_debug("recvSocket is null");
        return;
    }
    printf_hex((unsigned char*)buf,len);

    //信号槽机制 广播websocket
    emit controlMain->webSocket->broadcast_binary(QByteArray(buf,len));
}

QString toHexadecimal(const QByteArray &byteArray)
{
    QString str;
    for(int i = 0; i< byteArray.length(); i++){
        QString byteStr = QString::number(static_cast<uchar>(byteArray[i]), 16);
        if(byteStr.length() == 1) str += "0" + byteStr;
        else str += byteStr;
    }
    return str;
}

/* 接收到modbus发来的控制指令 */
void recvModbusTcp(char *buf,int len,SOCKET recvSocket)
{
    if(buf==NULL){
        log_debug("报文内容为空,不转发");
        return;
    }

    if(recvSocket == NULL){
        log_debug("与交换机的连接未完成,不转发");
        return;
    }

    //去除左边13个字节
    char hexs[512]={0};
    sprintf_hex(hexs,(unsigned char *)(buf+13),100);
    QString newHexstrT(hexs);
    //log_debug("控制台发来的报文:%s",newHexstrT.toLatin1().data());

    //用来保存值不为0的寄存器地址和值
    QStringList addList;
    QStringList valueList;

    //每次取2个字节，寄存器地址从40000开始
    int32_t storageAdd = 40000;
    newHexstrT.remove(QRegExp("\\s"));
    int iIndex = 0;
    for(int i = 0; i < 50; i++){
       QString tmpValue = newHexstrT.mid(iIndex,4);
       if(tmpValue.compare("0000")){
           addList.append(QString::number(storageAdd));
           valueList.append(tmpValue.toStdString().data());
       }
       storageAdd++;
       iIndex += 4;
    }

    //判断一下有不有不为0的寄存器地址
    if(addList.count() == 0){
        log_debug("从控制台发来的报文中没有检测到值不为0的情况");
        return;
    }

    //从配置文件中检测报文头
    QStringList canHeadList;
    QStringList canValueList;
    for(int i = 0 ; i < addList.count(); i++){
        QString strAdd = addList.at(i);//取得寄存器地址
        QString strValue = valueList.at(i);//取得值
        //log_debug("地址：%s，值：%s",strAdd.toStdString().data(),strValue.toStdString().data());
        std::string stdStrAdd = strAdd.toStdString();
        std::map<std::string,QByteArray>::iterator itfind = Conf::getInstance()->conf_storage_packs.find(stdStrAdd);
        //查找报文头
        if(itfind != Conf::getInstance()->conf_storage_packs.end()){
            QByteArray qbuf = itfind->second;
            QString name = toHexadecimal(qbuf);
            canValueList.append(strValue);
            canHeadList.append(name);
            //log_debug("报文头：%s, 值：%s",name.toStdString().data(),strValue.toStdString().data());
        }
    }

    if(canHeadList.count() == 0){
        log_debug("从控制台发来的报文中没有检测到报文头");
        return;
    }

    //检查有不有重复的报文头，如果有则将其值转为二进制相加后转为十六制
    QStringList newCanHead;
    QStringList newCanValue;
    //如果只有一条记录
    if(canHeadList.count() == 1){
        newCanHead.append(canHeadList.at(0));
        newCanValue.append(canValueList.at(0));
    }else{
        for(int i = 0; i < canHeadList.count(); i++){
            for(int k = i + 1; k < canHeadList.count(); k++){
                if(canHeadList.at(i).compare(canHeadList.at(k))){
                    //这里说明出现了相同的报文头
                    QString firstValue = canValueList.at(i);
                    QString secondValue = canValueList.at(k);
                    bool ok;
                    //把这两个相同报文头对应的值取到转成10进制了
                    unsigned int intFirst = firstValue.toInt(&ok,10);
                    unsigned int intSecond = secondValue.toInt(&ok,10);
                    //这两个值相加然后要转成16进制
                    unsigned int nResult = intFirst + intSecond;
                    newCanHead.append(canHeadList.at(i));
                    newCanValue.append(QString::number(nResult, 16));
                }
            }
        }
    }

    //分别取出报文头和值与配置文件进行校验
    if(newCanHead.count() == 0){
        //log_debug("去除重复后，从控制台发来的报文中没有检测到报文头");
        return;
    }
    QByteArray sendData;
    for(int i = 0; i < newCanHead.count(); i++){
        QString tmpCanHead = newCanHead.at(i);
        QString tmpCanValue = newCanValue.at(i);
        std::map<std::string,QByteArray> ab = Conf::getInstance()->conf_can_packs;
        std::map<std::string,QByteArray>::iterator iter;
        for(iter=ab.begin();iter!=ab.end();++iter){
            QByteArray qbuf = iter->second;
            QString rptText = toHexadecimal(qbuf);
            //比较报文头
            if(rptText.contains(tmpCanHead)){
                //报文头对上了，接着比较值对不对
                int iIndex = 10;
                for(int i = 0; i < 4; i++){
                   QString tmpValue = rptText.mid(iIndex,4);
                   if(tmpValue.compare(tmpCanValue) == 0){
                       sendData.append(qbuf);
                       //log_debug("匹配上报文：%s",rptText.toStdString().data());
                   }
                   iIndex += 4;
                }
            }
        }
    }

    //如果匹配结果不为空
    if(sendData.count() > 0){
        char *buf = sendData.begin();
        tcp_client_send(controlMain->can_client.acceptSocket,buf,sendData.length());
        //log_debug("推送报文：%s",toHexadecimal(sendData).toStdString().data());
    }
}
