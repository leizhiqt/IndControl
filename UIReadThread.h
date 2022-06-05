#ifndef UIREADTHREAD_H
#define UIREADTHREAD_H

#include <QThread>
#include "ui_CanOpenUI.h"

class UIReadThread:  public QThread
{
    Q_OBJECT
public:
    UIReadThread(){}

    ~UIReadThread();

    UIReadThread(Ui::CanOpenUI *ui);

protected:
    void run();

private:
    Ui::CanOpenUI *ui;
};

#endif // UIREADTHREAD_H
