#pragma execution_character_set("utf-8")

#include "ControlMain.h"
#include "UWLog.h"
#include "ProtocolWebJson.h"

ControlMain::ControlMain(){

    conf = Conf::getInstance();

    mWin = new CanOpenUI();
    mWin->setWindowTitle("CanOpen|ModBus|Tcp");

#ifdef ENABLE_GUI
    mWin->show();
#endif

#ifndef ENABLE_GUI
    mWin->hide();
#endif

    //启动位姿系统服务（向位姿系统下发配置参数，同时接收位姿系统上报的数据
    xly_srv = (server_info_t *)malloc(sizeof(server_info_t));
    xly_srv->port=conf->xueLangPort;
    xly_srv->s_forever=1;
    xly_srv->recvFun=recvXly;
    xly_srv->cliens_p =&xly_cliens;

    tcp_server_start(xly_srv);
    log_debug("xly_srv:%p",xly_srv);

    //启动ModBus,监听502端口，接收来自控制台的指令
    //解析后封装成CAN协议转发至22004
    modbus_srv = (server_info_t *)malloc(sizeof(server_info_t));
    modbus_srv->port=conf->modBusPort;
    modbus_srv->s_forever=1;
    modbus_srv->recvFun=recvModbusTcp;
    modbus_srv->cliens_p =&modbus_cliens;
    tcp_server_start(modbus_srv);
    log_debug("modbus_srv:%p",modbus_srv);


    command = new ProtocolWebJson();

    //启动WEBSOCKET服务，接收来自JAVA的指令，同时向JAVA推送工况数据
    webSocket = WebSocket::getInstance(conf->websocketPort,command);

    try_do();

    log_debug("boot OK");
}

ControlMain::~ControlMain(){

    qDebug()<<"destroy and Exit";
}

void ControlMain::th_do(){
    log_debug("th_do");
//    can_client.ip=conf->canOpenIp.toLatin1().data();
    strcpy(can_client.ip,conf->canOpenIp.toLatin1().data());
    can_client.port=conf->canOpenPort;

    strcpy(modbus_client.ip,conf->canOpenIp.toLatin1().data());
    modbus_client.port=conf->canOpenPort;

    int ret=-1;
    while(1)
    {
        //CANOPEN客户连接，用于转发控制指令，同时接收工况数据并转发给JAVA

        ret=start_tcp_client_th(&can_client);
        if(!ret)
            return;

        ret=start_tcp_client_th(&modbus_client);
        if(!ret)
            return;


        //CanOpen 客户端连接
        //start_tcp_client_th(conf->canOpenIp.toLatin1().data(),conf->canOpenPort);
        Sleep(1000*60*5);
    }
}

void ControlMain::try_do(){
    std::thread th(&ControlMain::th_do,this);
    Sleep(150);
    th.detach();
}
