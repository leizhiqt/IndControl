#include "ShellProcess.h"
#include <QDebug>
#include <QProcess>

QStringList ShellProcess::getIpRangle()
{
    return this->ipRange;
}

void ShellProcess::setIpRange(QStringList ipRange)
{
    this->ipRange = ipRange;
}

void ShellProcess::ping(QString cmd)
{
    QString ip;
    //依次扫描Ip列表中的各个IP
    foreach( ip, ipRange )
    {
        int exitCode;
    //对每个Ip执行ping命令检测其是否在线
        qDebug() << "ping " + ip << endl;
        #ifdef Q_OS_WIN
            QString strArg = "ping " + ip + " -n 1 -i 2";
            exitCode = QProcess::execute(strArg);
        #else
            //其他平台(Linux或Mac)
            exitCode = QProcess::execute("ping",  QStringList() << "-c 1" << "-t 2" << ip));
        #endif

        if(0 == exitCode)
        {
            //it's alive
            qDebug() << "shell ping " + ip + " sucessed!";
            //发射该IP在线的信号
            emit commandSuccessed(ip);
        } else {
            qDebug() << "shell ping " + ip + " failed!";
            //发射IP离线的信号
            emit commandFailed(ip);
        }
    }
}

void ShellProcess::run()
{
    //QString execArgs;
    //execArgs.clear();
    //execArgs.append(cmd);
    //execArgs.append("d:\\1.mp4");

//    QString execArgs;
//    execArgs.clear();
//    execArgs.append(cmd);
//    execArgs.append("d:\\1.mp4");

    qDebug()<<__FUNCTION__<<execArgs;
    //int exitCode = program.execute(execArgs);
    program.start(execArgs);
    program.waitForStarted();
    //if(exitCode==0)
    //{
    //    qDebug() << "exec_cmd OK";
    //}

}


void ShellProcess::exec_program(QString args)
{
    program.setProcessChannelMode(QProcess::MergedChannels);
    this->execArgs = args;

    //this->start();
}

void ShellProcess::kill_program()
{
//    QProcess p;
//    QString c = "taskkill /im "+cmd+" /f";
//    p.execute(c);
//    p.close();
    //emit program.finished();
    if(program.isOpen()){
        program.write("q");
        program.kill();
        //program.exitStatus();
    }
}
