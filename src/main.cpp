#include <QApplication>
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QString>

#include "ControlMain.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QString saveLogFile=getDateTimeForStr()+".txt";

    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toUtf8();

//    QString::fromLocal8Bit(localMsg.data());

    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        //调试信息
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        //警告信息
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        //严重错误
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        //致命错误
        strMsg = QString("Fatal:");
        break;
    case QtInfoMsg:
        strMsg = QString("Info:");
        break;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("[%1 %2 %3]%4")
            .arg(strDateTime).arg(context.file).arg(context.line).arg(localMsg.constData());

    QFile file;
    // 输出信息至文件中（读写、追加形式）
    file.setFileName(saveLogFile);

    file.open(QIODevice::ReadWrite | QIODevice::Append);

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF8"));//设置编码格式
    stream << QString::fromUtf8(localMsg.data()) << "\r\n";
//    printf("%s\n",strMessage.toLatin1().data());
    qDebug()<<QString::fromUtf8(localMsg.data());
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

//    char buf[512];
//    char save_path[512];
//    getDateTimeForStr(buf,sizeof (buf));
//    snprintf(save_path,sizeof(save_path),"%s.txt",buf);

ControlMain *controlMain;

int main(int argc, char *argv[])
{
    //App start
    qInstallMessageHandler(myMessageOutput);
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
