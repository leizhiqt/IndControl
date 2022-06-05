#ifndef COMMAND_H
#define COMMAND_H

class ICommand
{
public:
    ICommand();
    virtual ~ICommand() = 0;

    //解析入口函数
    virtual void processBytesMessage(char *message)=0;
};

#endif // COMMAND_H
