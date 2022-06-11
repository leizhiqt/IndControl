#include "QTTray.h"
#include <QMenu>
#include <QCoreApplication>
#include <QCloseEvent>

#pragma execution_character_set("utf-8")

QTTray::QTTray(QWidget *parent):parent(parent)
{
    ui_init();
}


void QTTray::ui_init()
{
    //托盘
    QIcon icon(":win.ico");
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(icon);
    systemTray->setToolTip("协议伺服器");
    systemTray->show();

    minimumAct = new QAction("Minimum Window", this);

    //Note the differences between hide() and showMinimized().
    //connect(minimumAct, SIGNAL(triggered()), this, SLOT(hide()));
    //maximumAct = new QAction("Maximum Window", this);
    //connect(maximumAct, SIGNAL(triggered()), this, SLOT(showMaximized()));
    //restoreAct = new QAction("Restore Window", this);
    //connect(restoreAct, SIGNAL(triggered()), this, SLOT(showNormal()));

//    quitAct = new QAction(QStringLiteral("退出"),this);
    quitAct = new QAction("退出",this);
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    // 关联托盘事件
    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(OnSystemTrayClicked(QSystemTrayIcon::ActivationReason)));

    pContextMenu = new QMenu();
    //pContextMenu->addAction(minimumAct);
    //pContextMenu->addAction(maximumAct);
    //pContextMenu->addAction(restoreAct);

    //QAction *isRun = new QAction("Runing", this);
    //pContextMenu->addAction(isRun);
    pContextMenu->addSeparator();
    //不允许退出
    pContextMenu->addAction(quitAct);

    //QLabel *label = new QLabel("Hello World!");
    //pContextMenu->addMenu(label);

    systemTray->setContextMenu(pContextMenu);
}

void QTTray::ui_release()
{
    delete pContextMenu;
    delete minimumAct;
    delete maximumAct;
    delete restoreAct;
    delete quitAct;
    delete systemTray;
}


void QTTray::closeEvent(QCloseEvent *event)
{
    if(systemTray->isVisible())
    {
//        this->hide();
//        systemTray->showMessage("Tips", "The program is running behind!");
        event->ignore();
    }
}

int QTTray::OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger
        || reason == QSystemTrayIcon::DoubleClick)
    {
    #ifdef ENABLE_GUI
        // 显示主窗口
        if(parent->isHidden())
            parent->showNormal();
        else
            parent->hide();

    #endif

    }
    return 0;
}
