#pragma execution_character_set("utf-8")

#ifndef CONF_H
#define CONF_H

#include <QString>
#include <QStringList>
#include <map>

#define WIN32_LEAN_AND_MEAN

//#define ENABLE_GUI 1

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

    /* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
     }
#endif


class Conf
{

private:
        Conf();//1.构造函数私有
        Conf(const Conf &);//拷贝构造函数不实现，防止拷贝产生多个实例
        Conf & operator =  (const Conf &);//复制构造函数不实现，防止赋值产生多个实例
public:
        QString xueLangIp;//雪浪云服务IP
        int xueLangPort;//雪浪云服务端口

        QString pgHostIp;//PG数据库IP
        QString pgHostPort;//PG数据库端口
        QString pgUser;//PG数据库用户
        QString pgPass;//PG数据库密码
        QString pgDataBase;//PG数据库名称
        QString modBusServer;//Modbus服务器IP
        int modBusPort;//modbus端口

        QString canOpenIp;//can服务IP
        int canOpenPort;//CAN服务端口

        int websocketPort;//WEBSOCKET服务端口

        QString modbusSlaveIp;//Modbus协同控制器IP
        int modbusSlavePort;//Modbus协同控制器端口

        std::map<std::string,QByteArray> conf_can_packs;//设备控制报文

        QStringList controlNameList;//设备控制名称
        QStringList controlValueList;//设备控制报文
public:
        ~Conf();
        static Conf * getInstance()//2.提供全局访问点
        {
            static Conf newServer;//3.c++11保证了多线程安全，程序退出时，释放资源
            return &newServer;
        }
};

#endif // CONF_H
