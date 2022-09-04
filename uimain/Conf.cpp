#pragma execution_character_set("utf-8")

#include "Conf.h"
#include "UWLog.h"
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include "ConvertUtil.h"

/* 构造 */
Conf::Conf()
{
    //日志文件
    log_open("IndControl.txt");

    //读取配置信息
    QSettings confSeting(qApp->applicationDirPath() + QString("/config.ini"),QSettings::IniFormat);

    //位姿服务IP
    xueLangIp = confSeting.value("/XueLang/XueLangIp").toString();
    //位姿服务端口
    xueLangPort = confSeting.value("/XueLang/XueLangPort").toString().toInt();

    //数据库IP地址
    pgHostIp = confSeting.value("/Database/PgHostIp").toString();
    //数据库端口
    pgHostPort = confSeting.value("/Database/PgHostPort").toString();
    //用户名
    pgUser = confSeting.value("/Database/PgUser").toString();
    //用户密码
    pgPass = confSeting.value("/Database/PgPass").toString();
    //数据库名称
    pgDataBase = confSeting.value("/Database/PgDatabase").toString();

    //ModBus服务
    modBusPort = confSeting.value("/Modbus/ModbusPort").toString().toInt();

    //总线服务IP
    canOpenIp = confSeting.value("/CanOpen/CanOpenip").toString();
    //总线端口
    canOpenPort = confSeting.value("/CanOpen/CanOpenPort").toString().toInt();

    //WebSocket端口
    websocketPort = confSeting.value("/WebSocket/WebSocketPort").toString().toInt();

    //Modbus协同控制器IP
    modbusSlaveIp = confSeting.value("/ModbusSlave/SlaveIp").toString();
    //Modbus协同控制器端口
    modbusSlavePort = confSeting.value("/ModbusSlave/SlavePort").toString().toInt();

    //控制命令,这里加载到map直接取了就发
    QSettings confSetingControl(qApp->applicationDirPath() + QString("/ControlSetting.ini"), QSettings::IniFormat);
    controlNameList = confSetingControl.allKeys();
    char binary[128];
    for(int i = 0; i < controlNameList.size(); i++){
        QString key=controlNameList.at(i);
        key.remove(QRegExp("\\s"));
        std::string stdkey= key.toStdString();
        QByteArray value =confSetingControl.value(key).toByteArray();
        QString qstr = QString(value);
        qstr.remove(QRegExp("\\s"));
        const char *p = qstr.toLatin1().constData();
        memset(binary,'\0',sizeof(binary));
        hexs_to_binary(p,qstr.length(),(unsigned char *)binary);
        QByteArray canFrame(binary,qstr.length()/2);
        conf_can_packs.insert(std::pair<std::string,QByteArray>(stdkey,canFrame));
        //printf_hex((unsigned char*)binary,canFrame.length());
        //log_debug("key:%s",stdkey.c_str());
    }
}

/* 销毁 */
Conf::~Conf()
{
    //关闭日志输出
    log_close();
}

