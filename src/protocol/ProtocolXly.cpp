#include "ProtocolXly.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "ConvertUtil.h"
#include "UWLog.h"
#include <QDebug>

 //解析位姿系统工况数据报文
 //2022-10-15修改报文协议，增加字段
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
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r3));parms.insert("X坐标测量值(机身)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r4));parms.insert("Y坐标测量值(机身)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r5));parms.insert("Z坐标测量值(机身)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r6));parms.insert("俯仰角测量值(机身)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r7));parms.insert("横滚角测量值(机身)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r8));parms.insert("航向角测量值(机身)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r9));parms.insert("X坐标测量值(截割头)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r10));parms.insert("Y坐标测量值(截割头)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r11));parms.insert("Z坐标测量值(截割头)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r12));parms.insert("回转测量值(截割头)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r13));parms.insert("伸缩测量值(截割头)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r14));parms.insert("抬升测量值(截割头)",hexs);
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r15));parms.insert("后置相机连接状态",hexs);//2022-10-15新增
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r16));parms.insert("雷达连接状态",hexs);//2022-10-15新增
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r17));parms.insert("惯导连接状态",hexs);//2022-10-15新增
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r18));parms.insert("倾角传感器连接状态",hexs);//2022-10-15新增
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r19));parms.insert("油缸行程传感器连接状态",hexs);//2022-10-15新增
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r20));parms.insert("截割完成",hexs);//2022-10-15新增
        sprintf(hexs,"%0.2f",byte4_to_float32(p->r21));parms.insert("截割暂停",hexs);//2022-10-15新增

        response_xly_qjson.insert("params", parms);
        QJsonDocument document;
        document.setObject(response_xly_qjson);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        char *str_p =bytes.begin();
        memcpy(str_json,str_p,bytes.size());
     }
 }

 //解析掘进机工况数据报文
 //原函数是void，现改成返回int
 //如果不是08开头的报文或者说COBID未定义，则返回0
 //如果返回0，则不向JAVA推送消息
 //避免JAVA收到{"param":{},"version":"1.0"}这种无用消息
 int conver_opencan_to_json(const char *frame_buf,const int len, char *str_json)
 {
    response_opencan13_t *p;
    char hexs[512];
    //接收到的报文
    printf_hex((uchar_8 *)frame_buf,len);

    if(len>=sizeof(response_opencan13_t)){
        p=(response_opencan13_t *)frame_buf;
        QJsonObject send_can_son;
        send_can_son.insert("version", "1.0");
        QJsonObject parms;

        //只处理08开头的报文，其他不处理
        if(p->r1 == 0x08){
            uint32_t cobid = hl_to_int32(p->cobid_4,p->cobid_12,p->cobid_20,p->cobid_28);
            log_debug("cobid 数据 十进制=%d 16进制=%04x",cobid,cobid);

            if(cobid == 0x0210){
                send_can_son.insert("method", "data210");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("系统电压",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data1[0],p->data1[1]));
                parms.insert("油泵A相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data2[0],p->data2[1]));
                parms.insert("油泵B相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data3[0],p->data3[1]));
                parms.insert("油泵C相电流",hexs);
            }
            else if(cobid == 0x0310){
                send_can_son.insert("method", "data310");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("截割A相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data1[0],p->data1[1]));
                parms.insert("截割B相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data2[0],p->data2[1]));
                parms.insert("截割C相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data3[0],p->data3[1]));
                parms.insert("二运A相电流",hexs);
            }
            else if(cobid == 0x0410){
                send_can_son.insert("method", "data410");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("二运B相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data1[0],p->data1[1]));
                parms.insert("二运C相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data2[0],p->data2[1]));
                parms.insert("风机A相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data3[0],p->data3[1]));
                parms.insert("风机B相电流",hexs);
            }
            else if(cobid == 0x0510){
                send_can_son.insert("method", "data510");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("风机C相电流",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data1[0],p->data1[1]));
                parms.insert("油泵电机温度",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data2[0],p->data2[1]));
                parms.insert("截割低速温度",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data3[0],p->data3[1]));
                parms.insert("截割高速温度",hexs);
            }
            else if(cobid == 0x0211){
                send_can_son.insert("method", "data211");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("油箱油温",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data1[0],p->data1[1]));
                parms.insert("油箱油位",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data2[0],p->data2[1]));
                parms.insert("PLC给出累加数",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data3[0],p->data3[1]));
                //取位
                parms.insert("开机预警",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),0));
                parms.insert("油泵运行",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),1));
                parms.insert("截低预警",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),2));
                parms.insert("截低待启",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),3));
                parms.insert("截低运行",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),4));
                parms.insert("截高预警",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),5));
                parms.insert("截高待启",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),6));
                parms.insert("截高运行",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),7));
                parms.insert("二运运行",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),8));
                parms.insert("风机运行",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),9));
                parms.insert("高级参数修改状态",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),10));
                parms.insert("全屏蔽状态",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),11));
                parms.insert("电压等级",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),12));
                parms.insert("遥控模式",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),14));
                parms.insert("远控模式",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),15));
            }
            else if(cobid == 0x0311){
                send_can_son.insert("method", "data311");
                //故障代码
                parms.insert("油泵故障",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),0));
                parms.insert("油泵短路",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),1));
                parms.insert("油泵缺相",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),2));
                parms.insert("油泵过载",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),3));
                parms.insert("油泵启动失败",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),4));
                parms.insert("油泵粘连",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),5));
                parms.insert("油泵超温",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),6));
                parms.insert("油泵漏电",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),7));
                parms.insert("截低故障",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),8));
                parms.insert("截低短路",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),9));
                parms.insert("截低缺相",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),10));
                parms.insert("截低过载",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),11));
                parms.insert("截低启动失败",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),12));
                parms.insert("截低粘连",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),13));
                parms.insert("截低超温",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),14));
                parms.insert("截低漏电",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),15));

                parms.insert("截高故障",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),0));
                parms.insert("截高短路",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),1));
                parms.insert("截高缺相",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),2));
                parms.insert("截高过载",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),3));
                parms.insert("截高启动失败",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),4));
                parms.insert("截高粘连",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),5));
                parms.insert("截高超温",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),6));
                parms.insert("截高漏电",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),7));
                parms.insert("二运故障",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),8));
                parms.insert("二运短路",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),9));
                parms.insert("二运缺相",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),10));
                parms.insert("二运过载",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),11));
                parms.insert("二运启动失败",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),12));
                parms.insert("二运粘连",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),13));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),14));
                parms.insert("二运漏电",GET_BIT(hl_to_int16(p->data1[0],p->data1[1]),15));

                parms.insert("风机故障",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),0));
                parms.insert("风机短路",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),1));
                parms.insert("风机缺相",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),2));
                parms.insert("风机过载",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),3));
                parms.insert("风机启动失败",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),4));
                parms.insert("风机粘连",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),5));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),6));
                parms.insert("风机漏电",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),7));
                parms.insert("油温过高",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),8));
                parms.insert("油位过低",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),9));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),10));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),11));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),12));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),13));
                parms.insert("低压故障",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),14));
                parms.insert("电机漏电",GET_BIT(hl_to_int16(p->data2[0],p->data2[1]),15));

                parms.insert("电压异常",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),0));
                parms.insert("总急停",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),1));
                parms.insert("截割急停",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),2));
                parms.insert("瓦斯闭锁",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),3));
                parms.insert("门未关闭",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),4));
                parms.insert("24V漏电",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),5));
                parms.insert("220V漏电",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),6));
                parms.insert("风电闭锁",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),7));
                parms.insert("流量闭锁",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),8));
                parms.insert("遥控模式失败",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),9));
                parms.insert("远控模式失败",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),10));
                parms.insert("遥控急停",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),11));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),12));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),13));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),14));
                parms.insert("未使用",GET_BIT(hl_to_int16(p->data3[0],p->data3[1]),15));
            }
            else if(cobid == 0x0287){
                send_can_son.insert("method", "data287");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("AI1（瓦斯）",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data1[0],p->data1[1]));
                parms.insert("AI2（风速）",hexs);

                sprintf(hexs,"%d",hl_to_int16(p->data2[0],p->data2[1]));
                parms.insert("AI3（粉尘）",hexs);
            }
            else if(cobid == 0x0487){
                send_can_son.insert("method", "data487");
                sprintf(hexs,"%d",hl_to_int16(p->data0[0],p->data0[1]));
                parms.insert("危险区1有人",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),0));
                parms.insert("危险区2有人",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),1));
                parms.insert("危险区3有人",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),2));
                parms.insert("危险区4有人",GET_BIT(hl_to_int16(p->data0[0],p->data0[1]),3));
            }
            else{
                log_debug("cobid不存在");
                return 0;
            }
        }else{
            log_debug("非08开头的报文,不需要解析");
            return 0;
        }
        send_can_son.insert("params", parms);
        QJsonDocument document;
        document.setObject(send_can_son);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        char *str_p =bytes.begin();
        memcpy(str_json,str_p,bytes.size());
        return 1;
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
