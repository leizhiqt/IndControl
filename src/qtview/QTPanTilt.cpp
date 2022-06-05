#include "ControlMain.h"
#include "QTPanTilt.h"
#include "DaHuaFactory.h"

QTPanTilt::QTPanTilt(QWidget *parent):QObject(parent),parent(parent)
{
    daHuaControll = DaHuaControll::getInstance();

    daHuaProduct =DaHuaFactory::CreateProduct(DAHUAPRODUCT::DAHUA_IOT);

    view_init();
    parent->hide();

    view_bind();
}

void QTPanTilt::view_init()
{
    pushButton_Up = new QPushButton(parent);
    pushButton_Up->setObjectName(QString::fromUtf8("pushButton_Up"));
    pushButton_Up->setGeometry(QRect(60, 0, 40, 40));
    pushButton_Left = new QPushButton(parent);
    pushButton_Left->setObjectName(QString::fromUtf8("pushButton_Left"));
    pushButton_Left->setGeometry(QRect(10, 50, 40, 40));
    pushButton_Right = new QPushButton(parent);
    pushButton_Right->setObjectName(QString::fromUtf8("pushButton_Right"));
    pushButton_Right->setGeometry(QRect(110, 50, 40, 40));
    pushButton_Down = new QPushButton(parent);
    pushButton_Down->setObjectName(QString::fromUtf8("pushButton_Down"));
    pushButton_Down->setGeometry(QRect(60, 100, 40, 40));
    pushButton_RUp = new QPushButton(parent);
    pushButton_RUp->setObjectName(QString::fromUtf8("pushButton_RUp"));
    pushButton_RUp->setGeometry(QRect(110, 0, 40, 40));
    pushButton_LDown = new QPushButton(parent);
    pushButton_LDown->setObjectName(QString::fromUtf8("pushButton_LDown"));
    pushButton_LDown->setGeometry(QRect(10, 100, 40, 40));
    pushButton_LUp = new QPushButton(parent);
    pushButton_LUp->setObjectName(QString::fromUtf8("pushButton_LUp"));
    pushButton_LUp->setGeometry(QRect(10, 0, 40, 40));
    pushButton_RDown = new QPushButton(parent);
    pushButton_RDown->setObjectName(QString::fromUtf8("pushButton_RDown"));
    pushButton_RDown->setGeometry(QRect(110, 100, 40, 40));
    pushButton_FocusIn = new QPushButton(parent);
    pushButton_FocusIn->setObjectName(QString::fromUtf8("pushButton_FocusIn"));
    pushButton_FocusIn->setGeometry(QRect(170, 50, 40, 40));
    pushButton_FoucsOut = new QPushButton(parent);
    pushButton_FoucsOut->setObjectName(QString::fromUtf8("pushButton_FoucsOut"));
    pushButton_FoucsOut->setGeometry(QRect(220, 50, 40, 40));
    pushButton_ZoomIn = new QPushButton(parent);
    pushButton_ZoomIn->setObjectName(QString::fromUtf8("pushButton_ZoomIn"));
    pushButton_ZoomIn->setGeometry(QRect(170, 0, 40, 40));
    pushButton_ZoomOut = new QPushButton(parent);
    pushButton_ZoomOut->setObjectName(QString::fromUtf8("pushButton_ZoomOut"));
    pushButton_ZoomOut->setGeometry(QRect(220, 0, 40, 40));
    label = new QLabel(parent);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 150, 31, 21));
    spinBox_SetpLen = new QSpinBox(parent);
    spinBox_SetpLen->setObjectName(QString::fromUtf8("spinBox_SetpLen"));
    spinBox_SetpLen->setGeometry(QRect(60, 150, 42, 22));
    spinBox_SetpLen->setMinimum(1);
    spinBox_SetpLen->setMaximum(8);
    pushButton_3D = new QPushButton(parent);
    pushButton_3D->setObjectName(QString::fromUtf8("pushButton_3D"));
    pushButton_3D->setGeometry(QRect(60, 50, 40, 40));
    pushButtonTest = new QPushButton(parent);
    pushButtonTest->setObjectName(QString::fromUtf8("pushButtonTest"));
    pushButtonTest->setGeometry(QRect(10, 180, 251, 41));
    pushButton_close = new QPushButton(parent);
    pushButton_close->setObjectName(QString::fromUtf8("pushButton_close"));
    pushButton_close->setGeometry(QRect(0, 250, 41, 23));
    pushButton_StartRecord = new QPushButton(parent);
    pushButton_StartRecord->setObjectName(QString::fromUtf8("pushButton_StartRecord"));
    pushButton_StartRecord->setGeometry(QRect(174, 100, 81, 23));
    pushButton_StopRecord = new QPushButton(parent);
    pushButton_StopRecord->setObjectName(QString::fromUtf8("pushButton_StopRecord"));
    pushButton_StopRecord->setGeometry(QRect(174, 130, 81, 23));
    pushButton = new QPushButton(parent);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(40, 310, 75, 23));
    spinBox = new QSpinBox(parent);
    spinBox->setObjectName(QString::fromUtf8("spinBox"));
    spinBox->setGeometry(QRect(120, 310, 42, 22));
    spinBox->setMaximum(9);
    spinBox->setMinimum(1);
    spinBox->setValue(1);
    spinBox->setDisplayIntegerBase(10);

    pushButton_Up->setText(QApplication::translate("MainWindow", "\344\270\212", nullptr));
    pushButton_Left->setText(QApplication::translate("MainWindow", "\345\267\246", nullptr));
    pushButton_Right->setText(QApplication::translate("MainWindow", "\345\217\263", nullptr));
    pushButton_Down->setText(QApplication::translate("MainWindow", "\344\270\213", nullptr));
    pushButton_RUp->setText(QApplication::translate("MainWindow", "\345\217\263\344\270\212", nullptr));
    pushButton_LDown->setText(QApplication::translate("MainWindow", "\345\267\246\344\270\213", nullptr));
    pushButton_LUp->setText(QApplication::translate("MainWindow", "\345\267\246\344\270\212", nullptr));
    pushButton_RDown->setText(QApplication::translate("MainWindow", "\345\217\263\344\270\213", nullptr));
    pushButton_FocusIn->setText(QApplication::translate("MainWindow", "\347\204\246\350\267\235+", nullptr));
    pushButton_FoucsOut->setText(QApplication::translate("MainWindow", "\347\204\246\350\267\235-", nullptr));
    pushButton_ZoomIn->setText(QApplication::translate("MainWindow", "\347\274\251\346\224\276+", nullptr));
    pushButton_ZoomOut->setText(QApplication::translate("MainWindow", "\347\274\251\346\224\276-", nullptr));
    label->setText(QApplication::translate("MainWindow", "\346\255\245\351\225\277:", nullptr));
    pushButton_3D->setText(QApplication::translate("MainWindow", "3D", nullptr));
    pushButtonTest->setText(QApplication::translate("MainWindow", "\346\222\255\346\224\276", nullptr));
    pushButton_close->setText(QApplication::translate("MainWindow", ">>", nullptr));
    pushButton_StartRecord->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\345\275\225\345\203\217", nullptr));
    pushButton_StopRecord->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\345\275\225\345\203\217", nullptr));
    pushButton->setText(QApplication::translate("MainWindow", "\346\250\241\345\274\217\350\256\276\347\275\256", nullptr));
}

