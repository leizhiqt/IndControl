#ifndef MODBUS_TCP_CLIENT_H
#define MODBUS_TCP_CLIENT_H
#include <QTcpSocket>
#include <QTcpServer>

class ModbusTcpClient: public QObject
{
Q_OBJECT
public:
    ModbusTcpClient();
    ~ModbusTcpClient();

    void toConnection(QString host,int port);
    void send(QString buf);

private slots:
    void onConnection();
    void socketDisconnected();
    void readRes();

    void sendMessages();

private:
    QTcpSocket *visualTcpSocket;

    QTcpServer *tcpSer;

//    QString host;
    int port;

};

#endif // CANZLG_H
