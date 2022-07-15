#pragma execution_character_set("utf-8")

#define WIN32_LEAN_AND_MEAN

#include <QMouseEvent>
#include "CanOpenUI.h"
#include "ControlMain.h"
#include "UWLog.h"
#include "WTcpClient.h"
#include "WTcpServer.h"
#include "Conf.h"

CanOpenUI::CanOpenUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CanOpenUI)
{
    ui->setupUi(this);
    set_default_UI();

}

CanOpenUI::~CanOpenUI()
{
    delete ui;
}


void CanOpenUI::set_default_UI()
{
    connect(ui->CanConn, &QPushButton::clicked, [=](){
//        int ret = start_tcp_client_th(canOpenIp,canOpenPort);
//        QString msg;
//        msg.append(canOpenIp);
//        msg.append(":");
//        msg.append(canOpenPort);

//        if(ret==0)
//            //ui->textBrowser->append("连接成功");
//            log_debug("can连接成功");
//        else
//           //ui->textBrowser->append("连接失败");
//            log_debug("can连接失败");
//        //ui->textBrowser->append(msg);
//        log_debug(msg);
    });

    connect(ui->sendCan, &QPushButton::clicked, [=](){
        unsigned char frame[13];
            memset(frame,'\0',sizeof(frame));
            frame[0]=0x06;
            frame[1]=0x00;
            frame[2]=0x00;
            frame[3]=0x01;
            frame[4]=0x8a;

            frame[5]=0x00;//b0
            frame[6]=0x00;//b1
            frame[7]=0x0;//b2
            frame[8]=0x01;//b3
            frame[9]=0x00;//b4
            frame[10]=0x00;//b5
            frame[11]=0x00;//b6
            frame[12]=0x00;//b7

//           tcp_client_send(frame,sizeof(frame));
    });

    connect(ui->ModConn, &QPushButton::clicked, [=](){
//        int ret = tcp_server_start(modBusPort);
//        if(ret==0)
//            //ui->ModBusView->append("Modbus Server 开启成功! 502");
//            log_debug("Modbus开启成功");
//        else
//           //ui->ModBusView->append("Modbus Server 开启失败! 502");
//           log_debug("Modbus开启失败");
    });

    connect(this,SIGNAL(AppendText(QString,int)),this,SLOT(SlotAppendText(QString,int)));

    //托盘
    qtTray = new QTTray(this);
}

void CanOpenUI::Append(const QString &text,int ch)
{
    emit AppendText(text,ch);
}

void CanOpenUI::SlotAppendText(const QString &text,int ch)
{
    if(ch==1){
        //ui->ModBusView->append(text);
//        log_debug(text);
        return;
    }
    if(ch==2){
        //ui->textBrowser->append(text);
//        log_debug(text);
        return;
    }
}

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
    if (e->buttons() & Qt::LeftButton) {
        move(e->globalPos() - mPoint);
        e->accept();
    }
}
