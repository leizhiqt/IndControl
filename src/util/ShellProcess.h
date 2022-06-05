#ifndef SHELLPROCESS_H
#define SHELLPROCESS_H

#include <QThread>
#include <QtCore/QStringList>
#include <QProcess>

//  Ping IP的线程子类
class ShellProcess : public QThread
{
    Q_OBJECT
public:
    explicit ShellProcess(QObject *parent = nullptr):
        QThread(parent){}

    QStringList getIpRangle(); //获取需要扫描的IP列表
    void setIpRange(QStringList ipRange); //设置需要扫描的IP列表

    void ping(QString cmd);

    void exec_program(QString args);
    void kill_program();
protected:
    void run();

signals:
    void commandSuccessed(QString ip);
    void commandFailed(QString ip);

public slots:

private:
    QStringList ipRange; //需要扫描的IP列表
    QString execArgs;
    QProcess program;
};

#endif // SHELLPROCESS_H
