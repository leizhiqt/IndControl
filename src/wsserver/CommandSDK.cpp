#include "Command.h"
#include "ControlMain.h"
#include "DaHuaFactory.h"

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#pragma execution_character_set("utf-8")

Command::Command()
{
    listProcess = new QProcess(this);
    listProcess->setProcessChannelMode(QProcess::MergedChannels);
    listProcess->setReadChannelMode(QProcess::MergedChannels);

    daHuaProduct=DaHuaFactory::CreateProduct(DAHUAPRODUCT::DAHUA_SDK);

    connect(listProcess, SIGNAL(finished()), this, SLOT(onFinished()));//进程结束时
    connect(listProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));//进程标准输出
}

Command::~Command()
{
    delete listProcess;
}

void Command::readOutput()
{
//    QString s = listProcess->readAllStandardOutput();
//    qDebug() << s;
}

void Command::onFinished()
{
    qDebug() << "onFinished";
}

void Command::processCommand(QString message){
    processTextMessage(message);
}

void Command::dahua_json_login(QJsonObject jo,DaHuaControll *daHuaControll){
    QJsonValue value = jo.value("params");
    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_Login";
        return;
    }

    QJsonObject jo2 = value.toObject();
    QString ip = jo2.value("ip").toString();
    int port = jo2.value("port").toInt();
    QString user = jo2.value("user").toString();
    QString password = jo2.value("password").toString();

    qDebug() << ip << port << user << password;
    daHuaControll->Login(ip,port,user,password);

    return;
}

void Command::dahua_json_loadgroupinfo(QJsonObject jo,DaHuaControll *daHuaControll,BDServer *dbServer){
    daHuaControll->LoadGroupInfo();
//    QJsonArray devlst = daHuaControll->getDevs();
//    QJsonObject json;
//    json.insert("devlst",devlst);
//    QJsonDocument document;
//    document.setObject(json);
//    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
//    QString strjson(bytearray);
//    dbServer->sendMessage(strjson);
}

void Command::dahua_json_startrealplay(QJsonObject jo,DaHuaControll *daHuaControll,
                                       MainWindow *mWin){
    int ret = -1;

    QJsonValue value = jo.value("params");
    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_startrealplay";
        return;
    }

    if(value.isObject())
    {
        QJsonObject jo2 = value.toObject();
        int index = jo2.value("index").toInt();
        QString cameraid = jo2.value("cameraid").toString();
        qDebug() << "cameraid: " << cameraid << "index: " << index;

        HWND hwnd=mWin->qtScreen->getHwnd(index);
        if(hwnd !=nullptr){
            ret=daHuaControll->OpenVideo(cameraid,hwnd);
            mWin->qtScreen->setSeq(index,daHuaControll->m_nSeq);
            qDebug() << "OpenVideo ret: " << ret << index << cameraid;
        }
        return;
    }

    if (value.isArray()) {
        QJsonArray ar = value.toArray();
        for (int i = 0;i < ar.size(); i++) {
            QJsonValue v2 = ar.at(i);
            QJsonObject jo2 = v2.toObject();
            int index = jo2.value("index").toInt();
            QString cameraid = jo2.value("cameraid").toString();

            HWND hwnd=mWin->qtScreen->getHwnd(index);
            if(hwnd !=nullptr){
                ret=daHuaControll->OpenVideo(cameraid,hwnd);
                mWin->qtScreen->setSeq(index,daHuaControll->m_nSeq);
                qDebug() << "OpenVideo ret: " << ret << index << cameraid;
            }
        }
        return;
    }
    return;
}

void Command::dahua_json_stoprealplay(QJsonObject jo,DaHuaControll *daHuaControll,MainWindow *mWin){
//    int ret = -1;
    QJsonValue value = jo.value("params");
    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_stoprealplay";
        return;
    }

    if(value.isObject())
    {
        QJsonObject jo2 = value.toObject();
        int index = jo2.value("index").toInt();
        int seq = -1;
        if((seq=mWin->qtScreen->getSeq(index))>0){
            daHuaControll->CloseVideo(seq);
        }
        qDebug() << index << endl;
        return;
    }

    if (value.isArray()){
        QJsonArray ar = value.toArray();
        for (int i = 0;i < ar.size(); i++) {
            QJsonValue v2 = ar.at(i);
            QJsonObject jo2 = v2.toObject();
            int index = jo2.value("index").toInt();
            int seq = -1;
            if((seq=mWin->qtScreen->getSeq(index))>0){
                daHuaControll->CloseVideo(seq);
            }
            qDebug() << index << endl;
        }
        return;
    }
    return;
}

