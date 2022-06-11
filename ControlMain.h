#ifndef CONTROLMAIN_H
#define CONTROLMAIN_H

//#define QTTRAY_ENABLE
//#undef QTTRAY_ENABLE

//#include "mainwindow.h"
//#include "ui_mainwindow.h"

//#include "ScreenshotWidget.h"
//#include "VideoWidget.h"
//#include "QTPanTilt.h"

//#include "ui_mainwindow.h"
//#include "JSONUtil.h"
//#include "MyDownloader.h"
//#include "PGSQLDriveHelper.h"

//#include "DaHuaControll.h"
//#include "BDServer.h"
//#include "Command.h"
//#include "QTTray.h"

#include "IOTUtil.h"
#include "Conf.h"
#include "CanOpenUI.h"

#ifdef ENABLE_OPENCAN
#include "CANopenQThread.h"
#endif

//#include "CANzlg.h"

#include <QDebug>

#pragma execution_character_set("utf-8")

class ControlMain
{
public:
    ControlMain();
    ~ControlMain();

    Conf *conf;

    CanOpenUI *mWin;
#ifdef ENABLE_OPENCAN
    CANopenQThread *canopenQThread;
#endif
};

extern ControlMain *controlMain;

#endif // CONTROLMAIN_H
