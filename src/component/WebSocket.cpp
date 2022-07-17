#pragma execution_character_set("utf-8")

#include "WebSocket.h"
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>
#include <QSharedMemory>
#include <QBuffer>
#include "WTcpClient.h"
#include "UWLog.h"
#include "PGSQLDriveHelper.h"
#include "ControlMain.h"
#include "ProtocolXly.h"
#include "ConvertUtil.h"

extern SOCKET jhjSocket;

WebSocket::WebSocket(quint16 port)
{
    this->port=port;
    init();

    this->start();

    log_debug("开始监听:%d",port);
}

/* 初始化WebSocket服务 */
void WebSocket::init()
{
    m_websocketserver = new QWebSocketServer("server", QWebSocketServer::NonSecureMode);
    connect(m_websocketserver, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    m_websocketserver->listen(QHostAddress::Any,port);

    connect(this, SIGNAL(broadcast_msg(QByteArray)), this, SLOT(slot_broadcast_msg(QByteArray)));
}

WebSocket::~WebSocket()
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    stop();
}

void WebSocket::run()
{
    while(1){
        sleep(180);
    }
}

/* 停止服务 */
void WebSocket::stop()
{
    //移除所有客户端连接
    for (int i = 0; i < m_clients.size(); ++i)
    {
        QWebSocket *csocket = (QWebSocket *)m_clients.at(i);
        if(csocket->state()==QAbstractSocket::SocketState::ConnectedState)
            csocket->close();
        m_clients.removeAt(i);
    }

    m_websocketserver->close();
    m_websocketserver=NULL;
}

