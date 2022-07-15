﻿#pragma execution_character_set("utf-8")

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QtWebSockets/QtWebSockets>
#include <QtWebSockets/QWebSocketServer>

class WebSocket :  public QThread
{
    Q_OBJECT

private:
    QList<QWebSocket *> m_clients;

    bool still;

    QWebSocketServer *m_websocketserver;
    quint16 port;


    //1.构造函数私有
    WebSocket(quint16 port);

    //拷贝构造函数不实现，防止拷贝产生多个实例
    WebSocket(const WebSocket &);

    //复制构造函数不实现，防止赋值产生多个实例
    WebSocket & operator =  (const WebSocket &);


public:
    ~WebSocket();
    static WebSocket * getInstance(quint16 port)
    {
        static WebSocket singleWebSocket(port);
        return &singleWebSocket;
    }
    QWebSocket *client_socket;
    void init();
    void run();
    void stop();
    void sendMessage(QString message);

    //增加Q_INVOKABLE声明
    Q_INVOKABLE void pushMessageToClients(QString message);

    void (*fun_processText)(QString);

Q_SIGNALS:

public Q_SLOTS:
    void onNewConnection();
    void recvTextMessage(QString message);
    void socketDisconnected();
    void recvBinaryMessage(QByteArray message);
};
#endif //WEBSOCKET_H