//截图
//返回base64 的图片数据
QString Command::dahua_save_img(DaHuaControll *daHuaControll,int seq,QString filename){
    //QString filepath = QCoreApplication::applicationDirPath() + "/capture/" + filename;

    QString filePath = "capture/";
    QDir tempDir;
    if(!tempDir.exists(filePath))
    {
        tempDir.mkpath(filePath);
    }
    QString filepath = filePath + filename;

    int ret = daHuaControll->Capture(seq,filepath);

    //filepath.replace();
    //QString str2 = filepath.replace(QRegExp("/"), "\\");
    qDebug() << __FILE__<< __LINE__ << seq << "path: " << filepath <<" Capture:"<<ret;

    QImage* img=new QImage;

    if(! ( img->load(filepath) ) ) {
            delete  img;
            qDebug() << __FILE__<< __LINE__ << filepath << "load error";
            return NULL;
    }

    QByteArray ba;
    QBuffer buf(&ba);

    buf.open(QIODevice::WriteOnly);
    img->save(&buf, "PNG",20);

    QByteArray ba2 = ba.toBase64();
    QString b64str = QString::fromLatin1(ba2);

    delete  img;
    buf.close();

    //qDebug() << __FILE__<< __LINE__<< "base64>>>" << b64str<< "<<<base64" << endl;
    return b64str;
}

void Command::dahua_json_capture(QJsonObject jo,DaHuaControll *daHuaControll,
                                 BDServer *dbServer,MainWindow *mWin){

    QJsonObject json;

    QJsonArray imagelst;
    QJsonValue value = jo.value("params");

    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_capture";
        return;
    }

    if(value.isObject())
    {
        QJsonObject jo2 = value.toObject();
        int index = jo2.value("index").toInt();
        QString filename = jo2.value("filename").toString();

        int seq = -1;
        if((seq=mWin->qtScreen->getSeq(index))>-1)
        {
           QString str=dahua_save_img(daHuaControll,seq,filename);
            if(str!=NULL){
                imagelst.append(str);
            }
        }
        json.insert("imagelst",imagelst);

        dbServer->sendMessage(JSONUtil::jsonToStr(json));
        return;
    }

    if (value.isArray()){
        QJsonArray ar = value.toArray();
        for (int i = 0;i < ar.size(); i++) {
            QJsonValue v2 = ar.at(i);
            QJsonObject jo2 = v2.toObject();
            int index = jo2.value("index").toInt();
            QString filename = jo2.value("filename").toString();

            int seq = -1;
            if((seq=mWin->qtScreen->getSeq(index))>-1)
            {
                QString b64str=dahua_save_img(daHuaControll,seq,filename);
                 if(b64str!=NULL){
                     imagelst.append(b64str);
                 }
            }
        }
        json.insert("imagelst",imagelst);
        dbServer->sendMessage(JSONUtil::jsonToStr(json));
        return;
    }
    return;
}

void Command::dahua_json_setwindowpos(QJsonObject jo,MainWindow *mWin){

    QJsonValue value = jo.value("params");
    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_setwindowpos";
        return;
    }

    QJsonObject jo2 = value.toObject();
    int x = jo2.value("x").toInt();
    int y = jo2.value("y").toInt();
    int width = jo2.value("width").toInt();
    int height = jo2.value("height").toInt();

    mWin->SetDisplay(x,y,width,height);

    qDebug() <<"setwindowpos:"<< x << y << width << height;

}

void Command::dahua_json_setsplitscreen(QJsonObject jo,MainWindow *mWin){

    QJsonValue value = jo.value("params");
    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_setsplitscreen";
        return;
    }

    QJsonObject jo2 = value.toObject();
    int mode = jo2.value("mode").toInt();

    mWin->qtScreen->update_screen(mode);

    qDebug() <<"SetSplitScreen:"<< mode;

}

void Command::dahua_json_setwindowvisble(QJsonObject jo,MainWindow *mWin){
    QJsonValue value = jo.value("params");

    if(value==NULL || (!value.isObject() && !value.isArray()))
    {
        qDebug() << "p_setwindowvisble";
        return;
    }

    QJsonObject jo2 = value.toObject();
    int mode = jo2.value("flag").toInt();

    if(mode)
    {
        mWin->show();
        return;
    }

    mWin->hide();
}

