#include "BDServer.h"
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>
#include <QSharedMemory>
#include <QBuffer>

#pragma execution_character_set("utf-8")

BDServer::BDServer(quint16 port)
{
    this->port=port;
    init();

    this->start();
    qDebug()<<"listen";
}

void BDServer::init()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    m_websocketserver = new QWebSocketServer("server", QWebSocketServer::NonSecureMode);
    connect(m_websocketserver, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    m_websocketserver->listen(QHostAddress::Any,port);
}

BDServer::~BDServer()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    stop();
}

void BDServer::run()
{
    while(1){
        //qDebug() << __FUNCTION__;
        sleep(180);
    }
}

void BDServer::stop()
{
    qDebug() << __FUNCTION__;

    for (int i = 0; i < m_clients.size(); ++i)
    {
        QWebSocket *csocket = (QWebSocket *)m_clients.at(i);
        if(csocket->state()==QAbstractSocket::SocketState::ConnectedState)
            csocket->close();

        m_clients.removeAt(i);
        //qDebug() << "removeAt"<< csocket->close();
        //m_clients.removeAt(i);       // 删除
        //m_clients.append(csocket);   // 添加
    }

    m_websocketserver->close();
    m_websocketserver=NULL;
}

void BDServer::onNewConnection()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;

    client_socket = m_websocketserver->nextPendingConnection();

    connect(client_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(recvTextMessage(QString)));

    connect(client_socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(recvBinaryMessage(QByteArray)),Qt::UniqueConnection);

    connect(client_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    m_clients << client_socket;
}

void BDServer::recvBinaryMessage(QByteArray message)
{
    qDebug()<<__FILE__<<__LINE__<<"recvTextMessage is:" <<message.toStdString().data();
}


//收到消息
void BDServer::recvTextMessage(QString message)
{
    qDebug()<<__FILE__<<__LINE__<<"recvTextMessage is:" <<message.toLatin1().constData();
    printf("========================\n");
//    DaHuaIOT *dahuaIOT = DaHuaIOT::getInstance();
//    dahuaIOT->
//    QByteArray bytes =message.toLatin1();
//    const char* chp = bytes.data();
    //cmd->processCommand(message);
}

//连接断开的操作
void BDServer::socketDisconnected()
{
    qDebug()<<__FILE__<<__LINE__<<"socketDisconnected.";
}

//Send message to process
//给客户端发送消息
void BDServer::sendMessage(QString message)
{
    qDebug()<<__FILE__<<__LINE__<<"sendMessage.";
    client_socket->sendTextMessage(message);
}

void BDServer::pushMessageToClients(QString content)
{
    for (auto socket:m_clients) {
        socket->sendTextMessage(content);
    }
}
