#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "ControlMain.h"
#include "ConvertUtil.h"
#include "UTypes.h"
#include <QSurfaceFormat>

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
    
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    fmt.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(fmt);

    //启动主程序
    controlMain = new ControlMain();

    return qtApp.exec();
    //App end
}
