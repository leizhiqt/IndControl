#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H
//漂浮截图
#include <QWidget>
#include <QHBoxLayout>

class ScreenshotWidget:public QWidget
{
    Q_OBJECT
public:
    explicit ScreenshotWidget(QWidget *parent = nullptr,int *seq=nullptr);

protected:
    //画图 显示视频
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void leaveEvent(QEvent *event);

private:
    bool isClicked;

    QHBoxLayout *horizontalLayout;

    //视频序号
    int *seq;
};

#endif // SCREENSHOTWIDGET_H
