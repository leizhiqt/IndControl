#include "ProtocolXly.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "ConvertUtil.h"
#include "UWLog.h"

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

        to_hexi(hexs,p->r3,true);parms.insert("bodyxvalue",hexs);
        to_hexi(hexs,p->r4,true);parms.insert("bodyyvalue",hexs);
        to_hexi(hexs,p->r5,true);parms.insert("bodyzvalue",hexs);
        to_hexi(hexs,p->r6,true);parms.insert("bodyfyangle",hexs);
        to_hexi(hexs,p->r7,true);parms.insert("bodyhgangle",hexs);
        to_hexi(hexs,p->r8,true);parms.insert("bodyhxangle",hexs);
        to_hexi(hexs,p->r9,true);parms.insert("headxvalue",hexs);
        to_hexi(hexs,p->r10,true);parms.insert("headyvalue",hexs);
        to_hexi(hexs,p->r11,true);parms.insert("headzvalue",hexs);
        to_hexi(hexs,p->r12,true);parms.insert("headfyangle",hexs);
        to_hexi(hexs,p->r13,true);parms.insert("headhgangle",hexs);
        to_hexi(hexs,p->r14,true);parms.insert("headhxangle",hexs);
        to_hexc(hexs,p->r15);parms.insert("qzystate",hexs);

        QJsonDocument doc(parms);
        QString parms_str = QString(doc.toJson(QJsonDocument::Indented));
        response_xly_qjson.insert("parms", parms);
        QJsonDocument document;
        document.setObject(response_xly_qjson);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        char *str_p =bytes.begin();
        memcpy(str_json,str_p,bytes.size());
     }
 }


 void conver_opencan_to_json(const char *frame_buf,const int len, char *str_json)
 {
     response_opencan13_t *p;

     char hexs[512];
     if(len>=sizeof(request_xly_t)){
        p=(response_opencan13_t *)frame_buf;

        QJsonObject response_xly_qjson;
        response_xly_qjson.insert("version", "1.0");
        response_xly_qjson.insert("method", "gesturedata");

        QJsonObject parms;
        to_hexc(hexs,p->r1);parms.insert("r1",hexs);
        to_hexc(hexs,p->r2);parms.insert("r2",hexs);
        to_hexc(hexs,p->r3);parms.insert("r3",hexs);
        to_hexc(hexs,p->r4);parms.insert("r4",hexs);
        to_hexc(hexs,p->r5);parms.insert("r5",hexs);
        to_hexc(hexs,p->r6);parms.insert("r6",hexs);
        to_hexc(hexs,p->r7);parms.insert("r7",hexs);
        to_hexc(hexs,p->r8);parms.insert("r8",hexs);
        to_hexc(hexs,p->r9);parms.insert("r9",hexs);
        to_hexc(hexs,p->r10);parms.insert("r10",hexs);
        to_hexc(hexs,p->r11);parms.insert("r11",hexs);
        to_hexc(hexs,p->r12);parms.insert("r12",hexs);
        to_hexc(hexs,p->r13);parms.insert("r13",hexs);

        QJsonDocument doc(parms);
        QString parms_str = QString(doc.toJson(QJsonDocument::Indented));
        response_xly_qjson.insert("parms", parms);
        QJsonDocument document;
        document.setObject(response_xly_qjson);
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
