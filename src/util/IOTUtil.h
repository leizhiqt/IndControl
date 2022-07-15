#pragma execution_character_set("utf-8")

#ifndef IOTUTILS_H
#define IOTUTILS_H
#include <QString>

#define MIN(a,b) {((a<b) ? (a:b))}

#define register(clsName,regName) {clsName regName;}

//extern "C" {

template<typename T>
void swap_t(T & x, T & y);

//读取当前时间格式化字符串
extern QString getDateTimeForStr();

extern int getNowStr(char *ptr,size_t size);

//获取磁盘和磁盘容量
extern void readHeadDisk();

//保存应用程序的数据目录
extern void readEnv();

//系统升级
extern int update();

//}

#endif // IOTUTILS_H
