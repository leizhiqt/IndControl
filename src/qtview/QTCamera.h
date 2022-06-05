#ifndef QTVIDEOSCREENSHOT_H
#define QTVIDEOSCREENSHOT_H
#include <QWidget>
//单独摄像头
class QTCamera:public QWidget
{
    Q_OBJECT
public:
    QTCamera(QWidget *parent = nullptr);
    ~QTCamera();

protected:
    //画图 显示视频
    void paintEvent(QPaintEvent *event);

public:
    //自定义控件
    QWidget *widget_video;
    QWidget *widget_screenshot;

    //视频序号
    int seq;
};

#endif // QTVIDEOSCREENSHOT_H
