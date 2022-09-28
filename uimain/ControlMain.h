#pragma execution_character_set("utf-8")

#ifndef CONTROLMAIN_H
#define CONTROLMAIN_H

#include "IOTUtil.h"
#include "Conf.h"

#include "WebSocket.h"
#include "WTcpClient.h"
#include "WTcpServer.h"
#include "WModbusTcp.h"
#include "WebSocketCommand.h"

#include "CanOpenUI.h"

class ControlMain
{
private:
    void try_do();
    void th_do();

    //位姿连接的所有客户端
    std::vector<SOCKET> xly_cliens;
    //MODbus连接的所有客户端
    std::vector<SOCKET> modbus_cliens;

public:
    ControlMain();
    ~ControlMain();

    //系统配置
    Conf *conf;

    //UI
    CanOpenUI *mWin;

    //websocket 解析
    WebSocketCommand* command;
    //websocket
    WebSocket *webSocket;

    //交换机
    client_info can_client={0};

    //交换机
    client_info modbus_client={0};

    //modbusTCP
    server_info_t *modbus_srv;

    //位姿系统
    server_info_t *xly_srv;
};

extern ControlMain *controlMain;

#endif // CONTROLMAIN_H
