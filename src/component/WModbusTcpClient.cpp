#include "WModbusTcpClient.h"

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
#include "ConvertUtil.h"

#include <thread>
#include <unistd.h>

#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#define ADDRESS_START 29  //寄存器起始地址
#define ADDRESS_END 30    //寄存器结束地址
modbus_t *pollCtx = NULL;

int modbustcp_client_dowork(void* lpParameter);

int modbustcp_clent_start(server_info_t *s_info){
    std::thread th(modbustcp_client_dowork, s_info);
    usleep(1000 * 30);
    th.detach();
    return 0;
}
//你这里是客户端

int modbustcp_client_dowork(void *lpParameter)
{
    while(1){
        if(controlMain != NULL){
            log_debug("nowValue= %d",controlMain->nowValue);

        }
        usleep(1000*300);
    }
    //log_debug("Modbus Tcp Client");
/*
    server_info_t *s_info=(server_info_t *)lpParameter;

    int ret = -1;
    int nums = 0;

    int addr = 0;

    //新建
    pollCtx = modbus_new_tcp(s_info->ip, s_info->port);
    if(NULL == pollCtx){
        return -1;
    }
    log_debug("设置TCP成功");

    nums = ADDRESS_END - ADDRESS_START;

    //设置从站号
    modbus_set_slave(pollCtx, 1);

    //设置超时时间
    modbus_set_response_timeout(pollCtx, 0, 1000 * 1000);

    //连接控制台
    ret = modbus_connect(pollCtx);
    if(-1 == ret){
        log_debug("连接控制台失败");
    }

    int red_buf_len=nums + 1;
    uint16_t red_buf[red_buf_len+1];

    int red_len=0;
    while(1){
        //那边写了值，这边好象读不了
        red_len = modbus_read_registers(pollCtx, ADDRESS_START, nums + 1, red_buf);
        if(red_len>0){
            log_debug("%d",red_len);
            printf_hex((uint8_t *)red_buf,red_len*2);
        }

//        if(ret != nums + 1){
//            log_debug("读取寄存器失败");
//        }else{
//            if(controlMain != NULL){

//                for(int i = 0; i < 2; i++){
//                    log_debug(QString("%1").arg(buffer16t[i]).toStdString().data());
//                }
//            }
//        }
        usleep(300*1000);
    }
    //断开连接
    modbus_close(pollCtx);
    modbus_free(pollCtx);*/
}

