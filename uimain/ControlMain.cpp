﻿#pragma execution_character_set("utf-8")

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

    //启动位姿系统服务（向位姿系统下发配置参数，同时接收位姿系统上报的数据)
    xly_srv = (server_info_t *)malloc(sizeof(server_info_t));
    xly_srv->port=conf->xueLangPort;
    xly_srv->s_forever=1;
    xly_srv->recvFun=recvXly;
    xly_srv->cliens_p =&xly_cliens;
    tcp_server_start(xly_srv);
    //log_debug("xly_srv:%p",xly_srv);

    //启动ModBus,监听502端口，接收来自控制台的指令
    //解析后封装成CAN协议转发至22004
    modbus_srv = (server_info_t *)malloc(sizeof(server_info_t));
    modbus_srv->port=conf->modBusPort;
    modbus_srv->s_forever=1;
    modbus_srv->recvFun=recvModbusTcp;
    modbus_srv->cliens_p =&modbus_cliens;
    tcp_server_start(modbus_srv);
    //log_debug("modbus_srv:%p",modbus_srv);

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
    //canOpen客户端连接，用于转发设备控制指令，同时接收掘进机工况数据并转发给JAVA
    snprintf(can_client.ip,sizeof(can_client.ip),"%s",conf->canOpenIp.toLatin1().data());
    can_client.port=conf->canOpenPort;
    can_client.recvFun=can_recv;

    //连接Modbus协同控制器，用于请求自移尾机的工况数据
    snprintf(modbus_client.ip,sizeof(modbus_client.ip),"%s",conf->modbusSlaveIp.toLatin1().data());
    modbus_client.port=conf->modbusSlavePort;


    int ret=-1;
    bool can_ok=false;
    bool modbus_ok=false;
    char buf[3]="ok";

    while(1)
    {
        if(!can_ok){
            ret=start_tcp_client_th(&can_client);
            if(!ret){
                can_ok=true;
            }else{
                log_debug("tcp_client_connect_fail:%s %d",can_client.ip,can_client.port);
            }
        }

        if(!modbus_ok){
            ret=start_tcp_client_th(&modbus_client);
            if(!ret){
                modbus_ok=true;
            }else{
                log_debug("modbus_client_connect_fail:%s %d",modbus_client.ip,modbus_client.port);
            }
        }

        //心跳包检测
//        Sleep(1000*60*5); //时间太长了，假设程序启动时服务端没有启动，会等待很久才会重连
        Sleep(1000 * 5);
        if(can_ok){
            ret=tcp_client_send(can_client.acceptSocket,buf,sizeof(buf));
            if(ret<0)
                can_ok=false;
        }

        if(modbus_ok)
        {
            ret=tcp_client_send(modbus_client.acceptSocket,buf,sizeof(buf));
            if(ret<0)
                can_ok=false;
        }

        //end
    }
}

void ControlMain::try_do(){
    std::thread th(&ControlMain::th_do,this);
    Sleep(150);
    th.detach();
}
