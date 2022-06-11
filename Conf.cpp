#include "Conf.h"
#include "UWLog.h"
#include <QSettings>
#include <QApplication>

Conf::Conf()
{
    log_open("IndControl.txt");

    log_debug("Conf");
    QSettings read(qApp->applicationDirPath() + QString("/config.ini"),QSettings::IniFormat);
    hostIp = read.value("/Setting/PostureServerIp").toString();
    log_debug(hostIp.toLatin1().data());
    port = read.value("/Setting/PostureServerPort").toString().toInt();
    log_debug("%d",port);

    pgHostIp = read.value("/Database/PgHostIp").toString();
    log_debug("%s",pgHostIp.toLatin1().data());
    pgHostPort = read.value("/Database/PgHostPort").toString();
    log_debug("%s",pgHostPort.toLatin1().data());
    pgUser = read.value("/Database/PgUser").toString();
    log_debug("%s",pgUser.toLatin1().data());
    pgPass = read.value("/Database/PgPass").toString();
    log_debug("%s",pgPass.toLatin1().data());
    pgDataBase = read.value("/Database/PgDatabase").toString();
     log_debug("%s",pgDataBase.toLatin1().data());

}

Conf::~Conf()
{
    log_close();
}

