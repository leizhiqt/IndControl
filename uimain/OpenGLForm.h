#ifndef OPENGLFORM_H
#define OPENGLFORM_H

#include <QWidget>
#include <thread>
#include <mutex>

namespace Ui {
class OpenGLForm;
}

class OpenGLForm : public QWidget
{
    Q_OBJECT

public:
    explicit OpenGLForm(QWidget *parent = nullptr);
    ~OpenGLForm();

    void start();

    void readyData();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::OpenGLForm *ui;

    QTimer *time;

//    std::mutex mtx; // 保护counter
    QList<QVector<QColor>> waterfallPlot;

    std::thread producer_data;
};

#endif // OPENGLFORM_H
