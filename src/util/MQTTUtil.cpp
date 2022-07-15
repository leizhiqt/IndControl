#pragma execution_character_set("utf-8")

#include "MQTTUtil.h"
#include <QDebug>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define ADDRESS     "tcp://127.0.0.9:1883"
#define CLIENTID    "ClientSDK"
#define TOPIC       "Camera/1.1/DIR"
#define PAYLOAD     "{\"id\":\"设备表主键id\",\"code\":\"1000001$1$0$0\",\"type\":\"002002\",\"dir\":\"top\",\"cmd\":\"stop\",\"step\":1}"

#define QOS         1
#define TIMEOUT     10000L

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    qDebug()<<__FILE__<<__LINE__;
    //printf("Message with token value %d delivery confirmed\n", dt);
    //deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    qDebug()<<__FILE__<<__LINE__;

    int i;
    char* payloadptr;

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    /*payloadptr = message->payload;
    if(strcmp(payloadptr, DISCONNECT) == 0){
        printf(" \n out!!");
        CONNECT = 0;
    }*/

    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    printf("\n");

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    qDebug()<<__FUNCTION__;
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int MQTTUtil::sdk_mqttx()
{
    qDebug()<<__FUNCTION__;

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int rc;
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc);
         //exit(EXIT_FAILURE);
         qDebug()<<"Failed to create client, return code "<< rc;
         return -1;
    }
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username="admin";
    conn_opts.password="admin";
    //conn_opts.connectTimeout=20;
    //conn_opts.retryInterval=20;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        qDebug()<<"Failed to connect, return code "<< rc;
        return -2;
    }

    char pload[] ="{\"id\":\"设备表主键id\",\"code\":\"1000001$1$0$0\",\"type\":\"002002\",\"dir\":\"top\",\"cmd\":\"stop\",\"step\":1}";

    pubmsg.payload = pload;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to publish message, return code %d\n", rc);
         qDebug()<<"Failed to connect, return code "<< rc;
         //exit(EXIT_FAILURE);
         return -3;
    }

    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    qDebug()<<"Message with delivery token  delivered "<< token;

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to disconnect, return code %d\n", rc);
        qDebug()<<"Failed to disconnect, return cod "<< rc;
    }

    MQTTClient_destroy(&client);

    return 0;
}

//#define ADDRESS     "tcp://127.0.0.9:1883"
//#define CLIENTID    "ClientSDK"
//#define TOPIC       "Camera/1.1/DIR"
//#define PAYLOAD     "{\"id\":\"设备表主键id\",\"code\":\"1000001$1$0$0\",\"type\":\"002002\",\"dir\":\"top\",\"cmd\":\"stop\",\"step\":1}"
//char pload[] ="{\"id\":\"设备表主键id\",\"code\":\"1000001$1$0$0\",\"type\":\"002002\",\"dir\":\"top\",\"cmd\":\"stop\",\"step\":1}";

int MQTTUtil::sdk_mqttx2(const char *address,const char *topic,const char *payload)
{
    qDebug()<<__FUNCTION__;

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int rc;
    if ((rc = MQTTClient_create(&client, address, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc);
         //exit(EXIT_FAILURE);
         qDebug()<<"Failed to create client, return code "<< rc;
         return -1;
    }
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username="admin";
    conn_opts.password="admin";
    //conn_opts.connectTimeout=20;
    //conn_opts.retryInterval=20;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        qDebug()<<"Failed to connect, return code "<< rc;
        return -2;
    }

//    char pload[] ="{\"id\":\"设备表主键id\",\"code\":\"1000001$1$0$0\",\"type\":\"002002\",\"dir\":\"top\",\"cmd\":\"stop\",\"step\":1}";

    pubmsg.payload = (void *)payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    if ((rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to publish message, return code %d\n", rc);
         qDebug()<<"Failed to connect, return code "<< rc;
         //exit(EXIT_FAILURE);
         return -3;
    }

    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    qDebug()<<"Message with delivery token  delivered "<< token;

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to disconnect, return code %d\n", rc);
        qDebug()<<"Failed to disconnect, return cod "<< rc;
    }

    MQTTClient_destroy(&client);

    return 0;
}
