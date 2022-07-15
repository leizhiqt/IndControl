﻿#include "ConvertUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

int hexs_to_binary(char *in,int len, unsigned char *out) {
//    int len = (int)strlen(in);
    char *str = (char *)malloc(len);
    memset(str, 0, len);
    memcpy(str, in, len);
    for (int i = 0; i < len; i+=2) {
        //小写转大写
        if(str[i] >= 'a' && str[i] <= 'f') str[i] = str[i] & ~0x20;
        if(str[i+1] >= 'a' && str[i] <= 'f') str[i+1] = str[i+1] & ~0x20;
        //处理第前4位
        if(str[i] >= 'A' && str[i] <= 'F')
            out[i/2] = (str[i]-'A'+10)<<4;
        else
            out[i/2] = (str[i] & ~0x30)<<4;
        //处理后4位, 并组合起来
        if(str[i+1] >= 'A' && str[i+1] <= 'F')
            out[i/2] |= (str[i+1]-'A'+10);
        else
            out[i/2] |= (str[i+1] & ~0x30);
    }
    free(str);
    return 0;
}

int binary_to_hexs(unsigned char *in, int len, char *out) {
    for (int i = 0; i < len; i++) {
        if ((in[i] >> 4) >= 10 && (in[i] >> 4) <= 15)
            out[2*i] = (in[i] >> 4) + 'A' - 10;
        else
            out[2*i] = (in[i] >> 4) | 0x30;

        if ((in[i] & 0x0f) >= 10 && (in[i] & 0x0f) <= 15)
            out[2*i+1] = (in[i] & 0x0f) + 'A' - 10;
        else
            out[2*i+1] = (in[i] & 0x0f) | 0x30;
    }
    return 0;
}

void utf8ToGbk(char *utf8String, char *gbkString)
{
    wchar_t *unicodeStr = NULL;
    int nRetLen = 0;

    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);

    //求需求的宽字符数大小
    unicodeStr = (wchar_t *)malloc(nRetLen * sizeof(wchar_t));
    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, unicodeStr, nRetLen);

    //将utf-8编码转换成unicode编码
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, 0);

    //求转换所需字节数
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, gbkString, nRetLen, NULL, 0);

    //unicode编码转换成gbk编码
    free(unicodeStr);
}

/*
int main(int argc, const char * argv[]) {
//    unsigned char* binary = (unsigned char*)malloc(sizeof(unsigned char )*((len/2)+1));
//    hexs_to_binary(buf,len,binary);
//    printf_hex(binary,len/2);
//    emit controlMain->webSocket->broadcast_msg((char *)binary);
//    char hexs[512];
//    sprintf_hex(hexs,(char*)binary,(len/2));
//    log_debug("hexs len=%d %s",len,hexs);

//    free(binary);
    //接收到的数据
//        PGSQLDriveHelper::getInstance()->pg_add_exec((char *)hexs);
//        PGSQLDriveHelper::getInstance()->pg_add_exec("prog_postures_recv",info.ip,info.port,(char *)hexs);

    //printf_hex(hexs,(char*)recvBuf,count);
    //log_debug("recv csocket=%d len=%d %s",info.acceptSocket,strlen((char *)hexs),hexs);

    char *str = "fa32333435363738393a3b3c3d3e3f40";
    unsigned char temp[16] = {0};
    hexstringtobyte(str, temp);
    for (int i = 0; i < 16; i++) {
        printf("%d ", temp[i]);
    }
    printf("\n");
    char out[33] = {0}; // 多留了一位存放 '\0'
    bytetohexstring(temp, 16, out);
    printf("%s\n", out);
    return 0;
}*/