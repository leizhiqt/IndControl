#include "PGSQLDriveHelper.h"
#include <iostream>
#include <QDebug>

//using namespace std;

int PGSQLDriveHelper::pg_test()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;

    //"PORT=5434;DATABASE=TPJY_DB;HOST=192.168.1.119;PASSWORD=123456;USER ID=postgres"
    PGconn *conn = PQsetdbLogin("192.168.1.119", "5434", NULL, NULL, "TPJY_DB", "postgres", "123456");
    //主机IP，端口号，附加选项，NULL，数据库名，用户名，密码
    if (conn==nullptr)
    {
        qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    }

    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;

    PGresult *result = PQexec(conn, "SELECT * FROM iot_device_info");//执行SQL语句，关键字要大写
    int rows=PQntuples(result);
    int cols=PQnfields(result);

    qDebug()<<__FILE__<<__LINE__<<"rows:"<<rows<<" clos:"<<cols;

    for (int i = 0; i < rows; i++)//结果行数
    {
        for (int j = 0; j < cols; j++)//结果列数
        {
            qDebug()<<__FILE__<<__LINE__<<PQgetvalue(result, i, j);
        }
        qDebug() << endl;
    }

    //应该在结果不需要的时候 PQclear PGresult，以避免内存泄漏
    PQclear(result);

    //关闭数据库连接并清理
    PQfinish(conn);

    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;

    return  0;
}
