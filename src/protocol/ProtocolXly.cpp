#include "ProtocolXly.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "ConvertUtil.h"
#include "UWLog.h"
#include <QDebug>

 void conver_xly_json_to_frame(const char *str_json,const int len, char *frame_buf)
 {
//     log_debug("conver_response_to_json");

     response_xly_t *p;
//     log_debug("%d %d",len,sizeof(response_xly_t));
     char hexs[512];
     if(len>=sizeof(response_xly_t)){
        p=(response_xly_t *)frame_buf;
        /*
        //QtJson topic
        QJsonObject response_xly_qjson;
        response_xly_qjson.insert("version", "1.0");
        response_xly_qjson.insert("method", "gesturedata");

        QJsonObject parms;
        to_hexc(hexs,p->r1);parms.insert("r1",hexs);
        to_hexc(hexs,p->r2);parms.insert("r2",hexs);
        to_hexi(hexs,p->r3,true);parms.insert("r3",hexs);
        log_debug("tcp_client_send");
        to_hexi(hexs,p->r4,true);parms.insert("r4",hexs);
        to_hexi(hexs,p->r5,true);parms.insert("r5",hexs);

        to_hexc(hexs,p->r6);parms.insert("r6",hexs);
        to_hexc(hexs,p->r7);parms.insert("r7",hexs);
        to_hexc(hexs,p->r8);parms.insert("r8",hexs);

        to_hexi(hexs,p->r9,true);parms.insert("r9",hexs);
        to_hexi(hexs,p->r10,true);parms.insert("r10",hexs);
        to_hexi(hexs,p->r11,true);parms.insert("r11",hexs);
        to_hexi(hexs,p->r12,true);parms.insert("r12",hexs);
        to_hexi(hexs,p->r13,true);parms.insert("r13",hexs);
        to_hexi(hexs,p->r14,true);parms.insert("r14",hexs);
        to_hexi(hexs,p->r15,true);parms.insert("r15",hexs);
        to_hexi(hexs,p->r16,true);parms.insert("r16",hexs);
        to_hexi(hexs,p->r17,true);parms.insert("r17",hexs);
        to_hexi(hexs,p->r18,true);parms.insert("r18",hexs);
        to_hexi(hexs,p->r19,true);parms.insert("r19",hexs);
        to_hexi(hexs,p->r20,true);parms.insert("r20",hexs);
        to_hexi(hexs,p->r21,true);parms.insert("r21",hexs);
        to_hexi(hexs,p->r22,true);parms.insert("r22",hexs);
        to_hexi(hexs,p->r23,true);parms.insert("r23",hexs);
        to_hexi(hexs,p->r24,true);parms.insert("r24",hexs);
        to_hexi(hexs,p->r25,true);parms.insert("r25",hexs);
        to_hexi(hexs,p->r26,true);parms.insert("r26",hexs);
        to_hexi(hexs,p->r27,true);parms.insert("r27",hexs);
        to_hexi(hexs,p->r28,true);parms.insert("r28",hexs);
        to_hexc(hexs,p->r29);parms.insert("r29",hexs);

        QJsonDocument doc(parms);
        QString parms_str = QString(doc.toJson(QJsonDocument::Indented));
        response_xly_qjson.insert("parms", parms);
        QJsonDocument document;
        document.setObject(response_xly_qjson);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        char *str_p =bytes.begin();
        log_debug("tcp_client_send %s",str_p);
//        strcpy(str_json,str_p);
        log_debug("tcp_client_send");*/
    }
 }

 //解析位姿系统工况数据报文
 void conver_xly_frame_to_json(const char *frame_buf,const int len, char *str_json)
 {
     request_xly_t *p;
     char hexs[512];
     if(len>=sizeof(request_xly_t)){
        p=(request_xly_t *)frame_buf;

        QJsonObject response_xly_qjson;
        response_xly_qjson.insert("version", "1.0");
        response_xly_qjson.insert("method", "gesturedata");

        QJsonObject parms;

        byte4_to_int(hexs,p->r3);parms.insert("bodyxvalue",hexs);
        byte4_to_int(hexs,p->r4);parms.insert("bodyyvalue",hexs);
        byte4_to_int(hexs,p->r5);parms.insert("bodyzvalue",hexs);
        byte4_to_int(hexs,p->r6);parms.insert("bodyfyangle",hexs);
        byte4_to_int(hexs,p->r7);parms.insert("bodyhgangle",hexs);
        byte4_to_int(hexs,p->r8);parms.insert("bodyhxangle",hexs);
        byte4_to_int(hexs,p->r9);parms.insert("headxvalue",hexs);
        byte4_to_int(hexs,p->r10);parms.insert("headyvalue",hexs);
        byte4_to_int(hexs,p->r11);parms.insert("headzvalue",hexs);
        byte4_to_int(hexs,p->r12);parms.insert("headfyangle",hexs);
        byte4_to_int(hexs,p->r13);parms.insert("headhgangle",hexs);
        byte4_to_int(hexs,p->r14);parms.insert("headhxangle",hexs);
        byte1_to_char(hexs,p->r15);parms.insert("qzystate",hexs);

        QJsonDocument doc(parms);
        QString parms_str = QString(doc.toJson(QJsonDocument::Indented));
        response_xly_qjson.insert("params", parms);
        QJsonDocument document;
        document.setObject(response_xly_qjson);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        char *str_p =bytes.begin();
        memcpy(str_json,str_p,bytes.size());
     }
 }

