#ifndef CONF_H
#define CONF_H

#include <QString>

//#define ENABLE_OPENCAN

//#define ENABLE_GUI

#define WIN32_LEAN_AND_MEAN

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

//    extern bool can_stop;
//    extern unsigned int can_sleep;
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
        QString hostIp;
        int port;
        QString pgHostIp;
        QString pgHostPort;
        QString pgUser;
        QString pgPass;
        QString pgDataBase;
public:
        ~Conf();
        static Conf * getInstance()//2.提供全局访问点
        {
            static Conf newServer;//3.c++11保证了多线程安全，程序退出时，释放资源
            return &newServer;
        }
};

#endif // CONF_H
