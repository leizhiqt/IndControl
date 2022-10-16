#pragma execution_character_set("utf-8")

#include "WebSocket.h"
#include "ControlMain.h"
#include "ProtocolXly.h"
#include "ConvertUtil.h"
#include "UWLog.h"
#include "SelfMovingtail.h"

WebSocket::WebSocket(quint16 port,WebSocketCommand* command)
{
    this->port=port;
    init();

    this->command = command;
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
    connect(this, SIGNAL(broadcast_binary_move(QByteArray)), this, SLOT(slot_broadcast_binary_move(QByteArray)));
    connect(this, SIGNAL(broadcast_binary_can(QByteArray)), this, SLOT(slot_broadcast_binary_can(QByteArray)));
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
    QWebSocket *client_socket = m_websocketserver->nextPendingConnection();

    connect(client_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(recvTextMessage(QString)));

    connect(client_socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(recvBinaryMessage(QByteArray)),Qt::UniqueConnection);

    connect(client_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    m_clients.push_back(client_socket);
}

/* 收到消息 */
void WebSocket::recvBinaryMessage(const QByteArray &message)
{
    log_debug("WebSocket rece Byte Message");
    const char *recvBuf = message.begin();
    tcp_client_send((controlMain->can_client.acceptSocket),recvBuf,strlen(recvBuf));
    tcp_server_broadcast(controlMain->xly_srv,(char *)recvBuf,strlen(recvBuf));
}

/* 这是websocket 收到消息 */
void WebSocket::recvTextMessage(const QString &content)
{
    log_debug("WebSocket rece Text Message");
    command->recvTextMessage(content);
}

//连接断开
void WebSocket::socketDisconnected()
{
    qDebug()<<__FILE__<<__LINE__<<"socketDisconnected.";
}

//推送消息给客户端
void WebSocket::slot_broadcast_msg(QString message)
{
    for (auto socket:m_clients) {
        socket->sendTextMessage(message);
    }
}

//推送位姿数据给客户
void WebSocket::slot_broadcast_binary(QByteArray content)
{
    int len = content.size();
    char json_buf[4096];
    memset(json_buf,'\0',sizeof(json_buf));
    if(len > 104){
        QString message = QString(content);
        message.remove(QRegExp("\\s"));
        char *str_ascii = message.toLatin1().data();
        int ascii_len =message.toLatin1().length();
        int bin_len = ascii_len/2;
        unsigned char frame[1024];
        hexs_to_binary(str_ascii,ascii_len,frame);
        conver_xly_frame_to_json((const char*)frame,bin_len,json_buf);
    }else{
        char *frame = (char *)content.data();
        conver_xly_frame_to_json((const char*)frame,content.length(),json_buf);
    }
    //推送消息给客户端
    for (auto socket:m_clients) {
        socket->sendTextMessage(json_buf);
    }
}

//推送消息自移尾机工况数据给客户端
void WebSocket::slot_broadcast_binary_move(QByteArray content)
{
    int len = content.size();
    char json_buf[4096];
    memset(json_buf,'\0',sizeof(json_buf));

    if(len > 104){
        QString message = QString(content);
        message.remove(QRegExp("\\s"));
        char *str_ascii = message.toLatin1().data();
        int ascii_len =message.toLatin1().length();
        int bin_len = ascii_len/2;
        unsigned char frame[1024];
        hexs_to_binary(str_ascii,ascii_len,frame);
        conver_selfmovetail_to_json((const char*)frame,content.length(),json_buf);
    }else{
        char *frame = (char *)content.data();
        conver_selfmovetail_to_json((const char*)frame,content.length(),json_buf);
    }
    //推送消息给客户端
    for (auto socket:m_clients) {
        socket->sendTextMessage(json_buf);
    }
}

//推送掘进机工况数据给客户端
void WebSocket::slot_broadcast_binary_can(QByteArray content){
    log_debug("开始解析报文");
    char *buf = content.begin();
    int len = content.length();
    char json_buf[10240];
    memset(json_buf,'\0',sizeof(json_buf));

    if(len%13 !=0){
        log_debug("掘进机工况数据报文长度不正确：%d",len);
        return;
    }
    //计算有多少条报文
    int newLen = len / 13;
    log_debug("需要处理的报文条数: %d",newLen);
    //循环处理每条报文
    for(int d = 0; d < newLen; d++)
    {
        char *frame = buf+13*d;
        memset(json_buf,'\0',sizeof(json_buf));
        int nResult = conver_opencan_to_json((const char*)frame,13, json_buf);
        if(nResult == 1){
            for (auto socket:m_clients)
            {
                socket->sendTextMessage(json_buf);
            }
        }
        delete frame;
        frame = NULL;
    }
    delete buf;
    buf = NULL;
}


