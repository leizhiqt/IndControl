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
    uchar_8 r1;
    uchar_8 r2;
    uchar_8 r3[4];
    uchar_8 r4[4];
    uchar_8 r5[4];
    uchar_8 r6;
    uchar_8 r7;
    uchar_8 r8;
    uchar_8 r9[4];
    uchar_8 r10[4];
    uchar_8 r11[4];
    uchar_8 r12[4];
    uchar_8 r13[4];
    uchar_8 r14[4];
    uchar_8 r15[4];
    uchar_8 r16[4];
    uchar_8 r17[4];
    uchar_8 r18[4];
    uchar_8 r19[4];
    uchar_8 r20[4];
    uchar_8 r21[4];
    uchar_8 r22[4];
    uchar_8 r23[4];
    uchar_8 r24[4];
    uchar_8 r25[4];
    uchar_8 r26[4];
    uchar_8 r27[4];
    uchar_8 r28[4];
    uchar_8 r29;
 };

 struct _request_xly_t
 {
     uchar_8 r1;
     uchar_8 r2;
     uchar_8 r3[4];
     uchar_8 r4[4];
     uchar_8 r5[4];
     uchar_8 r6[4];
     uchar_8 r7[4];
     uchar_8 r8[4];
     uchar_8 r9[4];
     uchar_8 r10[4];
     uchar_8 r11[4];
     uchar_8 r12[4];
     uchar_8 r13[4];
     uchar_8 r14[4];
     uchar_8 r15;
     uchar_8 r16;
 };

 void conver_xly_frame_to_json(const char *str_json,const int len, char *frame_buf);
 void conver_xly_json_to_frame(const char *frame_buf,const int len, char *str_json);

 //标准帧
 struct _response_opencan11_t
  {
     //字节1为帧信息。第7位（FF）表示帧格式，在标准帧中，FF＝0；第6位（RTR）表示帧的类型，RTR=0表示为数据帧，RTR=1表示为远程帧；DLC表示在数据帧时实际的数据长度。

     //FF
     //r1.7==0 标准帧

     //RTR
     //r1.6==0 数据帧
     //r1.6==1 远程帧

     //r1.4~0==1 数据帧时实际的数据长度
     uchar_8 r1;

     //第2&3字节为 帧ID 有效11位 4~11==1~7为数据帧
     //r2.0->r2.7 == id.3->id.10
     uchar_8 r2;
     //r3.5->r3.7 == id.0->id.2
     uchar_8 r3;

     // ID = (r2<<3 & (r3&0xe0)>>5)

     uchar_8 r4;//b0
     uchar_8 r5;//b1
     uchar_8 r6;//b2
     uchar_8 r7;//b3
     uchar_8 r8;//b4
     uchar_8 r9;//b5
     uchar_8 r10;//b6
     uchar_8 r11;//b7
};

//扩展帧
 struct _response_opencan13_t
  {
     //字节1为帧信息。第7位（FF）表示帧格式，在标准帧中，FF＝0；第6位（RTR）表示帧的类型，RTR=0表示为数据帧，RTR=1表示为远程帧；DLC表示在数据帧时实际的数据长度。
     uchar_8 r1;

     //第2&3字节为 帧ID 有效11位 4~11==1~7为数据帧
     uchar_8 r2;
     uchar_8 r3;

     //扩展ID
     uchar_8 r4;
     uchar_8 r5;

     uchar_8 r6;//b0
     uchar_8 r7;//b1
     uchar_8 r8;//b2
     uchar_8 r9;//b3
     uchar_8 r10;//b4
     uchar_8 r11;//b5
     uchar_8 r12;//b6
     uchar_8 r13;//b7
};

  void conver_opencan_to_json(const char *frame_buf,const int len, char *str_json);

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
      }
#endif

#endif // PROTOCOLXLY_H
