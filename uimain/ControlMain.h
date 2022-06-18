#ifndef CONTROLMAIN_H
#define CONTROLMAIN_H

//#define QTTRAY_ENABLE
//#undef QTTRAY_ENABLE

#include <QDebug>
#include "IOTUtil.h"
#include "Conf.h"
#include "CanOpenUI.h"

#ifdef ENABLE_OPENCAN
#include "CANopenQThread.h"
#endif

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
