﻿#pragma execution_character_set("utf-8")

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
    QSettings read(qApp->applicationDirPath() + QString("/config.ini"),QSettings::IniFormat);

    //位姿服务IP
    xueLangIp = read.value("/XueLang/XueLangIp").toString();
    //位姿服务端口
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

    //ModBus服务
    modBusPort = read.value("/Modbus/ModbusPort").toString().toInt();

    //总线服务IP
    canOpenIp = read.value("/CanOpen/CanOpenip").toString();
    //总线端口
    canOpenPort = read.value("/CanOpen/CanOpenPort").toString().toInt();

    //WebSocket端口
    websocketPort = read.value("/WebSocket/WebSocketPort").toString().toInt();

    //控制命令
    //这里用map 加载到map 直接取了就发
    QSettings readControl(qApp->applicationDirPath() + QString("/ControlSetting.ini"), QSettings::IniFormat);
    controlNameList = readControl.allKeys();
    char binary[128];
    for(int i = 0; i < controlNameList.size(); i++){
          QString key=controlNameList.at(i);
          std::string stdkey= key.toStdString();

          QByteArray value =readControl.value(key).toByteArray();
          const char *p = value.constData();
          memset(binary,'\0',sizeof(binary));
          hexs_to_binary(p,value.length(),(unsigned char *)binary);
          QByteArray canFrame(binary,value.length()/2);
          conf_can_packs.insert(std::pair<std::string,QByteArray>(stdkey,canFrame));

          printf_hex((unsigned char*)binary,canFrame.length());
            log_debug("key:%s",stdkey.c_str());

        //controlValueList.append(readControl.value(controlNameList.at(i)).toString());
    }
//    qDebug()<<"controlNameList:"<<controlNameList;
//    qDebug()<<"controlValueList:"<<controlValueList;
}

/* 销毁 */
Conf::~Conf()
{
    //关闭日志输出
    log_close();
}

