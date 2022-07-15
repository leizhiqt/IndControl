#pragma execution_character_set("utf-8")

#include "JSONUtil.h"


QString JSONUtil::jsonToStr(QJsonObject jsonObj){
    //JSON to String
    QJsonDocument document;
    document.setObject(jsonObj);

    QByteArray bytearray = document.toJson(QJsonDocument::Compact);
    QString json_str(bytearray);

    //qDebug() << __FILE__<< __LINE__<< "base64JSON>>>" << json_str<< "<<<base64JSON" << endl;
    return json_str;
}