/* 新连接接入 */
void WebSocket::onNewConnection()
{
    //qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    client_socket = m_websocketserver->nextPendingConnection();

    connect(client_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(recvTextMessage(QString)));

    connect(client_socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(recvBinaryMessage(QByteArray)),Qt::UniqueConnection);

    connect(client_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    m_clients.push_back(client_socket);
}

/* 收到消息 */
void WebSocket::recvBinaryMessage(const QByteArray &message)
{
    qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
    const char *recvBuf = message.begin();
    tcp_client_send((controlMain->canOpenSocket),recvBuf,strlen(recvBuf));
    tcp_server_broadcast(controlMain->xly_srv,(char *)recvBuf,strlen(recvBuf));
}



/* 收到消息 */
void WebSocket::recvTextMessage(const QString &message)
{
    //JAVA发来的消息
    QByteArray msgBytes = message.toLatin1().constData();
    qDebug()<<msgBytes;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(msgBytes);
    if(jsonDocument.isNull() || (!jsonDocument.isObject() && jsonDocument.isArray())){
        log_debug("Websocket解析结果：接收到的消息不正确");
        slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"消息格式不正确\"}");
        return;
    }

    //解析消息
    QJsonObject jsonObject = jsonDocument.object();
    QJsonValue cmdName = jsonObject.value("method");
    if(cmdName == NULL || cmdName.isNull() || cmdName.isUndefined()){
        log_debug("Websocket解析结果：命令不正确");
        slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"命令格式不正确\"}");
        return;
    }

    //如果消息内容是参数配置
    if(cmdName.toString() == "paramset"){
        QJsonValue paramObject = jsonObject.value("params");
        if(!paramObject.isObject()){
            log_debug("Websocket解析结果：参数格式不正确");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数格式不正确\"}");
            return;
        }
        //取得各参数值
        QJsonObject valueContent = paramObject.toObject();
        if(valueContent.count() != 35){
            log_debug("Websocket解析结果：参数个数不正确，应该有35个参数");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数个数不正确，应该有35个参数\"}");
            return;
        }
        QList<float> paramGroup;
        //前置相机参数-标靶长度	0-500
        float targetlength = valueContent.value("targetlength").toString().toFloat();
        if(targetlength < 0 || targetlength > 500){
            log_debug("Websocket解析结果：前置相机参数-标靶长度不正确[值域:0-500]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"前置相机参数-标靶长度不正确[值域:0-500]\"}");
            return;
        }
        paramGroup.append(targetlength);
        //前置相机参数-标靶宽度	0-500
        float targetwidth = valueContent.value("targetwidth").toString().toFloat();
        if(targetwidth < 0 || targetwidth > 500){
            log_debug("Websocket解析结果：前置相机参数-标靶宽度[值域:0-500]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"前置相机参数-标靶宽度[值域:0-500]\"}");
            return;
        }
        paramGroup.append(targetwidth);
        //前置相机参数-标靶倾角	-15~45
        float targetdipangle = valueContent.value("targetdipangle").toString().toFloat();
        if(targetdipangle < -15 || targetdipangle > 45){
            log_debug("Websocket解析结果：前置相机参数-标靶倾角不正确[值域:-15-45]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"前置相机参数-标靶倾角不正确[值域:-15-45]\"}");
            return;
        }
        paramGroup.append(targetdipangle);
        //后置相机参数-后置相机横向	0-15000
        float rearcameratransverse = valueContent.value("rearcameratransverse").toString().toFloat();
        if(rearcameratransverse < 0 || rearcameratransverse > 15000){
            log_debug("Websocket解析结果：后置相机参数-后置相机横向不正确[值域:0-15000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"后置相机参数-后置相机横向不正确[值域:0-15000]\"}");
            return;
        }
        paramGroup.append(rearcameratransverse);
        //后置相机参数-后置相机轴向	0-15000
        float rearcameraaxial = valueContent.value("rearcameraaxial").toString().toFloat();
        if(rearcameraaxial < 0 || rearcameraaxial > 15000){
            log_debug("Websocket解析结果：后置相机参数-后置相机轴向不正确[值域:0-15000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"后置相机参数-后置相机轴向不正确[值域:0-15000]\"}");
            return;
        }
        paramGroup.append(rearcameraaxial);
        //激光指向仪-激光指向仪间距	0-1500
        float laserpointerspace = valueContent.value("laserpointerspace").toString().toFloat();
        if(laserpointerspace < 0 || laserpointerspace > 1500){
            log_debug("Websocket解析结果：激光指向仪-激光指向仪间距不正确[值域:0-1500]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪-激光指向仪间距不正确[值域:0-1500]\"}");
            return;
        }
        paramGroup.append(laserpointerspace);
        //激光指向仪-激光指向仪长度	0-1500
        float laserpointerlength = valueContent.value("laserpointerlength").toString().toFloat();
        if(laserpointerlength < 0 || laserpointerlength > 1500){
            log_debug("Websocket解析结果：激光指向仪-激光指向仪长度不正确[值域:0-1500]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪-激光指向仪长度不正确[值域:0-1500]\"}");
            return;
        }
        paramGroup.append(laserpointerlength);
        //激光指向仪A点X坐标	0-10000000000000
        float laserpointerax = valueContent.value("laserpointerax").toString().toFloat();
        if(laserpointerax < 0 || laserpointerax > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪A点X坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪A点X坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerax);
        //激光指向仪A点y坐标	0-10000000000000
        float laserpointeray = valueContent.value("laserpointeray").toString().toFloat();
        if(laserpointeray < 0 || laserpointeray > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪A点Y坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪A点Y坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointeray);
        //激光指向仪A点z坐标	0-10000000000000
        float laserpointeraz = valueContent.value("laserpointeraz").toString().toFloat();
        if(laserpointeraz < 0 || laserpointeraz > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪A点Z坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪A点Z坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointeraz);
        //激光指向仪B点X坐标	0-10000000000000
        float laserpointerbx = valueContent.value("laserpointerbx").toString().toFloat();
        if(laserpointerbx < 0 || laserpointerbx > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪B点X坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪B点X坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerbx);
        //激光指向仪B点y坐标	0-10000000000000
        float laserpointerby = valueContent.value("laserpointerby").toString().toFloat();
        if(laserpointerby < 0 || laserpointerby > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪B点Y坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪B点Y坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerby);
        //激光指向仪B点z坐标	0-10000000000000
        float laserpointerbz = valueContent.value("laserpointerbz").toString().toFloat();
        if(laserpointerbz < 0 || laserpointerbz > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪B点Z坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪B点Z坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerbz);
        //激光指向仪C点X坐标	0-10000000000000
        float laserpointercx = valueContent.value("laserpointercx").toString().toFloat();
        if(laserpointercx < 0 || laserpointercx > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪C点X坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪C点X坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointercx);
        //激光指向仪C点y坐标	0-10000000000000
        float laserpointercy = valueContent.value("laserpointercy").toString().toFloat();
        if(laserpointercy < 0 || laserpointercy > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪C点Y坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪C点Y坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointercy);
        //激光指向仪C点z坐标	0-10000000000000
        float laserpointercz = valueContent.value("laserpointercz").toString().toFloat();
        if(laserpointercz < 0 || laserpointercz > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪C点Z坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪C点Z坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointercz);
        //激光指向仪A1点X坐标	0-10000000000000
        float laserpointera1x = valueContent.value("laserpointera1x").toString().toFloat();
        if(laserpointera1x < 0 || laserpointera1x > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪A1点X坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪A1点X坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointera1x);
        //激光指向仪A1点y坐标	0-10000000000000
        float laserpointera1y = valueContent.value("laserpointera1y").toString().toFloat();
        if(laserpointera1y < 0 || laserpointera1y > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪A1点Y坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪A1点Y坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointera1y);
        //激光指向仪A1点z坐标	0-10000000000000
        float laserpointera1z = valueContent.value("laserpointera1z").toString().toFloat();
        if(laserpointera1z < 0 || laserpointera1z > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪A1点Z坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪A1点Z坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointera1z);
        //激光指向仪B1点X坐标	0-10000000000000
        float laserpointerb1x = valueContent.value("laserpointerb1x").toString().toFloat();
        if(laserpointerb1x < 0 || laserpointerb1x > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪B1点X坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪B1点X坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerb1x);
        //激光指向仪B1点y坐标	0-10000000000000
        float laserpointerb1y = valueContent.value("laserpointerb1y").toString().toFloat();
        if(laserpointerb1y < 0 || laserpointerb1y > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪B1点Y坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪B1点Y坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerb1y);
        //激光指向仪B1点z坐标	0-10000000000000
        float laserpointerb1z = valueContent.value("laserpointerb1z").toString().toFloat();
        if(laserpointerb1z < 0 || laserpointerb1z > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪B1点Z坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪B1点Z坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerb1z);
        //激光指向仪C1点X坐标	0-10000000000000
        float laserpointerc1x = valueContent.value("laserpointerc1x").toString().toFloat();
        if(laserpointerc1x < 0 || laserpointerc1x > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪C1点X坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪C1点X坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerc1x);
        //激光指向仪C1点y坐标	0-10000000000000
        float laserpointerc1y = valueContent.value("laserpointerc1y").toString().toFloat();
        if(laserpointerc1y < 0 || laserpointerc1y > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪C1点Y坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪C1点Y坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerc1y);
        //激光指向仪C1点z坐标	0-10000000000000
        float laserpointerc1z = valueContent.value("laserpointerc1z").toString().toFloat();
        if(laserpointerc1z < 0 || laserpointerc1z > 10000000000000){
            log_debug("Websocket解析结果：激光指向仪C1点Z坐标不正确[值域:0-10000000000000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"激光指向仪C1点Z坐标不正确[值域:0-10000000000000]\"}");
            return;
        }
        paramGroup.append(laserpointerc1z);
        //雷达-雷达安装偏差左	0-1000
        float deviationleft = valueContent.value("deviationleft").toString().toFloat();
        if(deviationleft < 0 || deviationleft > 1000){
            log_debug("Websocket解析结果：雷达-雷达安装偏差左不正确[值域:0-1000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"雷达-雷达安装偏差左不正确[值域:0-1000]\"}");
            return;
        }
        paramGroup.append(deviationleft);
        //雷达-雷达安装偏差右	0-1000
        float deviationright = valueContent.value("deviationright").toString().toFloat();
        if(deviationright < 0 || deviationright > 1000){
            log_debug("Websocket解析结果：雷达-雷达安装偏差右不正确[值域:0-1000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"雷达-雷达安装偏差右不正确[值域:0-1000]\"}");
            return;
        }
        paramGroup.append(deviationright);
        //截割臂回转、抬升参数-截割臂水平时抬升液压缸长度	0-5000
        float horizontallength = valueContent.value("horizontallength").toString().toFloat();
        if(horizontallength < 0 || horizontallength > 5000){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-截割臂水平时抬升液压缸长度不正确[值域:0-5000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-截割臂水平时抬升液压缸长度不正确[值域:0-5000]\"}");
            return;
        }
        paramGroup.append(horizontallength);
        //截割臂回转、抬升参数-截割臂中线时回转液压缸长度	0-5000
        float centerlength = valueContent.value("centerlength").toString().toFloat();
        if(centerlength < 0 || centerlength > 5000){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-截割臂中线时回转液压缸长度不正确[值域:0-5000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-截割臂中线时回转液压缸长度不正确[值域:0-5000]\"}");
            return;
        }
        paramGroup.append(centerlength);
        //截割臂回转、抬升参数-液压缸根部到抬升关节距离	0-5000
        float rootdistance = valueContent.value("rootdistance").toString().toFloat();
        if(rootdistance < 0 || rootdistance > 5000){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-液压缸根部到抬升关节距离不正确[值域:0-5000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-液压缸根部到抬升关节距离不正确[值域:0-5000]\"}");
            return;
        }
        paramGroup.append(rootdistance);
        //截割臂回转、抬升参数-液压缸根部到回转台中心	0-5000
        float rotarydistance = valueContent.value("rotarydistance").toString().toFloat();
        if(rotarydistance < 0 || rotarydistance > 5000){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-液压缸根部到回转台中心不正确[值域:0-5000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-液压缸根部到回转台中心不正确[值域:0-5000]\"}");
            return;
        }
        paramGroup.append(rotarydistance);
        //截割臂回转、抬升参数-液压缸顶部到抬升关节距离	0-5000
        float topdistance = valueContent.value("topdistance").toString().toFloat();
        if(topdistance < 0 || topdistance > 5000){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-液压缸顶部到抬升关节距离不正确[值域:0-5000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-液压缸顶部到抬升关节距离不正确[值域:0-5000]\"}");
            return;
        }
        paramGroup.append(topdistance);
        //截割臂回转、抬升参数-回转台半径	0-5000
        float rotarytableradius = valueContent.value("rotarytableradius").toString().toFloat();
        if(rotarytableradius < 0 || rotarytableradius > 5000){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-回转台半径不正确[值域:0-5000]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-回转台半径不正确[值域:0-5000]\"}");
            return;
        }
        paramGroup.append(rotarytableradius);
        //截割臂回转、抬升参数-抬升初始夹角	0-90°
        float upliftinitialangle = valueContent.value("upliftinitialangle").toString().toFloat();
        if(upliftinitialangle < 0 || upliftinitialangle > 90){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-抬升初始夹角不正确[值域:0-90]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-抬升初始夹角不正确[值域:0-90]\"}");
            return;
        }
        paramGroup.append(upliftinitialangle);
        //截割臂回转、抬升参数-回转初始夹角	0-90°
        float rotaryinitialangle = valueContent.value("rotaryinitialangle").toString().toFloat();
        if(rotaryinitialangle < 0 || rotaryinitialangle > 90){
            log_debug("Websocket解析结果：截割臂回转、抬升参数-回转初始夹角不正确[值域:0-90]");
            slot_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"截割臂回转、抬升参数-回转初始夹角不正确[值域:0-90]\"}");
            return;
        }
        paramGroup.append(rotaryinitialangle);
        //将取得的参数封装成TCP报文
        QString hexStr = "AABB";
        //将各参数转为FLOAT AB CD
        for(int i = 0; i < paramGroup.size(); i++){
            float tmpFloat = paramGroup.at(i);
            uint b = *((uint *)&tmpFloat);
            QString floatStr = QString("%1").arg(b, 4, 16, QLatin1Char('0')).toUpper();
            /*这里是在加空格
            int nLength = floatStr.length();
            while(nLength - 2 > 0){
                nLength = nLength - 2;
                floatStr.insert(nLength," ");
            }
            */
            //char hexs[128];
            //sprintf(hexs,"%08x",tmpFloat);
            //hexStr = hexStr + " " + QString(hexs);
            hexStr += floatStr;
        }
        //结束符
        hexStr = hexStr + "BF";
        qDebug()<<"hexStr:"<<hexStr;

        std::string str = hexStr.toStdString();
        //
        char *str_ascii = (char *)hexStr.toLatin1().constData();
        int ascii_len =hexStr.length();
        int bin_len = ascii_len/2;

    //    log_debug("ascii_len=%d bin_len=%d",ascii_len,bin_len);

        unsigned char frame[1024];
        hexs_to_binary(str_ascii,ascii_len,frame);

        printf_hex(frame,bin_len);

        //将报文下发给位姿系统客户端
        for(size_t i = 0; i < controlMain->xly_cliens.size(); ++i)
        {


            tcp_client_send((controlMain->xly_cliens[i]),(char *)frame,bin_len);
        }
        //给WEBSOCKET客户端推送消息，没有了呢
        nomal_broadcast_msg("{\"version\":\"1.0\",\"method\":\"paramset\",\"result\":\"参数设置发送完毕\"}");
    }

}

