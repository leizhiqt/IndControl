#pragma execution_character_set("utf-8")

#include "WebSocket.h"
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>
#include <QSharedMemory>
#include <QBuffer>
#include "WTcpClient.h"
#include "UWLog.h"
#include "PGSQLDriveHelper.h"
#include "ControlMain.h"
#include "ProtocolXly.h"
#include "ConvertUtil.h"

extern SOCKET jhjSocket;

WebSocket::WebSocket(quint16 port)
{
    this->port=port;
    init();

    this->start();

    log_debug("开始监听:%d",port);
}

/* 初始化WebSocket服务 */
void WebSocket::init()
{
    m_websocketserver = new QWebSocketServer("server", QWebSocketServer::NonSecureMode);
    connect(m_websocketserver, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    m_websocketserver->listen(QHostAddress::Any,port);

    connect(this, SIGNAL(broadcast_binary(QByteArray)), this, SLOT(slot_broadcast_binary(QByteArray)));
    connect(this, SIGNAL(broadcast_msg(QString)), this, SLOT(slot_broadcast_msg(QString)));
}

WebSocket::~WebSocket()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    stop();
}

void WebSocket::run()
{
    while(1){
        sleep(180);
    }
}

/* 停止服务 */
void WebSocket::stop()
{
    //移除所有客户端连接
    for (int i = 0; i < m_clients.size(); ++i)
    {
        QWebSocket *csocket = (QWebSocket *)m_clients.at(i);
        if(csocket->state()==QAbstractSocket::SocketState::ConnectedState)
            csocket->close();
        m_clients.removeAt(i);
    }

    m_websocketserver->close();
    m_websocketserver=NULL;
}

/* 新连接接入 */
void WebSocket::onNewConnection()
{
    client_socket = m_websocketserver->nextPendingConnection();

    connect(client_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(recvTextMessage(QString)));

    connect(client_socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(recvBinaryMessage(QByteArray)),Qt::UniqueConnection);

    connect(client_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    m_clients.push_back(client_socket);
}

/* 收到消息 */
void WebSocket::recvBinaryMessage(const QByteArray &message)
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    const char *recvBuf = message.begin();
    tcp_client_send((controlMain->canOpenSocket),recvBuf,strlen(recvBuf));
    tcp_server_broadcast(controlMain->xly_srv,(char *)recvBuf,strlen(recvBuf));
}

