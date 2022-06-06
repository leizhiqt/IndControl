#include <QMouseEvent>

#include "CanOpenUI.h"
#include "ControlMain.h"
#include "UWLog.h"
#include "Windows.h"
#include "ModbusTcp.h"

#pragma execution_character_set("utf-8")

CanOpenUI::CanOpenUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CanOpenUI)
{
    ui->setupUi(this);
    set_default_UI();

    uiReadThread = new UIReadThread(ui);
    canClient = new TcpClientUtil();

    ui->ModBusView->append("Modbus Server 已经开启! 1520");
}

CanOpenUI::~CanOpenUI()
{
    delete ui;
}


void CanOpenUI::set_default_UI()
{
    //setWindowFlags(Qt::WindowStaysOnTopHint|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::SubWindow);
//    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::SubWindow);
//    ui->centralWidget->setWindowFlags(Qt::FramelessWindowHint);

    //主层
//    ui->centralWidget->setStyleSheet("#centralWidget{background:#ffffff;border:1px solid red}");// yellow  red green
    //播放层
//    ui->widget_video->setStyleSheet("#widget_video{background:#ffffff;border:1px solid red;}");
    //云台层
//    ui->widget_right->setStyleSheet("#widget_right{background:#ffffff;border:1px solid green;border-left:0px;}");

    //9格宫/多格宫 组件
//    qtScreen = new QTScreen(ui->widget_video);

    //云台组件
//    pan_tilt = new QTPanTilt(ui->widget_right);

    //监听
    ui->pushButton->setText("协议停止");
    ui->pushButton->setStyleSheet("#pushButton {background-color:#A3A3A3;border:1px solid #aabbcc;}");

    connect(ui->pushButton, &QPushButton::clicked, [=](){

        if(can_stop==0){
            can_stop=1;
#ifdef ENABLE_OPENCAN
            controlMain->canopenQThread->wait();
#endif
            uiReadThread->wait();
            ui->pushButton->setText("协议停止");
            ui->pushButton->setStyleSheet("#pushButton {background-color:#A3A3A3;}");
            return;
        }

        if(can_stop==1){
            can_stop=0;

#ifdef ENABLE_OPENCAN
            controlMain->canopenQThread->start();
#endif
            Sleep(1);
            uiReadThread->start();
            Sleep(1);

            ui->pushButton->setText("协议开启");
            ui->pushButton->setStyleSheet("#pushButton {background-color:#3366cc;}");
            return;
        }
    });

    connect(ui->CanConn, &QPushButton::clicked, [=](){
            canClient->toConnection(ui->CanIP->text(),ui->CanPort->text().toInt());
    });

    connect(ui->ModConn, &QPushButton::clicked, [=](){
        modbus_tcp_thread_start(ui->ModIP->text().toLatin1().data(),ui->ModPort->text().toInt());
    });

    //托盘
    qtTray = new QTTray(this);
}

//void CanOpenUI::call_open_zlgcan(QString ip,QString port){
//    char *p_ip = ip.toLatin1().data();
//    char *p_port = port.toLatin1().data();
//    open_zlgcan(p_ip,p_port);
//}

void CanOpenUI::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mPoint = e->globalPos() - frameGeometry().topLeft();
        e->accept();
    }
    if (e->button() == Qt::RightButton) {
        close();
        qApp->exit(0);
    }
}

void CanOpenUI::mouseMoveEvent(QMouseEvent *e)
{
    //Note that the returned value is always Qt::NoButton
    //for mouse move events.
    if (e->buttons() & Qt::LeftButton) {
        move(e->globalPos() - mPoint);
        e->accept();
    }
}
