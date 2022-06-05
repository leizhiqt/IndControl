#include "ControlMain.h"

#include <QDebug>
#include <QThread>
#include <QCloseEvent>
#include <Windows.h>
#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>

#define HOT_KEY_ALT_CTRL_M  0
#define HOT_KEY_CTRL_A      1
#define HOT_KEY_ALT_CTRL_H  2
#define HOT_KEY_ALT_CTRL_L  3
#define HOT_KEY_ALT_CTRL_R  4

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    set_default_UI();

    register_hotkey();

//    setFixedSize(QSize(930,480));
//    setMinimumSize(0,0);
//    setMaximumSize(QSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_default_UI()
{
    //setWindowFlags(Qt::WindowStaysOnTopHint|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::SubWindow);
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::CustomizeWindowHint|Qt::FramelessWindowHint|Qt::SubWindow);
    ui->centralWidget->setWindowFlags(Qt::FramelessWindowHint);

    //主层
    ui->centralWidget->setStyleSheet("#centralWidget{background:#ffffff;border:1px solid red}");// yellow  red green
    //播放层
    ui->widget_video->setStyleSheet("#widget_video{background:#ffffff;border:1px solid red;}");
    //云台层
    ui->widget_right->setStyleSheet("#widget_right{background:#ffffff;border:1px solid green;border-left:0px;}");

    //9格宫/多格宫 组件
    qtScreen = new QTScreen(ui->widget_video);

    //云台组件
    pan_tilt = new QTPanTilt(ui->widget_right);

    //托盘
    qtTray = new QTTray(this);
}

void MainWindow::register_hotkey(){
    //ALT + CTRL + L
    if (!RegisterHotKey(HWND(winId()), HOT_KEY_ALT_CTRL_R, MOD_ALT | MOD_CONTROL, 0x4C))
    {
        qDebug() << "注册热键 ALT + CTRL + R ok.";
    }

    //ALT + CTRL + R
    if (!RegisterHotKey(HWND(winId()), HOT_KEY_ALT_CTRL_R, MOD_ALT | MOD_CONTROL, 0x52))
    {
        qDebug() << "注册热键 ALT + CTRL + R ok.";
    }

    //ALT + CTRL + H
    if (!RegisterHotKey(HWND(winId()), HOT_KEY_ALT_CTRL_H, MOD_ALT | MOD_CONTROL, 0x48))
    {
        qDebug() << "注册热键 ALT + CTRL + H ok.";
    }
}

void MainWindow::SetDisplay(int x,int y,int width,int height)
{
    setGeometry(x,y,width,height);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY)
    {
        switch (msg->wParam) {
        case HOT_KEY_ALT_CTRL_L:
            if(ui->widget_video->isHidden())
                ui->widget_video->show();
            else
                ui->widget_video->hide();

            break;
        case HOT_KEY_ALT_CTRL_R:
            if(ui->widget_right->isHidden())
                ui->widget_right->show();
            else
                ui->widget_right->hide();

            break;
        case HOT_KEY_ALT_CTRL_H:
            if(this->isHidden())
                this->showNormal();
            else
                this->hide();

            break;
        default:
            qDebug() << "被注入了其他热键.";
        }
        return true;
    }
    return false;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
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

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    //Note that the returned value is always Qt::NoButton
    //for mouse move events.
    if (e->buttons() & Qt::LeftButton) {
        move(e->globalPos() - mPoint);
        e->accept();
    }
}
