#pragma execution_character_set("utf-8")

#ifndef CANOPENUI_H
#define CANOPENUI_H

//托盘配置
//#define QTTRAY_ENABLE
//#undef QTTRAY_ENABLE

//UI配置
//#define ENABLE_GUI

#include "QTTray.h"
#include <QMainWindow>
#include "ui_CanOpenUI.h"

namespace Ui {
class CanOpenUI;
}

class CanOpenUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit CanOpenUI(QWidget *parent = nullptr);
    ~CanOpenUI();


protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void AppendText(const QString &text,int ch);
private slots:
    void SlotAppendText(const QString &text,int ch);

    //声明接口函数
public:
    void Append(const QString &text,int ch);

private:
    Ui::CanOpenUI *ui;

    //鼠标移动窗口移动
    QPoint mPoint;
    //托盘
    QTTray *qtTray;

    void set_default_UI();
};

#endif // CANOPENUI_H
