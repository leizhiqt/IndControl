#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include "modbus.h"
#include "modbus-tcp.h"
#include "modbus-version.h"

extern int modbus_tcp_thread_start(char* ipAddr,int port);
extern void modbus_tcp_thread_wait();

#define NB_CONNECTION    5

//线圈地址数据的定义
const uint16_t UT_BITS_ADDRESS = 0x13;
const uint16_t UT_BITS_NB = 0x25;
const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
//离散量输入寄存器地址数据的定义
const uint16_t UT_INPUT_BITS_ADDRESS = 0xC4;
const uint16_t UT_INPUT_BITS_NB = 0x16;
const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };
//读保持寄存器地址数据的定义
const uint16_t UT_REGISTERS_ADDRESS = 0x6B;
//当此地址用于第一个字节时引发手动异常
const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x6C;
const uint16_t UT_REGISTERS_NB = 0x3;
const uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };
const uint16_t UT_REGISTERS_NB_SPECIAL = 0x2;
//输入寄存器地址数据定义
const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x08;
const uint16_t UT_INPUT_REGISTERS_NB = 0x1;
const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };

#endif // MODBUSTCP_H
