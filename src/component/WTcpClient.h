#ifndef WTCPCLIENT_H
#define WTCPCLIENT_H

int tcp_client_send(unsigned char const *buf,size_t size);

int start_tcp_client_th(char * host,int port);

int stop_tcp_client_th();

#endif // CANZLG_H