//解析掘进机工况数据报文
 void conver_opencan_to_json(const char *frame_buf,const int len, char *str_json)
 {
    response_opencan13_t *p;
    char hexs[512];
    printf_hex((uchar_8 *)frame_buf,len);

    if(len>=sizeof(response_opencan13_t)){
        p=(response_opencan13_t *)frame_buf;

        QJsonObject send_can_son;
        send_can_son.insert("version", "1.0");

        QJsonObject parms;

        //先把COBID算出
        uint32_t cobid = hl_to_int32(p->cobid_4,p->cobid_12,p->cobid_20,p->cobid_28);
        sprintf(hexs,"%d",cobid);
        printf(hexs,"%d\n",cobid);
        if(cobid == 210){
            send_can_son.insert("method", "data210");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("系统电压",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("油泵A相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("油泵B相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data3[1],p->data3[0]));
            parms.insert("油泵C相电流",hexs);
        }
        if(cobid == 310){
            send_can_son.insert("method", "data310");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("截割A相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("截割B相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("截割C相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data3[1],p->data3[0]));
            parms.insert("二运A相电流",hexs);
        }
        if(cobid == 410){
            send_can_son.insert("method", "data410");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("二运A相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("二运B相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("风机A相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data3[1],p->data3[0]));
            parms.insert("风机B相电流",hexs);
        }
        if(cobid == 510){
            send_can_son.insert("method", "data510");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("风机C相电流",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("油泵电机温度",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("截割低速温度",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data3[1],p->data3[0]));
            parms.insert("截割高速温度",hexs);
        }
        if(cobid == 211){
            send_can_son.insert("method", "data211");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("油箱油温",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("油箱油位",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("PLC给出累加数",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data3[1],p->data3[0]));
            //取位
            parms.insert("开机预警",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),0));
            parms.insert("油泵运行",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),1));
            parms.insert("截低预警",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),2));
            parms.insert("截低待启",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),3));
            parms.insert("截低运行",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),4));
            parms.insert("截高预警",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),5));
            parms.insert("截高待启",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),6));
            parms.insert("截高运行",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),7));
            parms.insert("二运运行",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),8));
            parms.insert("风机运行",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),9));
            parms.insert("高级参数修改状态",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),10));
            parms.insert("全屏蔽状态",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),11));
            parms.insert("电压等级",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),12));
            parms.insert("遥控模式",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),14));
            parms.insert("远控模式",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),15));
        }
        if(cobid == 311){
            send_can_son.insert("method", "data311");
            //故障代码
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            //取位
            parms.insert("油泵故障",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),0));
            parms.insert("油泵短路",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),1));
            parms.insert("油泵缺相",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),2));
            parms.insert("油泵过载",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),3));
            parms.insert("油泵启动失败",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),4));
            parms.insert("油泵粘连",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),5));
            parms.insert("油泵超温",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),6));
            parms.insert("油泵漏电",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),7));
            parms.insert("截低故障",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),8));
            parms.insert("截低短路",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),9));
            parms.insert("截低缺相",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),10));
            parms.insert("截低过载",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),11));
            parms.insert("截低启动失败",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),12));
            parms.insert("截低粘连",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),13));
            parms.insert("截低超温",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),14));
            parms.insert("截低漏电",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),15));

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("截高故障",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),0));
            parms.insert("截高短路",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),1));
            parms.insert("截高缺相",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),2));
            parms.insert("截高过载",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),3));
            parms.insert("截高启动失败",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),4));
            parms.insert("截高粘连",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),5));
            parms.insert("截高超温",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),6));
            parms.insert("截高漏电",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),7));
            parms.insert("二运故障",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),8));
            parms.insert("二运短路",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),9));
            parms.insert("二运缺相",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),10));
            parms.insert("二运过载",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),11));
            parms.insert("二运启动失败",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),12));
            parms.insert("二运粘连",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),13));
            parms.insert("二运漏电",GET_BIT(hl_to_int16(p->data1[1],p->data1[0]),15));

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("风机故障",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),0));
            parms.insert("风机短路",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),1));
            parms.insert("风机缺相",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),2));
            parms.insert("风机过载",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),3));
            parms.insert("风机启动失败",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),4));
            parms.insert("风机粘连",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),5));
            parms.insert("未使用",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),6));
            parms.insert("风机漏电",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),7));
            parms.insert("油温过高",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),8));
            parms.insert("油位过低",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),9));
            parms.insert("低压故障",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),14));
            parms.insert("电机漏电",GET_BIT(hl_to_int16(p->data2[1],p->data2[0]),15));

            sprintf(hexs,"%d",hl_to_int16(p->data3[1],p->data3[0]));
            parms.insert("电压异常",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),0));
            parms.insert("总急停",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),1));
            parms.insert("截割急停",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),2));
            parms.insert("瓦斯闭锁",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),3));
            parms.insert("门未关闭",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),4));
            parms.insert("24V漏电",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),5));
            parms.insert("220V漏电",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),6));
            parms.insert("风电闭锁",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),7));
            parms.insert("流量闭锁",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),8));
            parms.insert("遥控模式失败",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),9));
            parms.insert("远控模式失败",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),10));
            parms.insert("遥控急停",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),11));
            parms.insert("远控急停",GET_BIT(hl_to_int16(p->data3[1],p->data3[0]),12));
        }
        if(cobid == 287){
            send_can_son.insert("method", "data287");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("AI1（瓦斯）",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data1[1],p->data1[0]));
            parms.insert("AI2（风速）",hexs);

            sprintf(hexs,"%d",hl_to_int16(p->data2[1],p->data2[0]));
            parms.insert("AI3（粉尘）",hexs);
        }
        if(cobid == 487){
            send_can_son.insert("method", "data487");
            sprintf(hexs,"%d",hl_to_int16(p->data0[1],p->data0[0]));
            parms.insert("危险区1有人",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),0));
            parms.insert("危险区2有人",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),1));
            parms.insert("危险区3有人",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),2));
            parms.insert("危险区4有人",GET_BIT(hl_to_int16(p->data0[1],p->data0[0]),3));
        }

        QJsonDocument doc(parms);
        QString parms_str = QString(doc.toJson(QJsonDocument::Indented));
        send_can_son.insert("params", parms);

        QJsonDocument document;
        document.setObject(send_can_son);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        char *str_p =bytes.begin();
        memcpy(str_json,str_p,bytes.size());
     }
 }

 void conver_opencan_frame(uchar_8 *frame,uint16_t len)
 {
     //字节1为帧信息。第7位（FF）表示帧格式，在标准帧中，FF＝0；第6位（RTR）表示帧的类型，RTR=0表示为数据帧，RTR=1表示为远程帧；DLC表示在数据帧时实际的数据长度。
        if((*(frame)<9 && *(frame)>0))
        {
             return;
        }
        //第2&3字节为 帧ID 有效11位 4~11==1~7为数据帧
         if(*(frame+1)==0x01 && *(frame+2)==0x8a){
             return;
         }
         if(*(frame+1)==0x01 && *(frame+2)==0x8b){
             return;
         }
         if(*(frame+1)==0x01 && *(frame+2)==0x8c){
             return;
         }
         if(*(frame+1)==0x05 && *(frame+2)==0x01){
             return;
         }
         if(*(frame+1)==0x05 && *(frame+2)==0x02){
             return;
         }
         if(*(frame+1)==0x05 && (*(frame+2)==0x21 || *(frame+2)==0x22 || *(frame+2)==0x23
                                  || *(frame+2)==0x24 || *(frame+2)==0x25 || *(frame+2)==0x26
                                  || *(frame+2)==0x27)){
             return;
         }
         if(*(frame+1)==0x02 && *(frame+2)==0x10){
             return;
         }
         if(*(frame+1)==0x03 && *(frame+2)==0x10){
             return;
         }
         if(*(frame+1)==0x04 && *(frame+2)==0x10){
             return;
         }
         if(*(frame+1)==0x05 && *(frame+2)==0x10){
             return;
         }
         if(*(frame+1)==0x02 && *(frame+2)==0x11){
             return;
         }
         if(*(frame+1)==0x03 && *(frame+2)==0x11){
             return;
         }
         if(*(frame+1)==0x02 && *(frame+2)==0x87){
             return;
         }
         if(*(frame+1)==0x03 && *(frame+2)==0x87){
             return;
         }
         if(*(frame+1)==0x04 && *(frame+2)==0x87){
             return;
         }
         if(*(frame+1)==0x04 && *(frame+2)==0x0e){
             return;
         }
         if(*(frame+1)==0x02 && *(frame+2)==0x90){
             return;
         }
 }
