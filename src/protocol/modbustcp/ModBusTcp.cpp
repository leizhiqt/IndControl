#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <errno.h>

#include "modbus.h"
#include <thread>
#include "ControlMain.h"
#include "UWLog.h"

modbus_t* ctx;
modbus_mapping_t* mb_mapping;
int server_socket = -1;

std::thread thd_handle0;//主线程

std::thread thd_handle_receive;//接收线程
std::thread thd_handle_send;//发送线程

std::list<uint8_t *> rbuf;
uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

int modbus_tcp_receive_thread_run()
{
    for(;;)
    {
        int rc;
        rc = modbus_receive(ctx, query);
         log_debug("modbus_receive len=%d",rc);
        if(rc>1)
        {

            modbus_reply(ctx, query, rc, mb_mapping);

            rbuf.push_back(query);//尾部添加

            log_debug("modbus_receive len=%d",rc);

//            controlMain->mWin->canClient->send(can_data);
        }
        else
        {

            printf("Connection Closed\n");

            modbus_close(ctx);

            modbus_tcp_accept(ctx, &server_socket);
        }
    }
    printf("Quit the loop:%s\n", modbus_strerror(errno));
}

int modbus_tcp_send_thread_run()
{
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

        if(rbuf.size()==0){
            Sleep(300);
        }

//        modbus_write_register(ctx, 1, 500);

        uint8_t *modbus_rbuf = rbuf.front();
        uint8_t b0 = *modbus_rbuf;
        log_debug("modbus_receive b0=%40x",b0);

        if(b0==0x18a|| b0==0x18b||b0==0x18c){
            modbus_write_bits(ctx,1,13,can_data);
        }
        rbuf.pop_front();//头取出
    }
}

int modbus_tcp_start(char* ipAddr,int port)
{


//	const char *ipAdd = "127.0.0.1";
    ctx = modbus_new_tcp(ipAddr, port);

    modbus_set_slave(ctx, 0);
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

    //启动成功
    thd_handle_receive = std::thread(modbus_tcp_receive_thread_run);
    thd_handle_send = std::thread(modbus_tcp_send_thread_run);

    if (thd_handle_receive.joinable())
        thd_handle_receive.join();

    if (thd_handle_send.joinable())
        thd_handle_send.join();

    modbus_mapping_free(mb_mapping);
	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}




int test(void);

int modbus_tcp_thread_start(char* ipAddr,int port)
{
    // 启动 CAN 通道的接收线程
//    thd_handle0 = std::thread(modbus_tcp_start,ipAddr,port);
    thd_handle0 = std::thread(test);
    return 0;
}

void modbus_tcp_thread_wait()
{
    if (thd_handle0.joinable())
        thd_handle0.join();
}




int test(void)
{
    int server_socket = -1;
    modbus_t* ctx;
    modbus_mapping_t* mb_mapping;
    const char *ipAdd = "192.168.2.181";
    ctx = modbus_new_tcp(ipAdd, 502);
    modbus_set_slave(ctx, 0);
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

    for(;;)
    {
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int rc;

        modbus_write_register(ctx, 1, MODBUS_TCP_MAX_ADU_LENGTH);

        rc = modbus_receive(ctx, query);
        if(rc>1)
        {
           modbus_reply(ctx, query, rc, mb_mapping);
           //log_debug("modbus_receive b0=%40x ",b0);

           char buf[512];
           char hex[8];
           for(int i=0;i<rc;i++){
               sprintf(hex,"%40x",query[i]);
               strcat(buf,hex);
           }
           log_debug("modbus_receive %s",buf);

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
