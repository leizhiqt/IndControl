#ifndef QTCAMERA_H
#define QTCAMERA_H
//多画面 屏幕分割 9格宫以及多格宫

#include <QWidget>
#include <QGridLayout>
#include <Windows.h>

#include "QTCamera.h"

class QTScreen:public QObject
{
    Q_OBJECT
public:
    explicit QTScreen(QWidget *parent = nullptr);
    ~QTScreen();

    HWND getHwnd(int index);

    void setSeq(int index,int seq);

    int getSeq(int index);

    void play_at_screen(int index);

    void update_screen(const unsigned int count);

public:
    //传入参数 控件上渲染子组件
    QWidget *parent;

    //栅格布局
    QGridLayout *video_layout;

    //控制9格宫
    QVector<QTCamera *> cameras;

};

#endif // QTCAMERA_H
