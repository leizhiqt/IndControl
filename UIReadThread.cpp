#include "UIReadThread.h"
#include <Windows.h>
#include <QTextStream>
#include <QFile>
#include "UWLog.h"
#include "Conf.h"

UIReadThread::UIReadThread(Ui::CanOpenUI *ui):ui(ui)
{
    log_debug("UIReadThread New");

    ui->textBrowser->clear();//清空
    ui->textBrowser->document()->setMaximumBlockCount(100);
}

UIReadThread::~UIReadThread(){
    log_debug("UIReadThread Free");
}

void UIReadThread:: run()
{
//        while(1)
//        {
//            log_debug("读取失败");
//            Sleep(1000);
//        }

    log_debug("UIReadThread finsh");

    QString aFileName="IndControl.txt";
    QFile   aFile(aFileName);
    if (!aFile.exists()) //文件不存在
    {
        printf("文件不存在\n");
        return;
    }
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        printf("读取失败\n");
        return;
    }

    QTextStream aStream(&aFile);
    aStream.setAutoDetectUnicode(true); //自动检测Unicode,才能正常显示文档内的汉字

    ui->textBrowser->append("======================================="); //添加到文本框显示
    while(1)
    {
//        if(can_stop){
//            break;
//        }

        if(aStream.atEnd()){
            Sleep(100);
            continue;
        }

        //按行读取
        QString str=aStream.readLine();//读取文件的一行
        ui->textBrowser->append(str); //添加到文本框显示
        Sleep(10);
    }

    aStream.flush();
    aFile.flush();
    aFile.close();//关闭文件

    log_debug("UIReadThread finsh");
}
