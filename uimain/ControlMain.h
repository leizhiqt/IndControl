#pragma execution_character_set("utf-8")

#ifndef CONTROLMAIN_H
#define CONTROLMAIN_H

//#define QTTRAY_ENABLE
//#undef QTTRAY_ENABLE

#include <QDebug>
#include "IOTUtil.h"
#include "Conf.h"
#include "CanOpenUI.h"
#include "WebSocket.h"
#include "WTcpClient.h"
#include "WTcpServer.h"

#ifdef ENABLE_OPENCAN
#include "CANopenQThread.h"
#endif

#pragma execution_character_set("utf-8")
class ControlMain
{
private:
    void try_do();
    void th_do();
public:
    ControlMain();
    ~ControlMain();

    Conf *conf;

    CanOpenUI *mWin;
#ifdef ENABLE_OPENCAN
    CANopenQThread *canopenQThread;
#endif
    WebSocket *webSocket;

    SOCKET canOpenSocket;

    server_info_t *modbus_srv;
    server_info_t *xly_srv;
    std::vector<SOCKET> xly_cliens;
};

extern ControlMain *controlMain;

#endif // CONTROLMAIN_H
