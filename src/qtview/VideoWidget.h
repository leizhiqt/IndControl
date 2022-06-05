#ifndef SHAPEWIDGET_H
#define SHAPEWIDGET_H
//单视频播放
//并传入截图对象

#include <QWidget>
#include <QHBoxLayout>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr,QWidget *screenshot_widget=nullptr);

protected:
    //渲染事件
    void paintEvent(QPaintEvent *event);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QWidget *widget_screenshot;
    QHBoxLayout *horizontalLayout;
};

#endif // SHAPEWIDGET_H
