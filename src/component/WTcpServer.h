#pragma execution_character_set("utf-8")

#ifndef WTCPSERVER_H
#define WTCPSERVER_H

#include<WinSock2.h>
#include<WS2tcpip.h>
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

//启动TCP服务
int tcp_server_start(server_info_t *s_info);

//发送TCP广播
int tcp_server_broadcast(server_info_t *s_info,char *buf,int len);

//接收到位姿系统发来的报文
void recvXly(char *buf,int len,SOCKET recvSocket);
//接收到操作台发来的报文
void recvModbusTcp(char *buf,int len,SOCKET recvSocket);

#endif // WTCPSERVER_H
