#pragma execution_character_set("utf-8")

#include "NetToolHelper.h"
#include <QtDebug>
#include <QtNetwork/QtNetwork>

QString NetToolHelper::get_locale_ipv4(QString serverIP)
{
    //QStringList list_ip;
    //QString localIp;

//    qDebug()<<"IP: " << ip;

    quint32 raddr = QHostAddress(serverIP).toIPv4Address();

    raddr=(raddr >> 8);

 //   qDebug()<<"IP: " << QHostAddress(raddr).toString();

    QString v4rIp;
    QString v4Ip;
    //QList<QHostAddress> QNetworkInterface::allInterfaces();
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    int nListSize = ipAddressesList.size();

    for (int i = 0; i < nListSize; ++i){
        //list_ip << ipAddressesList.at(i).toString();
        v4Ip = ipAddressesList.at(i).toString();
        quint32 laddr =ipAddressesList.at(i).toIPv4Address();
        laddr=(laddr >> 8);

        int exitCode;
        QString strArg = "ping " + serverIP + " -n 1 -i 2";
        exitCode = QProcess::execute(strArg);

        if(exitCode==0 && raddr==laddr){
            qDebug()<<"Local IP: " << QHostAddress(laddr).toString();
            //v4Ip=QHostAddress(ipAddressesList.at(i).toIPv4Address()).toString();
           return v4Ip;
        }
    }
    return nullptr;
}
