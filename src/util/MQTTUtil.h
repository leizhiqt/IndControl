#ifndef MQTTUTIL_H
#define MQTTUTIL_H
#include "MQTTClient.h"

void delivered(void *context, MQTTClient_deliveryToken dt);

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

void connlost(void *context, char *cause);

class MQTTUtil
{
private:
    MQTTUtil(){}//1.构造函数私有
    MQTTUtil(const MQTTUtil &);//拷贝构造函数不实现，防止拷贝产生多个实例
    MQTTUtil & operator =  (const MQTTUtil &);//复制构造函数不实现，防止赋值产生多个实例

public:
    static MQTTUtil * getInstance()//2.提供全局访问点
    {
        static MQTTUtil singleMQTTUtil;//3.c++11保证了多线程安全，程序退出时，释放资源
        return &singleMQTTUtil;
    }

//    ~MQTTUtil();
    [[
       deprecated
       (
       "sdk_mqttx for test"
       )]]
    int sdk_mqttx();

    int sdk_mqttx2(const char* address,const char* topic,const char* payload);
};

#endif // MQTTUTIL_H
