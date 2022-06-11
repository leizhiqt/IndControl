#ifndef PGSQLDRIVEHELPER_H
#define PGSQLDRIVEHELPER_H
#include <libpq-fe.h>

//从外部引入
extern void PQfinish(PGconn *conn);
extern void PQclear(PGresult *res);

class PGSQLDriveHelper
{
private:
    PGSQLDriveHelper();//1.构造函数私有
    PGSQLDriveHelper(const PGSQLDriveHelper &);//拷贝构造函数不实现，防止拷贝产生多个实例
    PGSQLDriveHelper & operator =  (const PGSQLDriveHelper &);//复制构造函数不实现，防止赋值产生多个实例

 public:
    ~PGSQLDriveHelper(){}

    static PGSQLDriveHelper * getInstance()//2.提供全局访问点
    {
        static PGSQLDriveHelper newServer;//3.c++11保证了多线程安全，程序退出时，释放资源
        return &newServer;
    }

    int pg_add_exec(const char *table,char *host,int port,const char* data_hex);
    int pg_search();
};

#endif // PGSQLDRIVEHELPER_H
