#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#if defined(_WIN32)
#include<WinSock2.h>
#include<WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "ModbusTcp.h"
#include <thread>
#include "ControlMain.h"
#include "UWLog.h"

#pragma comment(lib,"ws2_32.lib")

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

static void close_sigint(int dummy) {
    modbus_free(ctx);
    modbus_mapping_free(mb_mapping);
    exit(dummy);
}

int modbus_tcp_start(char* ipAddr,int port)
{
    int master_socket;
    int rc;
    fd_set refset;
    fd_set rdset;
    int fdmax;
    int header_length = 7;

    //我们作为服务器，启动后操作台自已来连
    ctx = modbus_new_tcp(ipAddr, port);

    //初始化寄存器
    mb_mapping = modbus_mapping_new(UT_BITS_ADDRESS + UT_BITS_NB,//读线圈
                                    UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,//读离散量输入
                                    UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,//读保持寄存器
                                    UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);//读输入寄存器
    if(mb_mapping == NULL)
    {
        fprintf(stderr, "初始化寄存器失败:%s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    //初始化离散量输入寄存器
    modbus_set_bits_from_bytes(mb_mapping->tab_input_bits,
                                           UT_INPUT_BITS_ADDRESS,
                                           UT_INPUT_BITS_NB,
                                           UT_INPUT_BITS_TAB);

    //初始化输入寄存器
    for (int i = 0; i < UT_INPUT_REGISTERS_NB; i++)
    {
        mb_mapping->tab_input_registers[UT_INPUT_REGISTERS_ADDRESS + i] = UT_INPUT_REGISTERS_TAB[i];
    }

    //初始化读保持寄存器
    for (int i = 0; i < UT_REGISTERS_NB; i++)
    {
        mb_mapping->tab_registers[UT_REGISTERS_ADDRESS + i] = UT_REGISTERS_TAB[i];
    }

    //开始监听
    server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);
    if (server_socket == -1)
    {
        fprintf(stderr, "无法监听TCP\n");
        modbus_free(ctx);
        return -1;
    }

    signal(SIGINT, close_sigint);

    //清除套接字的引用集
    FD_ZERO(&refset);
    //添加服务器套接字
    FD_SET(server_socket, &refset);
    fdmax = server_socket;

    //以下实现断网重连机制
    for(;;)
    {
        rdset = refset;
        if (select(fdmax + 1, &rdset, NULL, NULL, NULL) == -1)
        {
            perror("服务器 select() 失败。\n");
            close_sigint(1);
        }

        for (master_socket = 0; master_socket <= fdmax; master_socket++) {
            if (FD_ISSET(master_socket, &rdset)) {
                if (master_socket == server_socket) {
                    //一个客户端要求一个新的连接
                    socklen_t addrlen;
                    struct sockaddr_in clientaddr;
                    int newfd;

                    //处理新的连接
                    addrlen = sizeof(clientaddr);
                    memset(&clientaddr, 0, sizeof(clientaddr));
                    newfd = accept(server_socket, (struct sockaddr *)&clientaddr, &addrlen);
                    if (newfd == -1)
                    {
                        perror("服务器 accept() 出错。\n");
                    }
                    else
                    {
                        FD_SET(newfd, &refset);
                        if (newfd > fdmax)
                        {
                            //记录最大值
                            fdmax = newfd;
                        }
                        printf("新连接 %s:%d on socket %d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
                    }
                }
                else
                {
                    //已连接的主服务器发送了一个新的查询
                    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
                    int data = 0;
                    int address = 0;

                    modbus_set_socket(ctx, master_socket);
                    rc = modbus_receive(ctx, query);

                    printf_hex((unsigned char *)query,rc);

                    address = (query[header_length + 1] << 8) + query[header_length + 2];
                    if (query[header_length] == 0x06)
                    {
                        //这里可以写保持寄存器
                    }
                    if (rc != -1)
                    {
                        modbus_reply(ctx, query, rc, mb_mapping);
                    }
                    else
                    {
                        //客户端关闭了连接
                        printf("Connection closed on socket %d\n", master_socket);
                        //移除
                        FD_CLR(master_socket, &refset);
                        if (master_socket == fdmax)
                        {
                            fdmax--;
                        }
                    }
                }
            }
        }
    }

    printf("停止循环:%s\n", modbus_strerror(errno));

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}

int modbus_tcp_thread_start(char* ipAddr,int port)
{
    //modbustcp server
    if(thd_handle0.joinable()){
        return 0;
    }

    thd_handle0 = std::thread(modbus_tcp_start,ipAddr,port);
    return 0;
}

void modbus_tcp_thread_wait()
{
    if (thd_handle0.joinable())
        thd_handle0.join();
}
