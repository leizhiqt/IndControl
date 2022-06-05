#ifndef TCPCLIENTUTIL_H
#define TCPCLIENTUTIL_H
#include <QTcpSocket>
#include <QTcpServer>

class TcpClientUtil: public QObject
{
Q_OBJECT
public:
    TcpClientUtil();
    ~TcpClientUtil();

    void toConnection(QString host,int port);
    void send(uint8_t *buf);

private slots:
    void onConnection();
    void socketDisconnected();
    void readRes();

//    void sendMessages();

private:
    QTcpSocket *visualTcpSocket;

//    QTcpServer *tcpSer;

//    QString host;
//    int port;

};

#endif // CANZLG_H
