#include "ProtocolXly.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <Qdebug>
#include "ConvertUtil.h"
#include "UWLog.h"

 void conver_response_to_json(const char *str_json,const int len, char *frame_buf)
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

 void conver_request_xly_to_frame(const char *frame_buf,const int len, char *str_json)
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