/* 这是websocket 收到消息 */
void WebSocket::recvTextMessage(const QString &content)
{
    //JAVA发来的消息
    QString message = QString(content);
    message.remove(QRegExp("\\s"));

    QByteArray msgBytes = message.toLatin1().constData();
    log_debug("%s",msgBytes.begin());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(msgBytes);
    if(jsonDocument.isNull() || (!jsonDocument.isObject() && jsonDocument.isArray())){
        log_debug("Websocket解析结果：接收到的消息不正确");
        emit broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"消息格式不正确\"}");
        return;
    }

    //解析消息
    QJsonObject jsonObject = jsonDocument.object();
    QJsonValue cmdName = jsonObject.value("method");
    if(cmdName == NULL || cmdName.isNull() || cmdName.isUndefined()){
        log_debug("Websocket解析结果：命令不正确");
        emit broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"命令格式不正确\"}");
        return;
    }

    //如果消息内容是参数配置
    if(cmdName.toString() == "paramset"){
        //{"version":"1.0","method":"paramset","params":{"字段1":"值1","字段2":"值2",...,"字段N":"值N"}}
        QJsonValue paramObject = jsonObject.value("params");
        if(!paramObject.isObject()){
            log_debug("Websocket解析结果：参数格式不正确");
            emit broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数格式不正确\"}");
            return;
        }
        //取得各参数值
        QJsonObject valueContent = paramObject.toObject();
        if(valueContent.count() != 26){
            log_debug("Websocket解析结果：参数个数不正确，应该有26个参数");
            emit broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数个数不正确，应该有26个参数\"}");
            return;
        }

        response_xly_t response_xly={0};
        response_xly.r1=(uchar_8)0xAA;
        response_xly.r2=(uchar_8)0xBB;

        //初始经度
        *((float *)&(response_xly.r3)) = valueContent.value("startxvalue").toString().toFloat();

        //初始纬度
        *((float *)&(response_xly.r4))  = valueContent.value("startyvalue").toString().toFloat();

        //初始高度
        *((float *)&(response_xly.r5))  = valueContent.value("startzvalue").toString().toFloat();

        //惯导标定
        *((int *)&(response_xly.r6))  = valueContent.value("stationstatus").toString().toInt();

        //前置视觉标定
        *((int *)&(response_xly.r7)) = valueContent.value("frontview").toString().toInt();

        //后置视觉标定
        *((int *)&(response_xly.r8)) = valueContent.value("rearview").toString().toInt();

        //激光A点X坐标
        *((float *)&(response_xly.r9)) = valueContent.value("laseraxvalue").toString().toFloat();

        //激光A点Y坐标
        *((float *)&(response_xly.r10)) = valueContent.value("laserayvalue").toString().toFloat();

        //激光A点Z坐标
        *((float *)&(response_xly.r11)) = valueContent.value("laserazvalue").toString().toFloat();

        //激光B点X坐标
        *((float *)&(response_xly.r12)) = valueContent.value("laserbxvalue").toString().toFloat();

        //激光B点Y坐标
        *((float *)&(response_xly.r13)) = valueContent.value("laserbyvalue").toString().toFloat();

        //激光B点Z坐标
        *((float *)&(response_xly.r14)) = valueContent.value("laserbzvalue").toString().toFloat();

        //激光C点X坐标
        *((float *)&(response_xly.r15)) = valueContent.value("lasercxvalue").toString().toFloat();

        //激光C点Y坐标
        *((float *)&(response_xly.r16)) = valueContent.value("lasercyvalue").toString().toFloat();

        //激光C点Z坐标
        *((float *)&(response_xly.r17)) = valueContent.value("laserczvalue").toString().toFloat();

        //激光A1点X坐标
        *((float *)&(response_xly.r18)) = valueContent.value("lasera1xvalue").toString().toFloat();

        //激光A1点Y坐标
        *((float *)&(response_xly.r19)) = valueContent.value("lasera1yvalue").toString().toFloat();

        //激光A1点Z坐标
        *((float *)&(response_xly.r20)) = valueContent.value("lasera1zvalue").toString().toFloat();

        //激光B1点X坐标
        *((float *)&(response_xly.r21)) = valueContent.value("laserb1xvalue").toString().toFloat();

        //激光B1点Y坐标
        *((float *)&(response_xly.r22)) = valueContent.value("laserb1yvalue").toString().toFloat();

        //激光B1点Z坐标
        *((float *)&(response_xly.r23)) = valueContent.value("laserb1zvalue").toString().toFloat();

        //激光C1点X坐标
        *((float *)&(response_xly.r24)) = valueContent.value("laserc1xvalue").toString().toFloat();

        //激光C1点Y坐标
        *((float *)&(response_xly.r25)) = valueContent.value("laserc1yvalue").toString().toFloat();

        //激光C1点Z坐标
        *((float *)&(response_xly.r26)) = valueContent.value("laserc1zvalue").toString().toFloat();

        //截割深度
        *((float *)&(response_xly.r27)) = valueContent.value("cutdepth").toString().toFloat();

        //掘进速度
        *((float *)&(response_xly.r28)) = valueContent.value("diginspeed").toString().toFloat();
        response_xly.r29 = 0xBF;
        printf_hex((unsigned char *)&response_xly,sizeof(response_xly_t));

        //将报文下发给位姿系统客户端
        tcp_server_broadcast(controlMain->xly_srv,(char *)&response_xly,sizeof(response_xly_t));

        //给WEBSOCKET客户端推送消息
        emit broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数设置发送完毕\"}");
        return;
    }

     if(cmdName.toString() == "devicecontrol"){
         log_debug("devicecontrol");
//         response_opencan13_t contr_can={0};
        //{"version":"1.0","method":"devicecontrol","params":"字段1"}}
        //解析得到命令内容封装协议并转发给交换机
        QJsonValue cmdText = jsonObject.value("params");
        QString cmdstr = cmdText.toString();
        //紧急停止
        if(cmdText.toString()=="Control001" || cmdText.toString()=="Control002"
                ||cmdText.toString()=="Control003"||cmdText.toString()=="Control004"
                ||cmdText.toString()=="Control005"||cmdText.toString()=="Control006"){

            cmdstr = "ControlSet/"+cmdstr;

            std::string stdcmd=cmdstr.toStdString();
            log_debug("cmdText %s",stdcmd.c_str());

            std::map<std::string,QByteArray>::iterator iter = Conf::getInstance()->conf_can_packs.find(stdcmd);
            if(iter != Conf::getInstance()->conf_can_packs.end())
            {
                log_debug("is find %s",cmdstr.toLatin1().data());
                QByteArray qbuf = iter->second;
                char *buf = qbuf.begin();
                printf_hex((unsigned char *)buf,qbuf.length());
                tcp_client_send(controlMain->canOpenSocket,buf,qbuf.length());
            }
            else{
                log_debug("not find %s",cmdstr.toLatin1().data());
            }

            return;
        }
/*
        //一键启动
        if(cmdText.toString()=="control002"){
            //根据收到的命令封装can桢
            contr_can.r1=0x08;
            contr_can.r4=0x01;
            contr_can.r5=0x9d;
            contr_can.r7=0x30;//只修改这个值就可以了
//            tcp_client_send(canOpenSocket,(char *)&contr_can,sizeof(response_opencan11_t));
            return;
        }
        //启动智能通风
         if(cmdText.toString()=="control003"){
            //根据收到的命令封装can桢
            contr_can.r1=0x08;
            contr_can.r4=0x01;
            contr_can.r5=0x9d;
            contr_can.r7=0x10;//只修改这个值就可以了
//            tcp_client_send(canOpenSocket,(char *)&contr_can,sizeof(response_opencan11_t));

        }
        //停止智能通风
        if(cmdText.toString()=="control004"){
            //根据收到的命令封装can桢
            contr_can.r1=0x08;
            contr_can.r4=0x01;
            contr_can.r5=0x9d;
            contr_can.r7=0x10;//只修改这个值就可以了
        }
        //启动智能运输
        if(cmdText.toString()=="control005"){
            //根据收到的命令封装can桢
            contr_can.r1=0x08;
            contr_can.r4=0x01;
            contr_can.r5=0x9d;
            contr_can.r7=0x10;//只修改这个值就可以了
        }
        //停止智能运输
        if(cmdText.toString()=="control006"){
            //根据收到的命令封装can桢
            contr_can.r1=0x08;
            contr_can.r4=0x01;
            contr_can.r5=0x9d;
            contr_can.r7=0x10;//只修改这个值就可以了
        }
        */
    }
}

