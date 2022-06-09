#include "ControlMain.h"
#include "UWLog.h"
#include "WTcpClient.h"

#pragma execution_character_set("utf-8")

ControlMain::ControlMain(){
    log_open("IndControl.txt");

    mWin = new CanOpenUI();
    //mWin->setWindowTitle(QString::fromLocal8Bit("北斗伺服器"));
    mWin->setWindowTitle("CanOpen|ModBus|Tcp");
//    mWin->hide();
    mWin->show();
#ifdef ENABLE_OPENCAN
    canopenQThread = new CANopenQThread();
#endif

//    daHuaControll = DaHuaControll::getInstance();
//    command = Command::getInstance();
//    dbServer = BDServer::getInstance(8888);

//    //register("PGSQLDriveHelper","pgHelper");

//    qDebug()<< "daHuaControll: "<<daHuaControll<<" static:"<<ControlMain::daHuaControll;
//    qDebug()<< "dbServer: "<<dbServer<<" static:"<<ControlMain::dbServer;
//    qDebug()<<"start";

//    websocketServer = BDServer::getInstance(8000);
//    deviceServer = BDServer::getInstance(8001);
}

ControlMain::~ControlMain(){
//    delete mWin;
//    delete dbServer;
//    delete command;
//    delete daHuaControll;

    stop_tcp_client_th();

    log_close();
    qDebug()<<"destroy and Exit";
}
