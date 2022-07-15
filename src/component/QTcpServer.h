#pragma execution_character_set("utf-8")

#ifndef QTCPSERVER_H
#define QTCPSERVER_H

#include<QTcpSocket>
#include<QTcpServer>

class QTcpServer :: public QObject
{
    Q_OBJECT
public:
    QTcpServer();
    ~QTcpServer();


private slots:
    void server_connect();
    void read_data();
    void disconnected();

private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // QTCPSERVER_H
