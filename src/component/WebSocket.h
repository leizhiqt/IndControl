#pragma execution_character_set("utf-8")

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QtWebSockets/QtWebSockets>
#include <QtWebSockets/QWebSocketServer>
#include "WebSocketCommand.h"

class WebSocket :  public QThread
{
    Q_OBJECT

private:
    QList<QWebSocket *> m_clients;

    bool still;

    QWebSocketServer *m_websocketserver;
    quint16 port;

    WebSocketCommand* command;

    //1.构造函数私有
    WebSocket(quint16 port,WebSocketCommand* command);

    //拷贝构造函数不实现，防止拷贝产生多个实例
    WebSocket(const WebSocket &);

    //复制构造函数不实现，防止赋值产生多个实例
    WebSocket & operator =  (const WebSocket &);

public:
    ~WebSocket();
    static WebSocket * getInstance(quint16 port,WebSocketCommand* command)
    {
        static WebSocket singleWebSocket(port,command);
        return &singleWebSocket;
    }

    void init();
    void run();
    void stop();

Q_SIGNALS:
    void broadcast_binary(QByteArray message);
    void broadcast_msg(QString message);
    void broadcast_binary_move(QByteArray message);
    void broadcast_binary_can(QByteArray message);

public Q_SLOTS:
    void onNewConnection();
    void recvTextMessage(const QString &message);
    void socketDisconnected();
    void recvBinaryMessage(const QByteArray &message);
    //推送位姿数据给JAVA
    void slot_broadcast_binary(QByteArray content);
    //推送自移尾机数据给JAVA
    void slot_broadcast_binary_move(QByteArray content);
    //推送掘进机工况数据给JAVA
    void slot_broadcast_binary_can(QByteArray content);
    void slot_broadcast_msg(QString message);
};
#endif //WEBSOCKET_H
