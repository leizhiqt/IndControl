#ifndef CONVERTUTIL_H
#define CONVERTUTIL_H

#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

int hexs_to_binary(char *in,int len, unsigned char *out);

int binary_to_hexs(unsigned char *in, int len, char *out);

void utf8ToGbk(char *utf8String, char *gbkString);

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
 }
#endif
#endif // CONVERTUTIL_H
