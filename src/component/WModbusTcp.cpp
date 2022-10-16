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

#define NB_CONNECTION    50

//线圈地址数据的定义
const uint16_t UT_BITS_ADDRESS = 0x13;
const uint16_t UT_BITS_NB = 0x25;
const uint8_t UT_BITS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };
//离散量输入寄存器地址数据的定义
const uint16_t UT_INPUT_BITS_ADDRESS = 0xC4;
const uint16_t UT_INPUT_BITS_NB = 0x16;
const uint8_t UT_INPUT_BITS_TAB[] = { 0xAC, 0xDB, 0x35 };
//读保持寄存器地址数据的定义
const uint16_t UT_REGISTERS_ADDRESS = 0;
//当此地址用于第一个字节时引发手动异常
const uint16_t UT_REGISTERS_ADDRESS_SPECIAL = 0x6C;
const uint16_t UT_REGISTERS_NB = 0x64;
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
    std::thread th(modbustcp_serv_dowork,s_info);
    usleep(1000*30);
    th.detach();
    return 0;
}
//这个是服务端//你看我改
//不忙改 要先理一下
//过程是这样
//这边不停的在接收控制台的报文
//如果他发过来的报文功能码是06，表示要变换寄存器的值
//所以我们收到06的报文后，要将相应地址的值改为他传过来的值
//然后另外一个类，主要是实时获取29这个地址的值返给JAVA
int modbustcp_serv_dowork(void* lpParameter)
{
    log_debug("Modbus服务启动中");

    server_info_t *s_info=(server_info_t *)lpParameter;

    int master_socket;
    int rc;
    fd_set refset;
    fd_set rdset;
    int fdmax;
    int header_length;

    //启动后操作台自已来连
    ctx = modbus_new_tcp(s_info->ip, s_info->port);

    //申请内存区用于存放寄存器数据
    mb_mapping = modbus_mapping_new(UT_BITS_ADDRESS + UT_BITS_NB,//读线圈
                                    UT_INPUT_BITS_ADDRESS + UT_INPUT_BITS_NB,//读离散量输入
                                    UT_REGISTERS_ADDRESS + UT_REGISTERS_NB,//读保持寄存器
                                    UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB);//读输入寄存器
    if(mb_mapping == NULL)
    {
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

    //设置从机地址
    int slaveResult = modbus_set_slave(ctx, 1);
    if(slaveResult != 0){
        log_debug("modbus_set_slave error: %s", modbus_strerror(errno));
    }

    //设置超时时间
    struct timeval timeout;
    uint32_t tv_sec = 30000;
    uint32_t tv_usec = 100000;
    timeout.tv_sec = tv_sec;
    timeout.tv_usec = tv_usec;
    modbus_set_response_timeout(ctx, tv_sec, tv_usec);

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
    header_length = modbus_get_header_length(ctx);

    // 开始接收数据
    log_debug("开始接收数据 \n");

    //以下实现断网重连机制
    for(;;)
    {
        rdset = refset;
        if (select(fdmax + 1, &rdset, NULL, NULL, &timeout) == -1)
        {
            log_debug("服务器select()失败");
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
                    if (newfd == -1){
                        log_debug("服务器accept()失败");
                    }
                    else
                    {
                        FD_SET(newfd, &refset);
                        if (newfd > fdmax){
                            //记录最大值
                            fdmax = newfd;
                        }
                        log_debug("新连接 %s:%d on socket %d\n", inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
                    }
                }else{
                    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
                    modbus_set_socket(ctx, master_socket);
                    //接收报文
                    rc = modbus_receive(ctx, query);
                    if (rc > 0) {
                        //回发响应数据
                        modbus_reply(ctx, query, rc, mb_mapping);
                        if(query[header_length] == 0x06){
                            if(controlMain != NULL){
                                uint16_t data = (query[10]<<8) + query[11];
                                uint16_t addr = ((query[8]<<8) + query[9])+1;

                                log_debug("addr=%d %02x %02x",addr,query[8],query[9]);
                                log_debug("value=%d %02x %02x",data,query[10],query[11]);

                                if(addr == 29){
                                    //这里没进来
                                    log_debug("11111");
                                    controlMain->nowValue = data;
                                }
                                if(controlMain->can_client.acceptSocket != NULL){
                                    recvModbusTcp((char *)query,rc,controlMain->can_client.acceptSocket);
                                }
                            }
//                            printf("报文长度 = %d \n", rc);
//                            for (int i = 0; i < rc; i++) {
//                                printf("%02x ", query[i]);
//                            }
//                            printf("\n报文接收完成\n");

                            //log_debug("addr=%d %02x %02x",addr,query[8],query[9]);
                            //log_debug("value=%d %02x %02x",data,query[10],query[11]);
                            //这里判断一下要变更数据的地址是不是29
                            //如果是则修改全局变量的值为当前值

                            //modbus_write_register(ctx, addr, data);
                            //modbus_flush(ctx);

                            //转发报文
                            //这里要判断一下，不然程序会崩溃
                            //if(controlMain != NULL){
                                //if(controlMain->can_client.acceptSocket != NULL){
                                    //recvModbusTcp((char *)query,rc,controlMain->can_client.acceptSocket);
                                //}
                            //}
                        }
                    }
                    else
                    {
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
