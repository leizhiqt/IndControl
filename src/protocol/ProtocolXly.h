#ifndef PROTOCOLXLY_H
#define PROTOCOLXLY_H

#include "UTypes.h"
#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

//#define PROTOCOLXLY_H

typedef struct _response_xly_t response_xly_t;
typedef struct _request_xly_t request_xly_t;

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

 void conver_response_to_json(const char *str_json,const int len, char *frame_buf);

 void conver_request_xly_to_frame(const char *frame_buf,const int len, char *str_json);

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
      }
#endif

#endif // PROTOCOLXLY_H
