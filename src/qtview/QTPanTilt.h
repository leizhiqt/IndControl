#ifndef QTPANTILT_H
#define QTPANTILT_H
//云台控制

#include "DaHuaControll.h"

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

class QTPanTilt:public QObject
{
    Q_OBJECT
public:
    explicit QTPanTilt(QWidget *parent = nullptr);
    void view_init();
    void view_bind();
public:
    QPushButton *pushButton_Up;
    QPushButton *pushButton_Left;
    QPushButton *pushButton_Right;
    QPushButton *pushButton_Down;
    QPushButton *pushButton_RUp;
    QPushButton *pushButton_LDown;
    QPushButton *pushButton_LUp;
    QPushButton *pushButton_RDown;
    QPushButton *pushButton_FocusIn;
    QPushButton *pushButton_FoucsOut;
    QPushButton *pushButton_ZoomIn;
    QPushButton *pushButton_ZoomOut;
    QLabel *label;
    QSpinBox *spinBox_SetpLen;
    QPushButton *pushButton_3D;
    QPushButton *pushButtonTest;
    QPushButton *pushButton_close;
    QPushButton *pushButton_StartRecord;
    QPushButton *pushButton_StopRecord;
    QPushButton *pushButton;
    QSpinBox *spinBox;
private:
    QWidget *parent;
    DaHuaControll *daHuaControll;
    DaHuaProduct *daHuaProduct;
};

#endif // QTPANTILT_H
