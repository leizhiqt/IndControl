#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include "UTypes.h"


#ifdef __cplusplus /* 如果采用了C++，如下代码使用C编译器 */
     extern  "C"  {  /* 如果没有采用C++，顺序预编译 */
#endif

     typedef struct _modbus_tcp modbus_tcp_t;

     struct _modbus_tcp
      {
         uchar_8 fr_mbap[6];//MPAP
         uchar_8 fr_cmd;
         uchar_8 fr_slave_id;
         uchar_8 r_slave[250];//最大250

         uchar_8 slave_buf[260];
         uchar_8 master_buf[50];
      };

/* 采用C编译器编译的C语言代码段 */
#ifdef __cplusplus /* 结束使用C编译器 */
           }
#endif

     extern modbus_tcp_t slave_reg;

#endif // MODBUSTCP_H
