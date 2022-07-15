#pragma execution_character_set("utf-8")

#ifndef WTCPSERVER_H
#define WTCPSERVER_H

#define WIN32_LEAN_AND_MEAN
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>

typedef struct _server_info
{
//  std::thread thread_handle_tcpserver;//主线程
  SOCKET ListenSocket;
  void (*recvFun)(char *buf,int len);

  int port;
  int s_forever;
} server_info_t;

typedef struct _client_info
{
  SOCKET acceptSocket;
  SOCKADDR_IN  addr;
  char ip[128];
  unsigned int port;
  int len;

  server_info_t *s_info;
} client_info;

int tcp_server_start(server_info_t *s_info);

void recvXly(char *buf,int len);
void recvModbusTcp(char *buf,int len);

#endif // WTCPSERVER_H
