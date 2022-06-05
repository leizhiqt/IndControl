#ifndef COMMAND_PROTOCOL_H
#define COMMAND_PROTOCOL_H
#include <Command.h>

class CommandProtocol : public ICommand
{
public:
    CommandProtocol();
    ~CommandProtocol();

    //预留接口函数
    void processBytesMessage(char *message) override;
};

#endif // COMMAND_PROTOCOL_H
