#ifndef CONF_H
#define CONF_H

//#define ENABLE_OPENCAN

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

    extern bool can_stop;
    extern unsigned int can_sleep;
    /* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
     }
#endif

#endif // CONF_H
