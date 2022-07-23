#include "ProtocolWebJson.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "ControlMain.h"
#include "ProtocolXly.h"
#include "ConvertUtil.h"
#include "UWLog.h"

/* 这是websocket 收到消息 */
void ProtocolWebJson::recvTextMessage(const QString &content)
{
    //JAVA发来的消息
    QString message = QString(content);
    message.remove(QRegExp("\\s"));

    QByteArray msgBytes = message.toLatin1().constData();
    //log_debug("%s",msgBytes.begin());
    QJsonDocument jsonDocument = QJsonDocument::fromJson(msgBytes);
    if(jsonDocument.isNull() || (!jsonDocument.isObject() && jsonDocument.isArray())){
        log_debug("Websocket解析结果：接收到的消息不正确");
        emit controlMain->webSocket->broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"消息格式不正确\"}");
        return;
    }

    //解析消息
    QJsonObject jsonObject = jsonDocument.object();
    QJsonValue cmdName = jsonObject.value("method");
    if(cmdName == NULL || cmdName.isNull() || cmdName.isUndefined()){
        log_debug("Websocket解析结果：命令不正确");
        emit controlMain->webSocket->broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"命令格式不正确\"}");
        return;
    }

    //如果消息内容是参数配置
    if(cmdName.toString() == "paramset"){
        //{"version":"1.0","method":"paramset","params":{"字段1":"值1","字段2":"值2",...,"字段N":"值N"}}
        QJsonValue paramObject = jsonObject.value("params");
        if(!paramObject.isObject()){
            log_debug("Websocket解析结果：参数格式不正确");
            emit controlMain->webSocket->broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数格式不正确\"}");
            return;
        }
        //取得各参数值
        QJsonObject valueContent = paramObject.toObject();
        if(valueContent.count() != 26){
            log_debug("Websocket解析结果：参数个数不正确，应该有26个参数");
            emit controlMain->webSocket->broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数个数不正确，应该有26个参数\"}");
            return;
        }

        response_xly_t response_xly={0};
        response_xly.r1=(uchar_8)0xAA;
        response_xly.r2=(uchar_8)0xBB;

        //初始经度
        *((float *)&(response_xly.r3)) = valueContent.value("startxvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r3));

        //初始纬度
        *((float *)&(response_xly.r4))  = valueContent.value("startyvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r4));

        //初始高度
        *((float *)&(response_xly.r5))  = valueContent.value("startzvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r5));

        //惯导标定
        *((int *)&(response_xly.r6))  = valueContent.value("stationstatus").toString().toInt();
        ntoh_32((char *)&(response_xly.r6));

        //前置视觉标定
        *((int *)&(response_xly.r7)) = valueContent.value("frontview").toString().toInt();
        ntoh_32((char *)&(response_xly.r7));

        //后置视觉标定
        *((int *)&(response_xly.r8)) = valueContent.value("rearview").toString().toInt();
        ntoh_32((char *)&(response_xly.r8));

        //激光A点X坐标
        *((float *)&(response_xly.r9)) = valueContent.value("laseraxvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r9));

        //激光A点Y坐标
        *((float *)&(response_xly.r10)) = valueContent.value("laserayvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r10));

        //激光A点Z坐标
        *((float *)&(response_xly.r11)) = valueContent.value("laserazvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r11));

        //激光B点X坐标
        *((float *)&(response_xly.r12)) = valueContent.value("laserbxvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r12));

        //激光B点Y坐标
        *((float *)&(response_xly.r13)) = valueContent.value("laserbyvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r13));

        //激光B点Z坐标
        *((float *)&(response_xly.r14)) = valueContent.value("laserbzvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r14));

        //激光C点X坐标
        *((float *)&(response_xly.r15)) = valueContent.value("lasercxvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r15));

        //激光C点Y坐标
        *((float *)&(response_xly.r16)) = valueContent.value("lasercyvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r16));

        //激光C点Z坐标
        *((float *)&(response_xly.r17)) = valueContent.value("laserczvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r17));

        //激光A1点X坐标
        *((float *)&(response_xly.r18)) = valueContent.value("lasera1xvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r18));

        //激光A1点Y坐标
        *((float *)&(response_xly.r19)) = valueContent.value("lasera1yvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r19));

        //激光A1点Z坐标
        *((float *)&(response_xly.r20)) = valueContent.value("lasera1zvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r20));

        //激光B1点X坐标
        *((float *)&(response_xly.r21)) = valueContent.value("laserb1xvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r21));

        //激光B1点Y坐标
        *((float *)&(response_xly.r22)) = valueContent.value("laserb1yvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r22));

        //激光B1点Z坐标
        *((float *)&(response_xly.r23)) = valueContent.value("laserb1zvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r23));

        //激光C1点X坐标
        *((float *)&(response_xly.r24)) = valueContent.value("laserc1xvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r5));

        //激光C1点Y坐标
        *((float *)&(response_xly.r25)) = valueContent.value("laserc1yvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r25));

        //激光C1点Z坐标
        *((float *)&(response_xly.r26)) = valueContent.value("laserc1zvalue").toString().toFloat();
        ntoh_32((char *)&(response_xly.r26));

        //截割深度
        *((float *)&(response_xly.r27)) = valueContent.value("cutdepth").toString().toFloat();
        ntoh_32((char *)&(response_xly.r27));

        //掘进速度
        *((float *)&(response_xly.r28)) = valueContent.value("diginspeed").toString().toFloat();
        ntoh_32((char *)&(response_xly.r28));

        response_xly.r29 = 0xBF;
        printf_hex((unsigned char *)&response_xly,sizeof(response_xly_t));

        //将报文下发给位姿系统客户端
        tcp_server_broadcast(controlMain->xly_srv,(char *)&response_xly,sizeof(response_xly_t));

        //给WEBSOCKET客户端推送消息
        emit controlMain->webSocket->broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数设置发送完毕\"}");
        return;
    }

     if(cmdName.toString() == "devicecontrol"){
         log_debug("devicecontrol");
        //{"version":"1.0","method":"devicecontrol","params":"字段1"}}
        //解析得到命令内容封装协议并转发给交换机
        QJsonValue cmdText = jsonObject.value("params");
        QString cmdstr = cmdText.toString();

        cmdstr = "ControlSet/"+cmdstr;

        std::string stdcmd=cmdstr.toStdString();
        log_debug("cmdText %s",stdcmd.c_str());

        std::map<std::string,QByteArray>::iterator iter = Conf::getInstance()->conf_can_packs.find(stdcmd);
        if(iter != Conf::getInstance()->conf_can_packs.end())
        {
            //获得命令内容
            log_debug("is find %s",cmdstr.toLatin1().data());
            QByteArray qbuf = iter->second;
            char *buf = qbuf.begin();
            printf_hex((unsigned char *)buf,qbuf.length());
            tcp_client_send(controlMain->can_client.acceptSocket,buf,qbuf.length());
            //发送消息给WEBSOCKET
            emit controlMain->webSocket->broadcast_msg("{\"version\":\"1.0\",\"method\":\"devicecontrol\",\"result\":\"控制命令发送完毕\"}");
        }
        else{
            log_debug("not find %s",cmdstr.toLatin1().data());
        }
        return;
    }
}
