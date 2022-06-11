#include "ControlMain.h"
#include "UWLog.h"
#include "WTcpClient.h"
#include "WTcpServer.h"

#pragma execution_character_set("utf-8")

ControlMain::ControlMain(){

    conf = Conf::getInstance();

    mWin = new CanOpenUI();
    //mWin->setWindowTitle(QString::fromLocal8Bit("北斗伺服器"));
    mWin->setWindowTitle("CanOpen|ModBus|Tcp");
//

#ifdef ENABLE_OPENCAN
    canopenQThread = new CANopenQThread();
#endif

#ifdef ENABLE_GUI
    mWin->show();
#endif

#ifndef ENABLE_GUI
    mWin->hide();
    tcp_server_start(502);
    start_tcp_client_th(conf->hostIp.toLatin1().data(),conf->port);
#endif

}

ControlMain::~ControlMain(){

    stop_tcp_client_th();

    qDebug()<<"destroy and Exit";
}
