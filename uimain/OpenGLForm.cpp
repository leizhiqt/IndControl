#include "OpenGLForm.h"
#include "ui_OpenGLForm.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <unistd.h>

OpenGLForm::OpenGLForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenGLForm)
{
    ui->setupUi(this);

//    this->setRect(parent->rect());
    setAttribute(Qt::WA_TranslucentBackground, true);
//    this->setGeometry(parent->geometry());
//    this->setGeometry(QOpenGLWidget::geometry());
    producer_data = std::thread(&OpenGLForm::readyData,this);
//    usleep(500);
    producer_data.detach();

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
    int width = this->width();
    int height = this->height();

    while(true)
    {
//        waterfallPlot.clear();
        int color;
        QVector<QColor> row;
        for(int j=0;j<width;j++){
            color = j%255;
            row.append(QColor(color, color, color));
        }

//        std::lock_guard<std::mutex> lk(mtx);

        waterfallPlot.append(row);
        if(waterfallPlot.size()>height)
        {
            waterfallPlot.removeFirst();
        }

        usleep(1000*15);
    }
}

void OpenGLForm::paintEvent(QPaintEvent *e)
{

//    this->setGeometry(QOpenGLWidget::geometry());
//    QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(Qt::green, 2, Qt::SolidLine);

    QPainter painter(this);
    painter.setPen(pen);
//Qt::SolidLine Qt::DotLine

//    painter.drawRect(this->rect());
//    painter.drawRect(160, 20, 50, 40);

    //QColor最后一个参数代表背景的透明度
    painter.fillRect(this->rect(), QColor(255, 255, 255, 70));
//    painter.drawLine(QPoint(0, 0), QPoint(100, 100));
//    painter.drawRect(160, 20, 50, 40);
    painter.drawRect(this->rect());

//    int height = this->rect().bottom();

//    std::lock_guard<std::mutex> lk(mtx);

    const int optimize=4;

    for(int y=0;y<waterfallPlot.size()/optimize;y++){
        for(int x=0;x<waterfallPlot[y].size()/optimize;x++)
        {
            painter.setPen(QPen(waterfallPlot[optimize*y][optimize*x],optimize));
            painter.drawPoint(QPoint(optimize*x,-optimize*y+270));
        }
    }

    e->ignore();
}
