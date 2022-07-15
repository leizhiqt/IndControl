#pragma execution_character_set("utf-8")

#ifndef WTCPCLIENT_H
#define WTCPCLIENT_H

#define WIN32_LEAN_AND_MEAN
#include<WinSock2.h>
#include<WS2tcpip.h>

int tcp_client_send(const SOCKET *sSocket,const char *buf,size_t size);

int start_tcp_client_th(char * host,int port,SOCKET *sSocket);

int stop_tcp_client_th();

#endif //WTCPCLIENT_H
