#pragma execution_character_set("utf-8")

#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JSONUtil
{
public:
    JSONUtil(){}

    static QString jsonToStr(QJsonObject jsonObj);
};

#endif // JSONUTIL_H
