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

    qDebug()<<"开始监听"<<port;
}

/* 初始化WebSocket服务 */
void WebSocket::init()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    m_websocketserver = new QWebSocketServer("server", QWebSocketServer::NonSecureMode);
    connect(m_websocketserver, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    m_websocketserver->listen(QHostAddress::Any,port);
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
void WebSocket::recvBinaryMessage(QByteArray message)
{
    qDebug()<<__FILE__<<__LINE__<<"recvTextMessage is:" <<message.toStdString().data();

}


/* 收到消息 */
void WebSocket::recvTextMessage(QString message)
{
    qDebug()<<__FILE__<<__LINE__<<"recvTextMessage is:" <<message.toLatin1().constData();
    //Web端发来的消息
    const char *recvBuf = message.toLatin1().constData();
    tcp_client_send(&(controlMain->canOpenSocket),recvBuf,message.length());
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
void WebSocket::pushMessageToClients(QString content)
{
//    qDebug()<<"push data:"<<content;
    log_debug("sendTextMessage %s",content.toLatin1().constData());
    for (auto socket:m_clients) {
        socket->sendTextMessage(content);
    }
}
