#include "ConvertUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "UWLog.h"
#include <QDebug>

void byte1_to_char(char *hexs,unsigned char va)
{
    memset(hexs,'\0',21);
    sprintf(hexs,"%02x",va);
}

float32_t byte4_to_float32(uchar_8 *va)
{
    UStuff32_t value;
    value.ascii[3]=*va;
    value.ascii[2]=*(va+1);
    value.ascii[1]=*(va+2);
    value.ascii[0]=*(va+3);
    return value.va_float;
}


void ntoh_32(char *net_bytes)
{
    unsigned char host_bytes[4]={0};

    host_bytes[0]=net_bytes[3];
    host_bytes[1]=net_bytes[2];
    host_bytes[2]=net_bytes[1];
    host_bytes[3]=net_bytes[0];

    net_bytes[0]=host_bytes[0];
    net_bytes[1]=host_bytes[1];
    net_bytes[2]=host_bytes[2];
    net_bytes[3]=host_bytes[3];
}

void swap_byte32(uchar_8 *htol,const uchar_8 *ltoh)
{
    *(htol+0)=*(ltoh+3);
    *(htol+1)=*(ltoh+2);
    *(htol+2)=*(ltoh+1);
    *(htol+3)=*(ltoh+0);
}


void copy_byte32(uchar_8 *htol,const uchar_8 *ltoh)
{
    *(htol+0)=*(ltoh+0);
    *(htol+1)=*(ltoh+1);
    *(htol+2)=*(ltoh+2);
    *(htol+3)=*(ltoh+3);
}

void ntoh_16(char *net_bytes)
{
    unsigned char host_bytes[2]={0};

    host_bytes[0]=net_bytes[1];
    host_bytes[1]=net_bytes[0];

    net_bytes[0]=host_bytes[0];
    net_bytes[1]=host_bytes[1];
}

void sprintf_hex(char *hexs,unsigned char const *p,int size)
{
    unsigned char bufs[1024];
    memset(bufs,'\0',sizeof(bufs));
    char buf[20];
    for (int i = 0; i < size;i++) {
        memset(buf,'\0',sizeof(buf));
        sprintf(buf,"%02x ",(unsigned char)*(p+i));
        strcat((char *)bufs,buf);
    }
    strcpy((char *)hexs,(char *)bufs);
}

void printf_hex(unsigned char *hexs,int len)
{
    for(int i=0;i<len;i++){
        printf(" 0x%02x",*(hexs+i));
    }
    printf("\n");
}

int hexs_to_binary(const char *in,int len, unsigned char *out) {
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

QString float_to_hex(float va)
{
   char hexs[128];
   memset(hexs,'\0',sizeof(hexs));
   UStuff32_t value;
   value.va_float=va;
   sprintf(hexs,"%02x%02x%02x%02x",value.ascii[3],value.ascii[2],value.ascii[1],value.ascii[0]);
   return QString(hexs);
}

QString int_to_hex(int va)
{
    char hexs[128];
    memset(hexs,'\0',sizeof(hexs));
    UStuff32_t value;
    value.va_int=va;
    sprintf(hexs,"%02x%02x%02x%02x",value.ascii[3],value.ascii[2],value.ascii[1],value.ascii[0]);
    return QString(hexs);
}

char convertCharToHex(char ch)
{
    if((ch >= '0') && (ch <= '9'))
         return ch-0x30;
     else if((ch >= 'A') && (ch <= 'F'))
         return ch-'A'+10;
     else if((ch >= 'a') && (ch <= 'f'))
         return ch-'a'+10;
    else return (-1);
}

//QString 转换为 Hex ByteArray
void convertStringToHex(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertCharToHex(hstr);
        lowhexdata = convertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}

//16进制字符串转换ByteArray
/*
 * @breif 将16进制字符串转换为对应的字节序列
 */
QByteArray HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
//                qDebug()<<"非法的16进制字符："<<s;
//                QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
//    qDebug()<<ret;
    return ret;
}


//ByteArray 转换为 16进制字符串
QString ByteArrayToHexString(QByteArray data){
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
//    qDebug()<<len;
    for(int i=1;i<len;i++)
    {
//        qDebug()<<i;
        ret.insert(2*i+i-1," ");
    }
    return ret;
}

void str_trim(const char *s1,const int len,char *s2)
{
    for(int i=0;i<len;i++){
         if(!isspace((int)*(s1+i))){
            *s2 = *(s1+i);
             s2++;
         }
    }
    *s2 = '\0';
}

int example_convert()
{
    QString str="12abcd";

    QString str1=str.toLatin1().toHex(' ');

    printf("%s\n",str1.toLatin1().data());

    str1.replace(" ","");

    printf("%s\n",str1.toLatin1().data());

    QByteArray ba;
    char *p = ba.begin();
    convertStringToHex(str1,ba);

    printf("%d\n",ba.length());
    int len=ba.length();
    for(int i=0;i<len;i++){
        printf("%02x\n",(unsigned char)*(p+i));
    }

    QString hexs=ByteArrayToHexString(ba);
    printf("%s\n",hexs.toLatin1().data());

    return 0;
}
