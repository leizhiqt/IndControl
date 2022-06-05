#ifndef BD_SERVER_H
#define BD_SERVER_H

#include <Command.h>
#include <QtWebSockets/QtWebSockets>
#include <QtWebSockets/QWebSocketServer>

class BDServer :  public QThread
{
    Q_OBJECT

private:
    bool still;

    QWebSocketServer *m_websocketserver;
    quint16 port;

    QList<QWebSocket *> m_clients;

    ICommand *command;

    BDServer(quint16 port);//1.构造函数私有
    BDServer(const BDServer &);//拷贝构造函数不实现，防止拷贝产生多个实例
    BDServer & operator =  (const BDServer &);//复制构造函数不实现，防止赋值产生多个实例

public:
//    BDServer(quint16 port);//1.构造函数私有
    static BDServer * getInstance(quint16 port)//2.提供全局访问点
    {
        BDServer *newServer = new BDServer(port);//3.c++11保证了多线程安全，程序退出时，释放资源
        return newServer;
    }

    ~BDServer();

    void initialize();
    void run();
    void stop();

    void sendMessage(QString message);
    //void (*fun_processText)(QString);

    void registerAnalyse(ICommand *command);

//Q_SIGNALS:

private Q_SLOTS:
    void onNewConnection();
    void socketDisconnected();

    void recvTextMessage(QString message);

    void pushMessageToClients(QString content);

//protected:
    //void start()override;
    //void stop(bool& successfullyStopped)override;
    //void pause()override{}
    //void resume()override{}

};
#endif
