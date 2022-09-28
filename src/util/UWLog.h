#pragma execution_character_set("utf-8")

#ifndef UWLOG_H
#define UWLOG_H

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif
    void nop();
    void log_open(const char* savefp);
    void log_printfs(const char* files,unsigned int line,const char* fmt, ...);
    void log_close();

    /* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
     }
#endif

#define log_debug(fmt, ...) \
        log_printfs(__FILE__,__LINE__,fmt, ##__VA_ARGS__)

//#define log_debug(fmt, ...) nop()

#endif // UWLOG_H
