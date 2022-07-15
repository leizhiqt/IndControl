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
    //qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
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



/* 收到消息 */
void WebSocket::recvTextMessage(const QString &message)
{
    //Web端发来的消息
    const char *recvBuf = message.toLocal8Bit().constData();
    tcp_client_send((controlMain->canOpenSocket),recvBuf,strlen(recvBuf));
    tcp_server_broadcast(controlMain->xly_srv,(char *)recvBuf,strlen(recvBuf));
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


//推送消息给客户端
void WebSocket::slot_broadcast_msg(QString content)
{
//    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    for (auto socket:m_clients) {
        socket->sendTextMessage(content);
//        qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
//        socket->sendBinaryMessage(content);
//        socket->sendBinaryMessage("\n");
//        socket->flush();
    }
}
