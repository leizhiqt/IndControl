#include "ScreenshotWidget.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

ScreenshotWidget::ScreenshotWidget(QWidget *parent,int *seq) :
    QWidget(parent)
{
    isClicked=false;
    setStyleSheet(".ScreenshotWidget {background-color:#ffeedd; \
                  background-image:url(:res/mask.png); \
                  background-repeat: no-repeat; background-origin: padding; \
                  background-position: left center; \
                  border:none; padding-left:1px;}");
    setGeometry(QRect(0,0,128, 42));
    this->hide();
}

void ScreenshotWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->type()==QMouseEvent::MouseButtonPress)
    {
        if(isClicked){
            isClicked=false;
            setStyleSheet(".ScreenshotWidget {background-color:#ffeedd;background-image:url(:res/mask.png);border:1px solid yellow;}");
        }else{
            isClicked=true;
            setStyleSheet(".ScreenshotWidget {background-color:yellow;background-image:url(:res/mask.png);border:1px solid yellow;}");

        }
        qDebug() << __FILE__<< __LINE__<< "this:" << this << "event:"<<event;
        qDebug() << __FILE__<< __LINE__<< "seq:" << seq;
    }
    event->accept();//不传递父组件
}

void ScreenshotWidget::paintEvent(QPaintEvent *event)
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
//    QWidget::paintEvent(event);//传递信号
    event->accept();//不传递父组件
//    event->ignore();//继续传给父组件
}

void ScreenshotWidget::leaveEvent(QEvent *event){
//    qDebug() << __FILE__<< __LINE__ << "mous out";
//    qDebug() << __FILE__<< __LINE__ << this << " QEvent:" <<event;

//    widget_screenshot->setStyleSheet("#widget_screenshot{background-color:#1122bb; border:1px solid #red;}");
        hide();

//    QWidget::leaveEvent(event);
        event->accept();//不传递父组件
//        event->ignore();
}

//    setStyleSheet(".ScreenshotWidget {background-color:#ddccee;border:1px solid red;}");//调试
//    setStyleSheet(".ScreenshotWidget {background-color:transparent;border:1px solid yellow;}");
//    setStyleSheet(".ScreenshotWidget {background-color:#ffeedd;background-image:url(:res/mask.png);border:1px solid yellow;}");

//    setGeometry(QRect(parent->x()+parent->width()-128-1, parent->y()+parent->height()-42-1,
//                      128, 42));

//    horizontalLayout = new QHBoxLayout(parent);
//    horizontalLayout->setSpacing(0);
//    horizontalLayout->setContentsMargins(0, 0, 0, 0);
//    horizontalLayout->setObjectName(QString::fromUtf8("ShapeWidgetLayout"));
//    horizontalLayout->setContentsMargins(0, 0, 0, 0);

//    horizontalLayout->addWidget(this);

//    setLayoutDirection(Qt::RightToLeft);
