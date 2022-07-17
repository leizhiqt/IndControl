#pragma execution_character_set("utf-8")

#include "QTcpComponentl.h"
#include "ControlMain.h"
#include "UWLog.h"
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

QTcpComponentl::QTcpComponentl(){
    visualTcpSocket = new QTcpSocket(this);
    connect(visualTcpSocket, SIGNAL(connection()), this, SLOT(onConnection()));
    connect(visualTcpSocket, SIGNAL(readyRead()), this, SLOT(readRes()));
    connect(visualTcpSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

QTcpComponentl::~QTcpComponentl(){

}

void QTcpComponentl::onConnection(){
    log_debug("onConnection");
}

void QTcpComponentl::socketDisconnected(){
    log_debug("socketDisconnected");
}

void QTcpComponentl::readRes(){
    log_debug("readRes");
    QByteArray data = visualTcpSocket->readAll();
    QString str(data);
}

void QTcpComponentl::send(uint8_t *buf){

}

void QTcpComponentl::toConnection(QString host,int port){
    visualTcpSocket->connectToHost(host,port);
}