void QTPanTilt::view_bind()
{
    //云台控制 上下左右
    connect(pushButton_Up, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_UP,false);
    });

    connect(pushButton_Up, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_UP,true);
    });

    connect(pushButton_Down, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_DOWN,false);
    });

    connect(pushButton_Down, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_DOWN,true);
    });

    connect(pushButton_Left, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFT,false);
    });

    connect(pushButton_Left, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFT,true);
    });

    connect(pushButton_Right, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHT,false);
    });

    connect(pushButton_Right, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHT,true);
    });

    //云台控制 左右 上下左右
    connect(pushButton_LUp, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFTUP,false);
    });

    connect(pushButton_LUp, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFTUP,true);
    });

    connect(pushButton_RUp, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTUP,false);
    });

    connect(pushButton_RUp, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTUP,true);
    });

    connect(pushButton_LDown, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFTDOWN,false);
    });

    connect(pushButton_LDown, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFTDOWN,true);
    });

    connect(pushButton_RDown, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTDOWN,false);
    });

    connect(pushButton_RDown, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_direction_control("1000001$1$0$0",dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTDOWN,true);
    });

    //云台控制 镜头缩放
    connect(pushButton_ZoomIn, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_camera_control("1000001$1$0$0",dpsdk_camera_operation_e::DPSDK_CORE_PTZ_ADD_ZOOM,false);
    });

    connect(pushButton_ZoomIn, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_camera_control("1000001$1$0$0",dpsdk_camera_operation_e::DPSDK_CORE_PTZ_ADD_ZOOM,true);
    });

    connect(pushButton_ZoomOut, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_camera_control("1000001$1$0$0",dpsdk_camera_operation_e::DPSDK_CORE_PTZ_REDUCE_ZOOM,false);
    });

    connect(pushButton_ZoomOut, &QPushButton::released, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        daHuaProduct->pt_camera_control("1000001$1$0$0",dpsdk_camera_operation_e::DPSDK_CORE_PTZ_REDUCE_ZOOM,true);
    });

    //隐藏右边
    connect(pushButton_close, &QPushButton::pressed, [=](){
        qDebug() << "btnStopRealPlay btn clicked.";
        parent->hide();
    });

    //录像
    connect(pushButton_StartRecord, &QPushButton::clicked, [=](){
        qDebug() << "pushButton_StartRecord btn clicked.";
//        DaHuaControll *daHuaControll=controlMain->daHuaControll;
        QString filePath="d:/"+getDateTimeForStr()+".avi";
        qDebug() << filePath;
        daHuaControll->StartRecord(filePath);
    });

    connect(pushButton_StopRecord, &QPushButton::clicked, [=](){
        qDebug() << "pushButton_StopRecord btn clicked.";
        daHuaControll->StopRecord();
    });

    connect(pushButton, &QPushButton::clicked, [=](){
        MainWindow *mWin=controlMain->mWin;
        mWin->qtScreen->update_screen(spinBox->value());
    });

    connect(pushButtonTest, &QPushButton::clicked, [=](){
        MainWindow *mWin=controlMain->mWin;
        daHuaControll->testPlay(mWin->qtScreen->getHwnd(spinBox->value()-1));
        qDebug() << daHuaControll->m_nSeq;
//        mWin->qtScreen->setSeq(spinBox->value()-1,daHuaControll->m_nSeq);
    });
}
