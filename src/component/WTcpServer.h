﻿#pragma execution_character_set("utf-8")

#ifndef WTCPSERVER_H
#define WTCPSERVER_H

#define WIN32_LEAN_AND_MEAN
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>
#include<vector>

typedef struct _client_info client_info;
typedef struct _server_info server_info_t;

typedef struct _server_info
{
  SOCKET ListenSocket;
  void (*recvFun)(char *buf,int len,SOCKET recvSocket);
  int port;
  int s_forever;

  std::vector<SOCKET> *cliens_p;

} server_info_t;

int tcp_server_start(server_info_t *s_info);
int tcp_server_broadcast(server_info_t *s_info,char *buf,int len);

//位姿系统接收
void recvXly(char *buf,int len,SOCKET recvSocket);
//modbus接收
void recvModbusTcp(char *buf,int len,SOCKET recvSocket);

#endif // WTCPSERVER_H
