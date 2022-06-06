#include <QMouseEvent>

#include "CanOpenUI.h"
#include "ControlMain.h"
#include "UWLog.h"
#include "Windows.h"
#include "ModbusTcp.h"

#pragma execution_character_set("utf-8")

extern int tcp_send();
extern int start_tcp(char * host,int port);

CanOpenUI::CanOpenUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CanOpenUI)
{
    ui->setupUi(this);
    set_default_UI();

    uiReadThread = new UIReadThread(ui);
//    canClient = new TcpClientUtil();

    ui->ModBusView->append("Modbus Server 已经开启! 502");
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
//            canClient->toConnection(ui->CanIP->text(),ui->CanPort->text().toInt());
        char ip[]="192.168.2.152";
        start_tcp(ip,ui->CanPort->text().toInt());
    });

    connect(ui->sendCan, &QPushButton::clicked, [=](){
        uint8_t can_data[13];
           memset(can_data,'\0',sizeof(can_data));
           can_data[0]=0x43;
           can_data[1]=0x00;
           can_data[2]=0x00;
           can_data[3]=0x01;
           can_data[4]=0x8a;

           can_data[5]=0x00;//b0
           can_data[6]=0x00;//b1
           can_data[7]=0x0;//b2
           can_data[8]=0x01;//b3
           can_data[9]=0x00;//b4
           can_data[10]=0x00;//b5
           can_data[11]=0x00;//b6
           can_data[12]=0x00;//b7

//            canClient->send(can_data);
           tcp_send();
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
