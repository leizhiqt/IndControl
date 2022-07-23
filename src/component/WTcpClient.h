﻿#pragma execution_character_set("utf-8")

#ifndef WTCPCLIENT_H
#define WTCPCLIENT_H

#define WIN32_LEAN_AND_MEAN
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>

typedef struct _client_info client_info;
typedef struct _server_info server_info_t;

typedef struct _client_info
{
  SOCKET acceptSocket;
  SOCKADDR_IN  addr;
  char ip[128];
  unsigned int port;
  int len;

  int (*recvFun)(char *buf,int len,SOCKET recvSocket);

  server_info_t *s_info;
} client_info;

int tcp_client_send(const SOCKET sSocket,const char *buf,int size);

int start_tcp_client_th(client_info *client);
int stop_tcp_client_th(SOCKET *sSocket);

int modbus_recv(char *buf,int len,SOCKET recvSocket);
int can_recv(char *buf,int len,SOCKET recvSocket);

#endif //WTCPCLIENT_H
