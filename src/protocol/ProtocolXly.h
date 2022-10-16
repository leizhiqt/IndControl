#ifndef PROTOCOLXLY_H
#define PROTOCOLXLY_H

#include "UTypes.h"
#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

typedef struct _response_xly_t response_xly_t;
typedef struct _request_xly_t request_xly_t;

typedef struct _response_opencan11_t response_opencan11_t;
typedef struct _response_opencan13_t response_opencan13_t;

typedef struct _request_opencan_t request_opencan_t;

struct _response_xly_t
 {
    uchar_8 r1;     //通讯头1
    uchar_8 r2;     //通讯头2
    uchar_8 r3[4];  //初始经度
    uchar_8 r4[4];  //初始纬度
    uchar_8 r5[4];  //初始高度
    uchar_8 r6;     //惯导标定,触发指令：0x01:触发；0x00：未触发
    uchar_8 r7;     //回转油缸标定,触发指令：0x01:触发；0x00：未触发
    uchar_8 r8;     //后置视觉标定,触发指令：0x01:触发；0x00：未触发
    uchar_8 r9[4];  //激光A点X坐标
    uchar_8 r10[4]; //激光A点Y坐标
    uchar_8 r11[4]; //激光A点Z坐标
    uchar_8 r12[4]; //激光B点X坐标
    uchar_8 r13[4]; //激光B点Y坐标
    uchar_8 r14[4]; //激光B点Z坐标
    uchar_8 r15[4]; //激光C点X坐标
    uchar_8 r16[4]; //激光C点Y坐标
    uchar_8 r17[4]; //激光C点Z坐标
    uchar_8 r18[4]; //激光A1点X坐标
    uchar_8 r19[4]; //激光A1点Y坐标
    uchar_8 r20[4]; //激光A1点Z坐标
    uchar_8 r21[4]; //激光B1点X坐标
    uchar_8 r22[4]; //激光B1点Y坐标
    uchar_8 r23[4]; //激光B1点Z坐标
    uchar_8 r24[4]; //激光C1点X坐标
    uchar_8 r25[4]; //激光C1点Y坐标
    uchar_8 r26[4]; //激光C1点Z坐标
    uchar_8 r27[4]; //截割深度
    uchar_8 r28[4]; //掘进速度
    uchar_8 r29[4]; //截割轨迹编号
    uchar_8 r30;    //结束符(2022-10-15新增）
 };

 struct _request_xly_t
 {
     uchar_8 r1;        //通讯头1
     uchar_8 r2;        //通讯头2
     uchar_8 r3[4];     //X坐标测量值（机身)
     uchar_8 r4[4];     //Y坐标测量值（机身)
     uchar_8 r5[4];     //Z坐标测量值（机身)
     uchar_8 r6[4];     //俯仰角测量值（机身)
     uchar_8 r7[4];     //横滚角测量值（机身)
     uchar_8 r8[4];     //航向角测量值（机身)
     uchar_8 r9[4];     //X坐标测量值（截割头)
     uchar_8 r10[4];    //Y坐标测量值（截割头)
     uchar_8 r11[4];    //Z坐标测量值（截割头)
     uchar_8 r12[4];    //回转测量值（截割头)
     uchar_8 r13[4];    //伸缩测量值（截割头)
     uchar_8 r14[4];    //抬升测量值（截割头)
     uchar_8 r15[4];    //后置相机连接状态
     uchar_8 r16[4];    //雷达连接状态
     uchar_8 r17[4];    //惯导连接状态
     uchar_8 r18[4];    //倾角传感器连接状态
     uchar_8 r19[4];    //油缸行程传感器连接状态
     uchar_8 r20[4];    //截割完成
     uchar_8 r21[4];    //截割暂停
     uchar_8 r22;       //结束符
 };

 void conver_xly_frame_to_json(const char *str_json,const int len, char *frame_buf);
 int conver_opencan_to_json(const char *frame_buf,const int len, char *str_json);
 //void conver_xly_json_to_frame(const char *frame_buf,const int len, char *str_json);

 //标准帧
 struct _response_opencan11_t
  {
     //字节1为帧信息,第7位（FF）表示帧格式，在标准帧中，FF＝0；第6位（RTR）表示帧的类型，RTR=0表示为数据帧，RTR=1表示为远程帧；DLC表示在数据帧时实际的数据长度。
     //FF
     //r1.7==0 标准帧
     //RTR
     //r1.6==0 数据帧
     //r1.6==1 远程帧
     //r1.4~0==1 数据帧时实际的数据长度
     uchar_8 r1;
     //第2&3字节为帧ID有效11位4~11==1~7为数据帧
     //r2.0->r2.7 == id.3->id.10
     uchar_8 cobid_h;
     //r3.5->r3.7 == id.0->id.2
     uchar_8 cobid_l;
     //ID = (r2<<3 & (r3&0xe0)>>5)
     uchar_8 data0[2];
     uchar_8 data1[2];
     uchar_8 data2[2];
     uchar_8 data3[2];
};

//13个字节帧
struct _response_opencan13_t
  {
     //08 00 00 00 D2 05 DC 0D AC 11 94 15 7C
     uchar_8 r1;//数据长度（1个字节）
     uchar_8 cobid_28;//数据1
     uchar_8 cobid_20;//帧格式（1个字节）
     uchar_8 cobid_12;//帧类型（1个字节）
     uchar_8 cobid_4;
     uchar_8 data0[2];//数据1
     uchar_8 data1[2];//数据2
     uchar_8 data2[2];//数据3
     uchar_8 data3[2];//数据4
};

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
      }
#endif

#endif // PROTOCOLXLY_H
