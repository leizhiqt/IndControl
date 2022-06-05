#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QGridLayout>

#include <QPushButton>

#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

//#include "VideoWidget.h"
//#include "QTPanTilt.h"
//#include "QTScreen.h"
//#include "QTTray.h"

class QTreeWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_default_UI();
    void SetDisplay(int x,int y,int width,int height);

//    //视频9格宫组件
//    QTScreen *qtScreen;
//    //云台组件
//    QTPanTilt *pan_tilt;

private:
    void register_hotkey();

private Q_SLOTS:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;

    //鼠标移动窗口移动
    QPoint mPoint;

//    //托盘
//    QTTray *qtTray;
};

#endif // MAINWINDOW_H
