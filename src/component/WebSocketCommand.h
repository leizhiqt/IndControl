#ifndef WEBSOCKETCOMMAND_H
#define WEBSOCKETCOMMAND_H
#include <QString>

class WebSocketCommand
{
public:
//    WebSocketCommand(){}
//    virtual ~WebSocketCommand()=0;
    virtual void recvTextMessage(const QString &content)=0;
//    virtual void disp()=0;//纯虚函数
};
#endif // WEBSOCKETCOMMAND_H
