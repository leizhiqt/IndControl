#ifndef CANOPENUI_H
#define CANOPENUI_H

#include <QMainWindow>

#include "Conf.h"
#include "QTTray.h"
#include "UIReadThread.h"
#include "TcpClientUtil.h"

namespace Ui {
class CanOpenUI;
}

class CanOpenUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit CanOpenUI(QWidget *parent = nullptr);
    ~CanOpenUI();

//    void call_open_zlgcan(QString ip,QString port);

    Ui::CanOpenUI *ui;
    TcpClientUtil *canClient;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    //鼠标移动窗口移动
    QPoint mPoint;

    //托盘
    QTTray *qtTray;

    UIReadThread *uiReadThread;


//     ModbusTcpClient *mtClient;

    void set_default_UI();
};

#endif // CANOPENUI_H