//云台控制
void Command::dahua_json_ptdc(QJsonObject jo,DaHuaProduct *daHuaProduct){
//    int ret = -1;
    QJsonValue params = jo.value("params");
    if(params==NULL || !params.isArray())
    {
        qDebug() << "p_ptdc error";
        return;
    }

//    if(params.isObject())
//    {
//        QJsonObject paramsObj = params.toObject();
//        qDebug() << "isObject";

//        //Params
////        int index = paramsObj.value("index").toInt();
//        QString cameraid = paramsObj.value("cameraid").toString();
//        QString ptd = paramsObj.value("ptd").toString();
//        QString onout = paramsObj.value("onout").toString();

//        qDebug() << cameraid << ptd << onout;
//        dpsdk_ptz_direct_e e;
//        if(ptd.contains("DPSDK_CORE_PTZ_GO_RIGHTUP")){
//            e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTUP;
//        }else if(ptd.contains("DPSDK_CORE_PTZ_GO_DOWN")){
//            e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_DOWN;
//        }else if(ptd.contains("DPSDK_CORE_PTZ_GO_LEFT")){
//            e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFT;
//        }else if(ptd.contains("DPSDK_CORE_PTZ_GO_RIGHT")){
//            e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHT;
//        }

//        bool nstop=true;
//        if(onout.contains("ont")){
//            nstop=false;
//        }

//        qDebug() << cameraid << e << nstop;

//        daHuaControll->pt_direction_control(cameraid,e,nstop);
//        qDebug() << "isObject";

//        return;
//    }

    if (params.isArray()){
        QJsonArray qjArray = params.toArray();
        for (int i = 0;i < qjArray.size(); i++) {
            QJsonValue arrayVa = qjArray.at(i);
            QJsonObject paramsObj = arrayVa.toObject();

            //Params
            QString cameraid = paramsObj.value("cameraid").toString();
            QString ptd = paramsObj.value("ptd").toString();
            QString onout = paramsObj.value("onout").toString();

            qDebug() << cameraid << ptd << onout;
            dpsdk_ptz_direct_e e;
                e=(dpsdk_ptz_direct_e)-1;
            if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_UP")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_UP;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_DOWN")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_DOWN;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_LEFT")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFT;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_RIGHT")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHT;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_LEFTUP")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFTUP;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_LEFTDOWN")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_LEFTDOWN;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_RIGHTUP")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTUP;
            }else if(ptd==QString::fromLocal8Bit("DPSDK_CORE_PTZ_GO_RIGHTDOWN")){
                e = dpsdk_ptz_direct_e::DPSDK_CORE_PTZ_GO_RIGHTDOWN;
            }

            bool nstop=false;
            if(onout==QString::fromLocal8Bit("out")){
                nstop=true;
            }

            qDebug() << cameraid << (int)e << nstop;
            daHuaProduct->pt_direction_control(cameraid,e,nstop);
            qDebug() << "pt_direction_control end";

            return;
        }
        return;
    }
    qDebug() << "isObject";

    return;
}

//云台控制
void Command::dahua_json_ptcc(QJsonObject jo,DaHuaProduct *daHuaProduct){
//    int ret = -1;
    QJsonValue params = jo.value("params");
    if(params==NULL || !params.isArray())
    {
        qDebug() << "p_ptdc error";
        return;
    }

    if (params.isArray()){
        QJsonArray qjArray = params.toArray();
        for (int i = 0;i < qjArray.size(); i++) {
            QJsonValue arrayVa = qjArray.at(i);
            QJsonObject paramsObj = arrayVa.toObject();

            //Params
            QString cameraid = paramsObj.value("cameraid").toString();
            QString zoom = paramsObj.value("zoom").toString();
            QString onout = paramsObj.value("onout").toString();

            qDebug() << cameraid << zoom << onout;
            dpsdk_camera_operation_e e;
                e=(dpsdk_camera_operation_e)-1;
            if(zoom==QString::fromLocal8Bit("DPSDK_CORE_PTZ_ADD_ZOOM")){
                e = dpsdk_camera_operation_e::DPSDK_CORE_PTZ_ADD_ZOOM;
            }else if(zoom==QString::fromLocal8Bit("DPSDK_CORE_PTZ_REDUCE_ZOOM")){
                e = dpsdk_camera_operation_e::DPSDK_CORE_PTZ_REDUCE_ZOOM;
            }

            bool nstop=false;
            if(onout==QString::fromLocal8Bit("out")){
                nstop=true;
            }

            qDebug() << cameraid << (int)e << nstop;
            daHuaProduct->pt_camera_control(cameraid,e,nstop);
            qDebug() << "pt_direction_control end";

            return;
        }
        return;
    }
    return;
}

