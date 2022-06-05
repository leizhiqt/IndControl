#include "QTScreen.h"
#include "ScreenshotWidget.h"
#include "VideoWidget.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QtMath>

#pragma execution_character_set("utf-8")

QTScreen::QTScreen(QWidget *parent):
    QObject(parent),parent(parent)
{
    video_layout = new QGridLayout(parent);
    video_layout->setSpacing(0);
    video_layout->setContentsMargins(0,0,0,0);
    video_layout->setObjectName(QString::fromUtf8("video_layout"));
    parent->setLayout(video_layout);

//    update_screen(2);
}

QTScreen::~QTScreen()
{

}

HWND QTScreen::getHwnd(int index)
{
    QTCamera *camera=cameras.value(index);
    if(camera!=nullptr)
    {
        return (HWND)camera->widget_video->winId();
    }
    return nullptr;
}

int QTScreen::getSeq(int index)
{
//    QVector<QTCamera *>::iterator iter;
//    for (iter=cameras.begin();iter!=cameras.end();iter++)
//    {
//        QTCamera *cam_ptr = *iter;

//    }
    QTCamera *camera=cameras.value(index);
    if(camera!=nullptr)
    {
        return camera->seq;
    }
    return -1;
}

void QTScreen::setSeq(int index,int seq)
{
//    QVector<QTCamera *>::iterator iter;
//    for (iter=cameras.begin();iter!=cameras.end();iter++)
//    {
//        QTCamera *cam_ptr = *iter;

//    }
    QTCamera *camera=cameras.value(index);
    if(camera!=nullptr)
    {
        camera->seq=seq;
    }
}

void QTScreen::play_at_screen(int index)
{

}

void QTScreen::update_screen(const unsigned int count)
{

    int cur_count=count;

    if(count <=0 || count >9){
        cur_count=1;
    }

    //Odd even
//    bool isEven = (count%2==0) ? true:false;

    QTCamera *qtCamera=nullptr;

    int c = sqrt(cur_count);// 4的平方根=2

    //pow(m,n) m 的n 次方
    int try_count = pow(c,2);

    if(try_count<cur_count){
        c++;
    }

    qDebug() << "row/column" <<c<<" try_count:"<<try_count<<" length:"<<cameras.length();

    if(!cameras.empty()){
        for(int i=0;i<cameras.length();i++){
            qtCamera = cameras.value(i);
            video_layout->removeWidget(qtCamera);
            delete  qtCamera;
        }
        cameras.clear();
    }

    int current_count=0;
    for(int row=0;row<c;row++){
        for(int column=0;column<c;column++){
            qDebug() << "row:" <<row<<"column:"<<column;

            qtCamera = new QTCamera(parent);
            qtCamera->setObjectName("Camera"+current_count++);

            cameras.push_back(qtCamera);
            video_layout->addWidget(qtCamera,row,column);
//                if(current_count++==count) break;
        }
    }
    qDebug() << " length:"<<cameras.length();
}
