#include "QTcpComponentl.h"
#include "ControlMain.h"
#include "UWLog.h"
#include <Windows.h>

QTcpComponentl::QTcpComponentl(){
    visualTcpSocket = new QTcpSocket(this);
    connect(visualTcpSocket, SIGNAL(connection()), this, SLOT(onConnection()));
    connect(visualTcpSocket, SIGNAL(readyRead()), this, SLOT(readRes()));
    connect(visualTcpSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    //visualTcpSocket->connectToHost(host,port);

//    port=1520;
    //tcp server

//    tcpSer = new QTcpServer(this);
//    if( !tcpSer->listen(QHostAddress::LocalHost, port+1) )
//    {   //监听本地主机的13888端口，如果出错就输出错误信息，并关闭
//        qDebug() << tcpSer->errorString();
//        tcpSer->close();
//    }
//    //连接信号和相应槽函数
//    connect(tcpSer, SIGNAL(newConnection()), this, SLOT(sendMessages()));
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
//    controlMain->mWin->Append(str);
}

void QTcpComponentl::send(uint8_t *buf){
//   int ok= visualTcpSocket->write((char *)buf);
       //visualTcpSocket->flush();
//   int ok=visualTcpSocket->writeData((char *)buf,13);
//       log_debug("send %d",ok);
}

void QTcpComponentl::toConnection(QString host,int port){
    visualTcpSocket->connectToHost(host,port);
}


//void TcpClientUtil::sendMessages()
//{
//    QByteArray block;   //用于暂存我们要发送的数据
//    QDataStream out(&block, QIODevice::WriteOnly);  //使用数据流写入数据
//    out.setVersion(QDataStream::Qt_4_7);
//    out << (quint16) 0;
//    out << tr(" This is Modbus tcp server 3q");
//    out.device()->seek(0);
//    out << (quint16) (block.size() - sizeof(quint16));

//    //获取已经建立的连接的子套接字
//    QTcpSocket *clientConnection = tcpSer->nextPendingConnection();
//    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
//    /*clientConnection->disconnectFromHost();它表示当发送完成时就会断开连接，
//     *这时就会发出disconnected()信号，
//     *而最后调用deleteLater()函数保证在关闭连接后删除该套接字clientConnection。
//    */
////    for(int i=0;i<100;i++){
//        clientConnection->write(block);
//        clientConnection->disconnectFromHost();
//        controlMain->mWin->ui->ModBusView->append("Send message successfil");
////        Sleep(1000*20);
////    }

//}


