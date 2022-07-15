#pragma execution_character_set("utf-8")

#ifndef CONF_H
#define CONF_H

#include <QString>

#define WIN32_LEAN_AND_MEAN

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

        int modBusPort;//本机modbus端口

        QString canOpenIp;//can服务IP
        int canOpenPort;//CAN服务端口

        int websocketPort;//WEBSOCKET服务端口
public:
        ~Conf();
        static Conf * getInstance()//2.提供全局访问点
        {
            static Conf newServer;//3.c++11保证了多线程安全，程序退出时，释放资源
            return &newServer;
        }
};

#endif // CONF_H
