#include "UWLog.h"
#include "stdarg.h"
#include "stdio.h"
#include <fstream>

#include "FormatTimes.h"

using namespace std;

char logfile[1024]="logfile.log";

ofstream out_log_fp;

void log_open(const char* savefp)
{
    snprintf(logfile, sizeof(logfile),"%s",savefp);
    out_log_fp.open(logfile,ios::trunc);
}

void log_printfs(const char* files,unsigned int line,const char* fmt, ...)
{

    char bufstr[2048]    = "\0";
    char logstr[4096] = "\0";
    char times[20]="\0";

    va_list va_alist;

    va_start(va_alist, fmt);

    vsnprintf(bufstr, sizeof(bufstr), fmt, va_alist);

    va_end (va_alist);

    //printf("logfile save:%s\n",logfile);

    t_stime(times);

    snprintf(logstr,sizeof(logstr),"[%s\t%s\t%d]%s\n",times,files,line,bufstr);

//    snprintf(logstr,sizeof(logstr),"[%s]%s\n",times,bufstr);

    out_log_fp.write(logstr,strlen(logstr));
    out_log_fp.flush();
    //printf("_level:%d\n",_level);

//    if(_level) printf("%s",logbuf);
}

void log_close()
{
    out_log_fp.close();
}


void printf_hex(unsigned char *hexs,unsigned char const *p,int size)
{
    unsigned char bufs[1024];
    memset(bufs,'\0',sizeof(bufs));
    char buf[20];
    for (int i = 0; i < size;i++) {
        memset(buf,'\0',sizeof(buf));
        sprintf(buf,"0x%02x ",*(p+i));
        strcat((char *)bufs,buf);
    }
    strcpy((char *)hexs,(char *)bufs);
//    log_debug((char *)bufs);
    //printf(bufs);
}
