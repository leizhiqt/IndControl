#include "VideoWidget.h"
#include <QDebug>
#include <QPainter>
#include <QEvent>
#include <QStyleOption>
#include <QPaintEvent>

VideoWidget::VideoWidget(QWidget *parent,QWidget *_widget_screenshot) :
    QWidget(parent),widget_screenshot(_widget_screenshot)
{
//    setStyleSheet(".VideoWidget {background-color:#bb33cc; border:1px solid green;}");
    setStyleSheet(".VideoWidget {background-color:#000000; border:1px solid green;}");

    horizontalLayout = new QHBoxLayout(parent);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->setObjectName(QString::fromUtf8("VideoWidget"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    horizontalLayout->addWidget(this);
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
//    qDebug() << __FILE__<< __LINE__ << this << " QPaintEvent:" <<event;

    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);
//    QPainter painter(this);
//    char imgpath[]="G:/setmask/bg.jpg";
//    QImage img;
//    if(! ( img.load(imgpath) ) ) {
//            qDebug() << __FILE__<< __LINE__ << imgpath << "load error";
//    }
//    painter.drawImage(this->rect(),img);
//    QWidget::paintEvent(event);
//    event->accept();//不传递父组件
    event->ignore();
}

void VideoWidget::enterEvent(QEvent *event){
//    qDebug() << __FILE__<< __LINE__ << "mouse in";
//    qDebug() << __FILE__<< __LINE__ << this << " QEvent:" <<event;

//    widget_screenshot->setStyleSheet("#widget_screenshot{background-color:#ccaabb; border:1px solid red;}");

    if (widget_screenshot->geometry().contains(this->mapFromGlobal(QCursor::pos()))
            && widget_screenshot->isVisible())
    {
        qDebug() << __FILE__<< __LINE__ << "mouse in need nohing";
        return;
    }
    widget_screenshot->show();

//    QWidget::leaveEvent(event);
        event->accept();//不传递父组件
//        event->ignore();
}

void VideoWidget::leaveEvent(QEvent *event){
//    qDebug() << __FILE__<< __LINE__ << "mous out";
//    qDebug() << __FILE__<< __LINE__ << this << " QEvent:" <<event;

//    widget_screenshot->setStyleSheet("#widget_screenshot{background-color:#1122bb; border:1px solid #red;}");
    if (widget_screenshot->geometry().contains(this->mapFromGlobal(QCursor::pos())) )
    {
        qDebug() << __FILE__<< __LINE__ << "mouse in need nohing";
        return;
    }
    widget_screenshot->hide();

//    QWidget::leaveEvent(event);
        event->accept();//不传递父组件
//        event->ignore();
}
