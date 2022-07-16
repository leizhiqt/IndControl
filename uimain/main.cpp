#pragma execution_character_set("utf-8")

#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "ControlMain.h"
#include "ConvertUtil.h"

ControlMain *controlMain;

int main(int argc, char *argv[])
{
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
    
//    UStuff value;
//    value.numeric=0.0;

//    value.ascii[3] = 0x66;
//    value.ascii[2] = 0x41;
//    value.ascii[1] = 0x7c;
//    value.ascii[0] = 0xcc;

//    qDebug()<<value.numeric;
//        printf("=====%f\n",value.numeric);

    //启动主程序
    controlMain = new ControlMain();

    return qtApp.exec();
    //App end
}
