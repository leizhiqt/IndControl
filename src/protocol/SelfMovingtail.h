#ifndef SELFMOVINGTAIL_H
#define SELFMOVINGTAIL_H

#include "UTypes.h"

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

#define read_40003 {0x00,0x04,0x00,0x03}

typedef struct _selfmoveingtail_t selfmoveingtail_t;

struct _selfmoveingtail_t
{
    uchar_8 head[8];
    uchar_8 r_len;
    uchar_8 r_40003[2];
    uchar_8 r_40004[2];
    uchar_8 r_40005[2];
    uchar_8 r_40006[2];
    uchar_8 r_40007[2];
    uchar_8 r_40008[2];
    uchar_8 r_40009[2];
    uchar_8 r_40010[2];
};

void conver_selfmovetail_to_json(const char *frame_buf,const int len, char *str_json);

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
    }
#endif

#endif // SELFMOVINGTAIL_H
