#pragma execution_character_set("utf-8")

#include "Conf.h"
#include "UWLog.h"
#include <QSettings>
#include <QApplication>

/* 构造 */
Conf::Conf()
{
    //日志文件
    log_open("IndControl.txt");

    //读取配置信息
    QSettings read(qApp->applicationDirPath() + QString("/config.ini"),QSettings::IniFormat);

    //雪浪云服务IP
    xueLangIp = read.value("/XueLang/XueLangIp").toString();
    //雪浪云服务端口
    xueLangPort = read.value("/XueLang/XueLangPort").toString().toInt();

    //数据库IP地址
    pgHostIp = read.value("/Database/PgHostIp").toString();
    //数据库端口
    pgHostPort = read.value("/Database/PgHostPort").toString();
    //用户名
    pgUser = read.value("/Database/PgUser").toString();
    //用户密码
    pgPass = read.value("/Database/PgPass").toString();
    //数据库名称
    pgDataBase = read.value("/Database/PgDatabase").toString();

    //本机ModBus服务
    modBusPort = read.value("/Modbus/ModbusPort").toString().toInt();

    //CanOpen服务IP
    canOpenIp = read.value("/CanOpen/CanOpenip").toString();
    //CanOpen端口
    canOpenPort = read.value("/CanOpen/CanOpenPort").toString().toInt();

    //WebSocket端口
    websocketPort = read.value("/WebSocket/WebSocketPort").toString().toInt();
}

/* 销毁 */
Conf::~Conf()
{
    //关闭日志输出
    log_close();
}

