#ifndef CANZLG_H
#define CANZLG_H


//#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
//     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
//#endif
//    extern int open_zlgcan(char* ip0, char* port0);
//    /* 采用C编译器编译的C语言代码段 */
//#ifdef __cplusplus /* 结束使用C编译器 */
//     }
//#endif

     extern int open_zlgcan(char* ip0, char* port0);

#endif // CANZLG_H
