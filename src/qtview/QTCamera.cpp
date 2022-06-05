#include "QTCamera.h"
#include "ScreenshotWidget.h"
#include "VideoWidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

QTCamera::QTCamera(QWidget *parent) : QWidget(parent)
{
    setStyleSheet(".QTCamera {background-color:#aabbcc;border:1px solid yellow;}");

    //截图
    seq = -100;
    widget_screenshot = new ScreenshotWidget(this,&seq);
    widget_screenshot->setObjectName(QString::fromUtf8("widget_screenshot"));

    //视频
    widget_video = new VideoWidget(this,widget_screenshot);
    widget_video->setObjectName(QString::fromUtf8("widget_video"));
//    widget_video->setGeometry(QRect(0, 0, 213, 160));
//    widget_video->setGeometry(this->geometry());
    widget_screenshot->raise();//最上层
}

QTCamera::~QTCamera()
{
    delete widget_video;
    delete widget_screenshot;
}

void QTCamera::paintEvent(QPaintEvent *event)
{
//    qDebug() << __FILE__<< __LINE__ << this << " QPaintEvent:" <<event;

    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);
//    QWidget::paintEvent(event);//传递信号
//    event->accept();//不传递父组件
    event->ignore();//继续传给父组件
}