void Command::processTextMessage(QString message)
{
//    int ret = -1;
    MainWindow *mWin =controlMain->mWin;

    DaHuaControll *daHuaControll=controlMain->daHuaControll;
    qDebug()<< "daHuaControll: "<<daHuaControll;

    BDServer *dbServer=controlMain->dbServer;
    qDebug()<< "dbServer: "<<dbServer;

    QByteArray bytes =message.toLatin1();
//    const char* p_msg = bytes.data();
//    qDebug()<< "processTextMessage: "<<p_msg;

    QJsonDocument jd = QJsonDocument::fromJson(bytes);
    if(jd.isNull() || (!jd.isObject() && !jd.isArray()))
    {
        qDebug() << "processTextMessage";
        return;
    }

    QJsonObject jo = jd.object();
    QJsonValue method = jo.value("method");
    qDebug() << method.toString();

    if(method.toString() == "login")
    {
        dahua_json_login(jo,daHuaControll);
        return;
    }

    if (method.toString() == "loginout") {
        daHuaControll->Logout();
        return;
    }

    if (method.toString() == "loadgroupinfo") {
        dahua_json_loadgroupinfo(jo,daHuaControll,dbServer);
        return;
    }

    if (method.toString() == "startrealplay") {
        dahua_json_startrealplay(jo,daHuaControll,mWin);
        return;
    }

    if (method.toString() == "stoprealplay") {
        dahua_json_stoprealplay(jo,daHuaControll,mWin);
        return;
    }

    if (method.toString() == "setwindowpos") {
        dahua_json_setwindowpos(jo,mWin);
        return;
    }

    if (method.toString() == "setsplitscreen") {
        dahua_json_setsplitscreen(jo,mWin);
        return;
    }

    if (method.toString() == "setwindowvisble") {
        dahua_json_setwindowvisble(jo,mWin);
        return;
    }

    if (method.toString() == "capture") {
        dahua_json_capture(jo,daHuaControll,dbServer,mWin);
        return;
    }

    if (method.toString() == "pt_direction_control") {
        qDebug() << "pt_direction_control";
        dahua_json_ptdc(jo,daHuaProduct);
        return;
    }

    if (method.toString() == "pt_camera_control") {
        qDebug() << "pt_camera_control";
        dahua_json_ptcc(jo,daHuaProduct);
        return;
    }

    if (method.toString() == "addon_start_screencap") {
        qDebug() << "addon_start_screencap.";
        //QString execArgs="./tools/ffmpeg.exe -y -f gdigrab -i desktop -f mp4 d:\\1.mp4";
        //QString execArgs="./tools/ffmpeg.exe -y -f dshow -i desktop  -pix_fmt yuv420p -vcodec libx264 -acodec aac -s 1440x900 -r 25 -q 10 -ar 44100 -ac 2 -tune zerolatency -preset ultrafast -f mp4 d:\\1.mp4";

        //connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));//用于更新时间
        QJsonValue params = jo.value("params");
        QJsonObject parObj = params.toObject();
        QString save_path=parObj.value("save_path").toString();

        QString execArgs="./tools/ffmpeg.exe -y -f gdigrab -i desktop -f mp4 "+save_path;
        //execArgs="./tools/ffmpeg.exe -y -f gdigrab -i desktop -f mp4 d:/1.mp4";
        qDebug() << __FILE__<< __LINE__ << execArgs;

        listProcess->start(execArgs);
        listProcess->waitForStarted();
        qDebug() << __FILE__<< __LINE__ << execArgs;
       // program.start(execArgs);
       // program.waitForStarted();
        return;
    }

    if (method.toString() == "addon_stop_screencap") {
        qDebug() << "addon_stop_screencap.";
        if(listProcess->isOpen()){
            listProcess->write("q");
        }
        return;
    }
    //dbServer->sendMessage("OK");
}
