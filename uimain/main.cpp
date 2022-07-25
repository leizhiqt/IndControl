#pragma execution_character_set("utf-8")

#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "ControlMain.h"
#include "ConvertUtil.h"
#include "UTypes.h"

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
    
//    uchar_8 value[4];
//    value[0] = 0x00;
//    value[1] = 0x01;
//    value[2] = 0x01;
//    value[3] = 0x11;

//    printf("=====%d\n",hl_to_int32(value[0],value[1],value[2],value[3]));

//    qDebug()<<value.numeric;
//        printf("=====%f\n",value.numeric);

    //启动主程序
    controlMain = new ControlMain();

    return qtApp.exec();
    //App end
}
