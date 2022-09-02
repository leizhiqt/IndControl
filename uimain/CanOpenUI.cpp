#pragma execution_character_set("utf-8")

#define WIN32_LEAN_AND_MEAN

#include <QMouseEvent>
#include "CanOpenUI.h"
#include "UWLog.h"
#include "WTcpClient.h"
#include "WTcpServer.h"
#include "Conf.h"
#include "OpenGLForm.h"

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

    //
    openGLForm = new OpenGLForm(this->ui->GLSlot);
    openGLForm->setStyleSheet(".GLSlot {background-color:#000000; border:1px solid green;}");

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
