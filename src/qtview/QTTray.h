#ifndef QTTRAY_H
#define QTTRAY_H

//托盘
#include <QSystemTrayIcon>
#include <QAction>

class QTTray:public QObject
{
    Q_OBJECT

public:
    explicit QTTray(QWidget *parent = nullptr);
    void ui_init();
    void ui_release();

protected:
    void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    int OnSystemTrayClicked(QSystemTrayIcon::ActivationReason reason);

private:
    QWidget *parent;
    //托盘
    QSystemTrayIcon *systemTray;
    QAction *minimumAct,*maximumAct,*restoreAct,*quitAct;
    QMenu *pContextMenu;
};
#endif // QTTRAY_H
