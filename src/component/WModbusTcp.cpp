#include "WModbusTcp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include "modbus.h"
#include "modbus-tcp.h"
#include "modbus-version.h"
#include "UWLog.h"
#include "ControlMain.h"

#include <thread>
#include <unistd.h>

#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#define NB_CONNECTION    5

//线圈地址数据的定义
const uint16_t UT_BITS_ADDRESS = 0x13;
const uint16_t UT_BITS_NB = 0x25;
const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
//离散量输入寄存器地址数据的定义
const uint16_t UT_INPUT_BITS_ADDRESS = 0xC4;
const uint16_t UT_INPUT_BITS_NB = 0x16;
const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };
//读保持寄存器地址数据的定义
const uint16_t UT_REGISTERS_ADDRESS = 0x6B;
//当此地址用于第一个字节时引发手动异常
const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x6C;
const uint16_t UT_REGISTERS_NB = 0x3;
const uint16_t UT_REGISTERS_TAB[] = { 0x022B, 0x0001, 0x0064 };
const uint16_t UT_REGISTERS_NB_SPECIAL = 0x2;
//输入寄存器地址数据定义
const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x08;
const uint16_t UT_INPUT_REGISTERS_NB = 0x1;
const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };

modbus_t *ctx = NULL;
SOCKET server_socket = -1;
modbus_mapping_t *mb_mapping;

static void close_sigint(int dummy) {
    modbus_free(ctx);
    modbus_mapping_free(mb_mapping);
    exit(dummy);
}

int modbustcp_serv_dowork(void* lpParameter);

int modbustcp_server_start(server_info_t *s_info)
{
    log_debug("modbustcp_server_start");

    std::thread th(modbustcp_serv_dowork,s_info);
    usleep(1000*30);
    th.detach();
    return 0;
}

int modbustcp_serv_dowork(void* lpParameter)
{
    log_debug("Modbus服务启动中");

    server_info_t *s_info=(server_info_t *)lpParameter;

    int master_socket;
    int rc;
    fd_set refset;
    fd_set rdset;
    int fdmax;
    int header_length = 7;

    //启动后操作台自已来连
    ctx = modbus_new_tcp(s_info->ip, s_info->port);

    //设置
    modbus_set_slave(ctx, 1);

    //申请内存区用于存放寄存器数据
    mb_mapping = modbus_mapping_new(UT_BITS_ADDRESS + UT_BITS_NB,//读线圈
                                    UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,//读离散量输入
                                    UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,//读保持寄存器
                                    UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);//读输入寄存器
    if(mb_mapping == NULL)
    {
        //fprintf(stderr, "初始化寄存器失败:%s\n", modbus_strerror(errno));
        log_debug("初始化寄存器失败:%s", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    log_debug("初始化寄存器成功");

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

    log_debug("初始化输入寄存器成功");

    //初始化读保持寄存器
    for (int i = 0; i < UT_REGISTERS_NB; i++)
    {
        mb_mapping->tab_registers[UT_REGISTERS_ADDRESS + i] = UT_REGISTERS_TAB[i];
    }

    log_debug("初始化读保持寄存器成功");

    //开始监听端口
    server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);
    if (server_socket == -1)
    {
        log_debug("无法监听TCP");
        modbus_free(ctx);
        return -1;
    }

    log_debug("ModbusSlave开始监听");

    signal(SIGINT, close_sigint);

    //清除套接字的引用集
    FD_ZERO(&refset);
    //添加服务器套接字
    FD_SET(server_socket, &refset);
    fdmax = server_socket;

    // 开始接收数据
    log_debug("开始接收数据 \n");

    modbus_tcp_accept(ctx, &server_socket);

    //以下实现断网重连机制
    for(;;)
    {
        rdset = refset;
        if (select(fdmax + 1, &rdset, NULL, NULL, NULL) == -1)
        {
            log_debug("服务器 select() 失败");
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
                        log_debug("服务器 accept() 失败");
                    }
                    else
                    {
                        FD_SET(newfd, &refset);
                        if (newfd > fdmax)
                        {
                            //记录最大值
                            fdmax = newfd;
                        }
                        log_debug("新连接 %s:%d on socket %d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
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
                    if (rc > 0) {
                        //mb_mapping->tab_registers[1] = 2333;
                        //回发响应数据
                        modbus_reply(ctx, query, rc, mb_mapping);
                        recvModbusTcp((char *)query,rc,controlMain->can_client.acceptSocket);
                    }
                    else
                    {
                        printf("连接关闭.\n");
                        modbus_close(ctx);
                        //等待下一个客户端报文
                        modbus_tcp_accept(ctx, &server_socket);
                        printf("\n等待下一个客户端报文 \n");
                    }
                }
            }
        }
    }

    printf("停止服务:%s\n", modbus_strerror(errno));

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}
