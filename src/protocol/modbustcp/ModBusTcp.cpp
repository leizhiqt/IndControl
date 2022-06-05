#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"
#include <thread>
#include "ControlMain.h"

int modbus_tcp_start(char* ipAddr,int port)
{
    int server_socket = -1;
	modbus_t* ctx;
	modbus_mapping_t* mb_mapping;
//	const char *ipAdd = "127.0.0.1";
    ctx = modbus_new_tcp(ipAddr, port);
	modbus_set_debug(ctx, TRUE);

	
	mb_mapping = modbus_mapping_new(500, 500, 500, 500);
	if(mb_mapping == NULL)
	{
	    fprintf(stderr, "Failed mapping:%s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}


	server_socket = modbus_tcp_listen(ctx, 1);
	if(server_socket==-1)
	{
	    fprintf(stderr, "Unable to listen TCP\n");
		modbus_free(ctx);
		return -1;
	}


	modbus_tcp_accept(ctx, &server_socket);

    uint8_t can_data[13];
    memset(can_data,'\0',sizeof(can_data));
    can_data[0]=0x43;
//    can_data[1]=0x06;
//    can_data[2]=0x06;
    can_data[3]=0x03;
    can_data[4]=0xff;

//    can_data[5]=0x11;
//    can_data[6]=0x22;
    can_data[7]=0x01;
//    can_data[8]=0x44;
//    can_data[9]=0x55;
//    can_data[10]=0x66;
//
//    11-12
	for(;;)
	{
	    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
		int rc;

        modbus_write_register(ctx,1,345);

		rc = modbus_receive(ctx, query);
		if(rc>1)
		{
		
	        modbus_reply(ctx, query, rc, mb_mapping);
            controlMain->mWin->canClient->send(can_data);
		}
		else
		{
		    
			printf("Connection Closed\n");

			modbus_close(ctx);

			modbus_tcp_accept(ctx, &server_socket);
		}
	}

	printf("Quit the loop:%s\n", modbus_strerror(errno));

	modbus_mapping_free(mb_mapping);
	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}

std::thread thd_handle0;

int modbus_tcp_thread_start(char* ipAddr,int port)
{
    // 启动 CAN 通道的接收线程
    thd_handle0 = std::thread(modbus_tcp_start,ipAddr,port);
}

void modbus_tcp_thread_wait()
{
    if (thd_handle0.joinable())
        thd_handle0.join();
}
