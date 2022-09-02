#include "OpenGLForm.h"
#include "ui_OpenGLForm.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <unistd.h>
#include "UWLog.h"
#include <QStyleOption>
#include <QPainter>
#include <QRandomGenerator>

OpenGLForm::OpenGLForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenGLForm)
{
    ui->setupUi(this);

//    log_debug("%d",parent->width());
//    log_debug("%d",parent->height());

    resize(parent->width(), parent->height());

//    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet(".OpenGLForm {background-color:#FFFFFF; border:1px solid green;padding:1px;}");

    ui->time_fs->setText("");
    ui->time_fs->setStyleSheet("#time_fs {border:1px solid #FFFFFF;color:#FFFFFF;}");

    producer_data = std::thread(&OpenGLForm::readyData,this);

    start();
}

OpenGLForm::~OpenGLForm()
{
    delete ui;
}

void OpenGLForm::start()
{
    time = new QTimer(this);
    connect(time,&QTimer::timeout,this,[=](){
        static QTime time(QTime::currentTime());
        double key = time.elapsed()/1000.0;

        update();

        static double lastFpsKey = 0;
        static int  frameCount;
        ++frameCount;
        if(key - lastFpsKey>1){
            QString v_fs=QString("%1 FPS").arg(frameCount/(key-lastFpsKey), 0, 'f', 0);
            ui->time_fs->setText(v_fs);
            lastFpsKey = key;
            frameCount = 0;
        }
    });
    time->start(15);
//    time->start(0);
}

void OpenGLForm::readyData()
{
//    int x0 = this->x();
//    int y0 = this->y();
    int width = this->rect().width();
    int height = this->geometry().height();

    while(true)
    {
//        int color;
        QVector<QColor> row;
        for(int x=0;x<width;x++){
//            color = x%255;
            int color_r=QRandomGenerator::global()->bounded(255);
            int color_g=QRandomGenerator::global()->bounded(255);
            int color_b=QRandomGenerator::global()->bounded(255);
            row.append(QColor(color_r, color_g, color_b));
        }

        waterfallPlot.append(row);
        if(waterfallPlot.size()>height)
        {
            waterfallPlot.removeFirst();
        }

//        log_debug("%d",waterfallPlot.size());

        usleep(1000*15);
    }
}

void OpenGLForm::paintEvent(QPaintEvent *e)
{
//    QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(Qt::green, 2, Qt::SolidLine);

    QPainter painter(this);
    painter.setPen(pen);

//    std::lock_guard<std::mutex> lk(mtx);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);

    const int optimize=2;
    for(int y=1;y<waterfallPlot.size()/optimize;y++){
        for(int x=1;x<waterfallPlot[y].size()/optimize;x++)
        {
            painter.setPen(QPen(waterfallPlot[optimize*y][optimize*x],optimize));
            painter.drawPoint(QPoint(optimize*x,-optimize*y+height()));
        }
    }

    e->ignore();//继续传给父组件
}
