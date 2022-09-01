#pragma execution_character_set("utf-8")

#include "IOTUtil.h"
#include "MyDownloader.h"

#include <QDateTime>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <Psapi.h>
#include <strsafe.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QProcessEnvironment>

#include <stdio.h>
#include <time.h>

using namespace std;

template<typename T>
void swap_t(T & x, T & y)
{
    T tmp = x;
    x = y;
    y = tmp;
}

//读取当前时间格式化字符串
QString getDateTimeForStr(){
    return QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
}

//读取当前时间格式化字符串
int getQtNowStr(char *nowStr,size_t size){
    QString qdt_str=QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    const char *ptr = qdt_str.toLatin1().constData();
    snprintf(nowStr,size,"%s",ptr);
    return 0;
}

int getNowStr(char *ptr,size_t size){
    time_t t;
    struct tm *lt=nullptr;
    time(&t);//获取Unix时间戳。
    localtime_s(lt,&t);
    //printf( "%d/%d/%d %d:%d:%d\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);//输出结果
    snprintf(ptr,size,"%d%d%d%d%d%d",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    printf("%s\n",ptr);
    return 0;
}

//%APPDATA%
//保存应用程序的数据目录
void readEnv()
{
      QString  appDataPath= QProcessEnvironment::systemEnvironment().value("APPDATA");
      qDebug() << appDataPath;
}

//获取磁盘和磁盘容量
void readHeadDisk()
{
    CHAR szDriveBuf[MAX_PATH] = { 0 };
    DWORD dwLen = GetLogicalDriveStringsA(MAX_PATH * sizeof(CHAR), szDriveBuf);//获取系统字符串长度.
    LPSTR pDrive = szDriveBuf;

    qDebug() << dwLen;

    ULONGLONG i=0;
    ULONGLONG count=dwLen/4;
    vector<QString> v;

    while (pDrive && i<count)
    {
        QString strDriver = pDrive;
        v.push_back(strDriver);

        qDebug() << strDriver;

        pDrive += 4;   //这里就要注意了,如果不懂就看看上面的那个内存图
        i++;
    }

    uint DType;
//    int si = 0;
    BOOL fResult;

    ULARGE_INTEGER i64FreeBytesToCaller;
    ULARGE_INTEGER i64TotalBytes;
    ULARGE_INTEGER i64FreeBytes;

    //为了显示每个驱动器的状态，则通过循环输出实现，由于DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围
    for (ULONGLONG i = 0; i < count; i++)
    {
        QString driveStr(v[i]);
        std::wstring wlpstr = driveStr.toStdWString();
        LPCWSTR lpcwstr_drive = wlpstr.c_str();

        qDebug() << driveStr;

        DType = GetDriveType(lpcwstr_drive);
        //GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录
        if (DType == DRIVE_FIXED)
        {
            qDebug() << QString::fromLocal8Bit("硬盘");
        }
        else if (DType == DRIVE_CDROM)
        {
            qDebug() << QString::fromLocal8Bit("光驱");
        }
        else if (DType == DRIVE_REMOVABLE)
        {
            qDebug() << QString::fromLocal8Bit("可移动式磁盘");
        }
        else if (DType == DRIVE_REMOTE)
        {
            qDebug() << QString::fromLocal8Bit("网络磁盘");
        }
        else if (DType == DRIVE_RAMDISK)
        {
            qDebug() << QString::fromLocal8Bit("虚拟RAM磁盘");
        }
        else if (DType == DRIVE_UNKNOWN)
        {
            qDebug() << QString::fromLocal8Bit("未知设备");
        }
//typedef ULARGE_INTEGER *PULARGE_INTEGER;
        fResult = GetDiskFreeSpaceEx(
            lpcwstr_drive,
            &i64FreeBytesToCaller,
            &i64TotalBytes,
            &i64FreeBytes);

        //GetDiskFreeSpaceEx函数，可以获取驱动器磁盘的空间状态,函数返回的是个BOOL类型数据
        if (fResult)//通过返回的BOOL数据判断驱动器是否在工作状态
        {
            qDebug() << " totalspace:" << i64TotalBytes.QuadPart / 1024 / 1024 << " MB";//磁盘总容量
            qDebug() << " freespace:" << i64FreeBytesToCaller.QuadPart / 1024 / 1024 << " MB";//磁盘剩余空间
        }
        else
        {
            qDebug() << " 设备未准备好";
        }
    }
}

QString read_text(QString filePath){
    QFile file(filePath);
    if(!file.exists()) //文件不存在则退出
    {
        qDebug()<<"file.cfg not exist";
        return nullptr;
    }
    if(!file.open(QIODevice::ReadOnly))//文件不能打开则退出
    {
        qDebug()<<"file.cfg not open";
        return nullptr;
    }

    QTextStream out(&file);
    QString body_text;
    while(!out.atEnd())//未到末尾，就循环读取
    {
        body_text.append(out.readLine());
    }
    file.close();

    return body_text;
}

//系统升级
int update(){
    QString version_url("http://127.0.0.1/download/version.html");
    QString version_path("C:\\version.txt");

    MyDownloader m_downLoder;
    m_downLoder.setData(QUrl(version_url),version_path);
    m_downLoder.cancel();
    m_downLoder.getFile();

    QString last_ver = read_text(version_path);
    QString curr_ver; //当前版本

    if (last_ver == "0.4") {  //==后为当前版本  //诺后端与前端相同
        cout << "无版本更新";
        return 0;
    }

    int betayes;
    cout << "检测到新版本" << endl; //不同
    cout << "是否立即安装(1/2)";
    cin >> betayes;
    if (betayes != 1) {
        return  1;
    }

    QString down_url("http://127.0.0.1/download/update.msi");
    QString down_path("C:\\update.msi");

    m_downLoder.setData(QUrl(down_url),down_path);
    m_downLoder.cancel();
    m_downLoder.getFile();

    QString st("start "+down_path);
    QString del("del "+down_path);
    system(st.toLocal8Bit().data());
    system(del.toLocal8Bit().data());

    return  0;
}

//int up(){
//    QString xiaves;
//    cout << "是否下载beta版(1/2)";
//    int betayes;
//    cin >> betayes;
//    if (betayes == 1) {
//        xiaves = "beta.html";
//    }
//    else {
//        xiaves = "update.html";
//    }

//    QString url_up("http://127.0.0.1/update/update.html");
//    std::wstring wlpstr = url_up.toStdWString();
//    LPCWSTR wangzhi = wlpstr.c_str();

//    const char* szStr = "C:\\bugfixed_update.txt";
//    WCHAR wchar_file[256];
//    memset(wchar_file,0,sizeof(wchar_file));
//    MultiByteToWideChar(CP_ACP,0,szStr,strlen(szStr)+1,
//                        wchar_file,sizeof(wchar_file)/sizeof(wchar_file[0]));

//    HRESULT dyes;
////    cout << "检查中" << endl;
//    dyes = URLDownloadToFile(NULL, wangzhi, wchar_file, 0, NULL); //下载检测文件

//    if (dyes != S_OK)
//    {
//        qDebug() << " 检测失败";
//        return -1;
//    }

//    string temp; //读取文件
//    fstream file1;
//    file1.open("c:\\config.sys",ios::binary|ios::in,0);

//    ifstream infile;
//    infile.open("C:\\bugfixed_update.txt",ios::in | ios::trunc);

//    ifstream readFile("C:\\bugfixed_update.txt");
//    readFile >> temp;
//    cout << temp << endl;
//    readFile.close();

//    if (temp == "0.4") {  //==后为当前版本  //诺后端与前端相同
//        cout << "无版本更新";
//        return 0;
//    }

//    cout << "检测到新版本" << endl; //不同
//    cout << "是否立即安装(1/2)";
//    cin >> betayes;

//    if (betayes != 1) {
//        return  1;
//    }

//    QString down_url("http://127.0.0.1/download/"+xiaves);
//    std::wstring down_wstr = url_up.toStdWString();
//    LPCWSTR down_lpcwstr = wlpstr.c_str();

//    dyes = URLDownloadToFile(NULL, down_lpcwstr, _T("C:\\update.msi"), 0, NULL);
//    if (dyes != S_OK) {
//        cout << "下载失败" << endl;
//        return -2;
//    }
//    system("start c:\\update.msi");
//    system("del c:\\update.msi");

//    return  0;
//}
//QCoreApplication::applicationDirPath()
