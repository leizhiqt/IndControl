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

public:
    BDServer(quint16 port);
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
