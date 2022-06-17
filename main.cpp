#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "ControlMain.h"

ControlMain *controlMain;

int main(int argc, char *argv[])
{
    //App start
//    qInstallMessageHandler(myMessageOutput);
//    qDebug("[%s %d]:%s",__FILE__ , __LINE__,"main");

//    printf("%s %d\n",__FILE__ , __LINE__);
    //>>测试
//    PGSQLDriveHelper pgsql;
//    pgsql.pg_test();
    //MOD TEST
//    readEnv();
//    readHeadDisk();
    //<<测试

    QApplication qtApp(argc, argv);
    setbuf(stdout, NULL);
    
    QSystemSemaphore semaphore("ProgramKey",1, QSystemSemaphore::Open);
    semaphore.acquire();
    //在临界区操作共享内存SharedMemory
    QSharedMemory memory("IotService");//全局对象名
    if(!memory.create(1)) //如果全局对象存在则提示退出
    {
       QMessageBox::information(0, "提示", "IotService 已经运行!");
       semaphore.release();
       exit(0);
    }
    semaphore.release();
    
//    //启动主程序
    controlMain = new ControlMain();

    return qtApp.exec();
    //App end
}
