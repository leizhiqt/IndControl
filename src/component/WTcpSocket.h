#ifndef WTCPSOCKET_H
#define WTCPSOCKET_H

int tcp_client_send(unsigned char const *buf,size_t size);

int start_tcp_client_th(char * host,int port);

#endif // CANZLG_H
