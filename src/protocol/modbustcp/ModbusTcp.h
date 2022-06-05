#ifndef MODBUSTCP_H
#define MODBUSTCP_H
#include "ModbusTcpClient.h"

extern int modbus_tcp_thread_start(char* ipAddr,int port);
extern void modbus_tcp_thread_wait();

#endif // MODBUSTCP_H
