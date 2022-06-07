#ifndef QTCPCOMPONENTL_H
#define QTCPCOMPONENTL_H
#include <QTcpSocket>
#include <QTcpServer>

class QTcpComponentl: public QObject
{
Q_OBJECT
public:
    QTcpComponentl();
    ~QTcpComponentl();

    void toConnection(QString host,int port);
    void send(uint8_t *buf);

private slots:
    void onConnection();
    void socketDisconnected();
    void readRes();

//    void sendMessages();

public:
    QTcpSocket *visualTcpSocket;

//    QTcpServer *tcpSer;

//    QString host;
//    int port;
};

#endif // CANZLG_H
