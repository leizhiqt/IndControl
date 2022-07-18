#ifndef PROTOCOLWEBJSON_H
#define PROTOCOLWEBJSON_H
#include <QString>
#include "WebSocketCommand.h"

class ProtocolWebJson:public WebSocketCommand
{
public:
    ProtocolWebJson(){}
    ~ProtocolWebJson(){}
    void recvTextMessage(const QString &content) override;
};

#endif // PROTOCOLWEBJSON_H
