#ifndef BD_SERVER_H
#define BD_SERVER_H

#include <QtWebSockets/QtWebSockets>
#include <QtWebSockets/QWebSocketServer>

class BDServer :  public QThread
{
    Q_OBJECT

private:
    bool still;

    QWebSocketServer *m_websocketserver;
    quint16 port;
    QWebSocket *client_socket;

    QList<QWebSocket *> m_clients;

    BDServer(quint16 port);//1.构造函数私有
    BDServer(const BDServer &);//拷贝构造函数不实现，防止拷贝产生多个实例
    BDServer & operator =  (const BDServer &);//复制构造函数不实现，防止赋值产生多个实例


public:
    ~BDServer();
    static BDServer & getInstance(quint16 port)
    {
        static BDServer singleBDServer(port);
        return singleBDServer;
    }

    void init();
    void run();
    void stop();
    void sendMessage(QString message);

    void (*fun_processText)(QString);

Q_SIGNALS:

private Q_SLOTS:
    void onNewConnection();
    void recvTextMessage(QString message);
    void socketDisconnected();
    void pushMessageToClients(QString content);

    void recvBinaryMessage(QByteArray message);

//protected:
    //void start()override;
    //void stop(bool& successfullyStopped)override;
    //void pause()override{}
    //void resume()override{}

};
#endif
