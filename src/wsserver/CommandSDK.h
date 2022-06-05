#ifndef COMMAND_H
#define COMMAND_H
#include "DaHuaControll.h"
#include "BDServer.h"
#include "mainwindow.h"

#include <QProcess>

class Command: public QObject
{
    Q_OBJECT
private:
    QProcess *listProcess;
    DaHuaProduct *daHuaProduct;

    Command();//1.构造函数私有
    Command(const Command &);//拷贝构造函数不实现，防止拷贝产生多个实例
    Command & operator =  (const Command &);//复制构造函数不实现，防止赋值产生多个实例

public:
//    Command();//1.构造函数私有
    static Command * getInstance()//2.提供全局访问点
    {
        static Command singleCommand;//3.c++11保证了多线程安全，程序退出时，释放资源
        return &singleCommand;
    }

    ~Command();

    //virtual void processTextMessage(QString message);
    //截图
    //返回base64 的图片数据
    QString dahua_save_img(DaHuaControll *daHuaControll,int seq,QString filename);
    void dahua_json_login(QJsonObject jo,DaHuaControll *daHuaControll);
    void dahua_json_loadgroupinfo(QJsonObject jo,DaHuaControll *daHuaControll,BDServer *dbServer);

    void dahua_json_startrealplay(QJsonObject jo,DaHuaControll *daHuaControll, MainWindow *mWin);
    void dahua_json_stoprealplay(QJsonObject jo,DaHuaControll *daHuaControll,MainWindow *mWin);
    void dahua_json_capture(QJsonObject jo,DaHuaControll *daHuaControll,BDServer *dbServer,MainWindow *mWin);
    void dahua_json_setwindowpos(QJsonObject jo,MainWindow *mWin);
    void dahua_json_setsplitscreen(QJsonObject jo,MainWindow *mWin);
    void dahua_json_setwindowvisble(QJsonObject jo,MainWindow *mWin);

    //
    void dahua_json_ptdc(QJsonObject jo,DaHuaProduct *daHuaProduct);
    void dahua_json_ptcc(QJsonObject jo,DaHuaProduct *daHuaProduct);

    //预留接口函数
    void processCommand(QString message);

    //解析入口函数
    void processTextMessage(QString message);

Q_SIGNALS:
    void finished();
    void readyReadStandardOutput();

private Q_SLOTS:
    void onFinished();
    void readOutput();
};

#endif // COMMAND_H