//连接断开
void WebSocket::socketDisconnected()
{
    qDebug()<<__FILE__<<__LINE__<<"socketDisconnected.";
}

//给客户端发送消息
void WebSocket::sendMessage(QString message)
{
    qDebug()<<__FILE__<<__LINE__<<"sendMessage.";
}

//给客户端发送消息
void WebSocket::slot_broadcast_msg(QString message)
{
    for (auto socket:m_clients) {
        socket->sendTextMessage(message);
    }
}

//推送消息给客户端binary
void WebSocket::slot_broadcast_binary(QByteArray content)
{
    int len = content.size();
    char json_buf[1024];
    memset(json_buf,'\0',sizeof(json_buf));

    if(len>=104){
        QString message = QString(content);
        message.remove(QRegExp("\\s"));

        char *str_ascii = message.toLatin1().data();
        int ascii_len =message.toLatin1().length();
        int bin_len = ascii_len/2;

        unsigned char frame[1024];
        hexs_to_binary(str_ascii,ascii_len,frame);

        printf_hex(frame,bin_len);

        conver_xly_frame_to_json((const char*)frame,bin_len,json_buf);
    }else{
        char *frame = (char *)content.data();
        printf_hex((unsigned char*)frame,content.length());
        log_debug("buf len=%d %d",content.length(),content.size());
        log_debug("ascii_len");
        conver_xly_frame_to_json((const char*)frame,content.length(),json_buf);
    }

    //推送消息给客户端
    for (auto socket:m_clients) {
        socket->sendTextMessage(json_buf);
    }
}