//连接断开
void WebSocket::socketDisconnected()
{
    qDebug()<<__FILE__<<__LINE__<<"socketDisconnected.";
}

//给客户端发送消息
void WebSocket::sendMessage(QString message)
{
    qDebug()<<__FILE__<<__LINE__<<"sendMessage.";
}

//给客户端发送消息
void WebSocket::nomal_broadcast_msg(QString message)
{
    qDebug()<<__FILE__<<__LINE__<<"sendMessage.";
    for (auto socket:m_clients) {
        socket->sendTextMessage(message);
    }
}

//推送消息给客户端
void WebSocket::slot_broadcast_msg(QByteArray content)
{
    int len = content.size();
    char json_buf[1024];
    memset(json_buf,'\0',sizeof(json_buf));
log_debug("QByteArray =%d",len);

    if(len>=104){
        QString message = QString(content);
        message.remove(QRegExp("\\s"));

        char *str_ascii = message.toLatin1().data();
        int ascii_len =message.toLatin1().length();
        int bin_len = ascii_len/2;

        log_debug("ascii_len=%d bin_len=%d",ascii_len,bin_len);

        unsigned char frame[1024];
        hexs_to_binary(str_ascii,ascii_len,frame);

        printf_hex(frame,bin_len);

        conver_request_xly_to_frame((const char*)frame,bin_len,json_buf);
    }else{ //len==52
        char *frame = (char *)content.data();
        printf_hex((unsigned char*)frame,content.length());
        log_debug("buf len=%d %d",content.length(),content.size());
        log_debug("ascii_len");
        conver_request_xly_to_frame((const char*)frame,content.length(),json_buf);
    }

    log_debug("tcp_client_send %s",json_buf);
    //推送消息给客户端
    for (auto socket:m_clients) {
        socket->sendTextMessage(json_buf);
//        qDebug()<<__FILE__<<__LINE__<<__FUNCTION__;
//        socket->sendBinaryMessage(content);
//        socket->sendBinaryMessage("\n");
//        socket->flush();
    }
}
