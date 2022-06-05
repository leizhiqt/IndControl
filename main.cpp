#include <QApplication>
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QTextCodec>

#include "ControlMain.h"


ControlMain *controlMain;

int main(int argc, char *argv[])
{
    //App start
//    qInstallMessageHandler(myMessageOutput);
    //qDebug("[%s %d]:%s",__FILE__ , __LINE__,"main");

    //>>测试
//    PGSQLDriveHelper pgsql;
//    pgsql.pg_test();
    //MOD TEST
//    readEnv();
//    readHeadDisk();
    //<<测试

    QApplication qtApp(argc, argv);

    //启动主程序
    controlMain = new ControlMain();

    return qtApp.exec();
    //App end
}
