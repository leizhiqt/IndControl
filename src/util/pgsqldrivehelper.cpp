#include "PGSQLDriveHelper.h"
#include "Conf.h"
#include "UWLog.h"
#include <iostream>

//using namespace std;

PGSQLDriveHelper::PGSQLDriveHelper(){

}

int PGSQLDriveHelper::pg_add_exec(const char *table,char *host,int port,const char* data_hex)
{
//    log_debug(__FUNCTION__);
    Conf *conf = Conf::getInstance();
//log_debug(__FUNCTION__);
    //"PORT=5434;DATABASE=TPJY_DB;HOST=192.168.1.119;PASSWORD=123456;USER ID=postgres"
    PGconn *conn = PQsetdbLogin(conf->pgHostIp.toLatin1().data(), conf->pgHostPort.toLatin1().data(),
                                NULL, NULL,
                                conf->pgDataBase.toLatin1().data(), conf->pgUser.toLatin1().data(), conf->pgPass.toLatin1().data());
//    log_debug(__FUNCTION__);
    //主机IP，端口号，附加选项，NULL，数据库名，用户名，密码
    if (conn==nullptr)
    {
        log_debug(__FUNCTION__);
    }

    char query[1024];
    snprintf(query,sizeof(query),"insert into %s(data_hex,data_add,data_port)  VALUES ('%s','%s','%d')",table,data_hex,host,port);

    log_debug("exec SQL:%s",query);

    PGresult *result = PQexec(conn, query);//执行SQL语句，关键字要大写
    int rows=PQntuples(result);
    int cols=PQnfields(result);

//    log_debug("rows:%d clos:%d",rows,cols);

    for (int i = 0; i < rows; i++)//结果行数
    {
        for (int j = 0; j < cols; j++)//结果列数
        {
            log_debug(PQgetvalue(result, i, j));
        }
    }

    //应该在结果不需要的时候 PQclear PGresult，以避免内存泄漏
    PQclear(result);

    //关闭数据库连接并清理
    PQfinish(conn);

//    log_debug(__FUNCTION__);

    return  0;
}

int PGSQLDriveHelper::pg_search()
{
    log_debug(__FUNCTION__);
    Conf *conf = Conf::getInstance();

    //"PORT=5434;DATABASE=TPJY_DB;HOST=192.168.1.119;PASSWORD=123456;USER ID=postgres"
    PGconn *conn = PQsetdbLogin(conf->pgHostIp.toLatin1().data(), conf->pgHostPort.toLatin1().data(), NULL, NULL, conf->pgDataBase.toLatin1().data(), conf->pgUser.toLatin1().data(), conf->pgPass.toLatin1().data());
    //主机IP，端口号，附加选项，NULL，数据库名，用户名，密码
    if (conn==nullptr)
    {
       log_debug(__FUNCTION__);
    }

    log_debug(__FUNCTION__);

    PGresult *result = PQexec(conn, "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");//执行SQL语句，关键字要大写
    int rows=PQntuples(result);
    int cols=PQnfields(result);

    log_debug("rows:%d clos:%d",rows,cols);
    for (int i = 0; i < rows; i++)//结果行数
    {
        for (int j = 0; j < cols; j++)//结果列数
        {
            log_debug(PQgetvalue(result, i, j));
        }
    }

    //应该在结果不需要的时候 PQclear PGresult，以避免内存泄漏
    PQclear(result);

    //关闭数据库连接并清理
    PQfinish(conn);

    log_debug(__FUNCTION__);

    return  0;
}

//连接数据库
//        QSqlDatabase db;

//连接数据库
//    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
//    db = QSqlDatabase::addDatabase("QPSQL");
//    log_debug("Conf");
//    db.setHostName(pgHostIp);
//    log_debug("Conf");
//    db.setPort(pgHostPort.toInt());
//    db.setDatabaseName(pgDataBase);
//    db.setUserName(pgUser);
//    db.setPassword(pgPass);
//    log_debug("Conf");

//void Conf::saveDb()
//{
//    //从数据库中取出需要发送的数据
//    log_debug("db.open");
//    if(db.open()){
//        log_debug("db.open");

//        QString strId;
//        QString strFrameXvalue;
//        QString strFrameYvalue;
//        QString strFrameZvalue;
//        QString strFramePitchangle;
//        QString strFrameRollangle;
//        QString strFrameHeadingangle;
//        QString strCuttingXvalue;
//        QString strCuttingYvalue;
//        QString strCuttingZvalue;
//        QString strCuttingPitchangle;
//        QString strCuttingRollangle;
//        QString strCuttingHeadingangle;
//        QString strStationStatus;
//        QSqlQuery query("select * from prog_postures_send where data_status = 0");
//        while(query.next()){
//            strId = query.value("id").toString();
//            strFrameXvalue = query.value("frame_xvalue").toString();
//            strFrameYvalue = query.value("frame_yvalue").toString();
//            strFrameZvalue = query.value("frame_zvalue").toString();
//            strFramePitchangle = query.value("frame_pitchangle").toString();
//            strFrameRollangle = query.value("frame_rollangle").toString();
//            strFrameHeadingangle = query.value("frame_headingangle").toString();
//            strCuttingXvalue = query.value("cutting_xvalue").toString();
//            strCuttingYvalue = query.value("cutting_yvalue").toString();
//            strCuttingZvalue = query.value("cutting_zvalue").toString();
//            strCuttingPitchangle = query.value("cutting_pitchangle").toString();
//            strCuttingRollangle = query.value("cutting_rollangle").toString();
//            strCuttingHeadingangle = query.value("cutting_headingangle").toString();
//            strStationStatus = query.value("station_status").toString();
//            //封装报文
//            QString strSend = "AA BB "; //通讯头
//            //机身X坐标测量值
////            strSend += tranFloatToHex(strFrameXvalue);
////            //机身Y坐标测量值
////            strSend += tranFloatToHex(strFrameYvalue);
////            //机身Z坐标测量值
////            strSend += tranFloatToHex(strFrameZvalue);
////            //机身俯仰角测量值
////            strSend += tranFloatToHex(strFramePitchangle);
////            //机身横滚角测量值
////            strSend += tranFloatToHex(strFrameRollangle);
////            //机身航向角测量值
////            strSend += tranFloatToHex(strFrameHeadingangle);
////            //截割头X坐标测量值
////            strSend += tranFloatToHex(strCuttingXvalue);
////            //截割头Y坐标测量值
////            strSend += tranFloatToHex(strCuttingYvalue);
////            //截割头Z坐标测量值
////            strSend += tranFloatToHex(strCuttingZvalue);
////            //截割头俯仰角测量值
////            strSend += tranFloatToHex(strCuttingPitchangle);
////            //截割头横滚角测量值
////            strSend += tranFloatToHex(strCuttingRollangle);
////            //截割头航向角测量值
////            strSend += tranFloatToHex(strCuttingHeadingangle);
////            //惯导+全站仪状态
////            strSend += tranFloatToHex(strStationStatus);
//            //结束符
//            strSend += " BF";
//            //发送
//            QByteArray sendMessage = strSend.toLocal8Bit();
//            //修改数据库的发送标志
//            query.exec("update prog_postures_send set data_status = 1 where id = " + strId);
//        }
//    }
//}

