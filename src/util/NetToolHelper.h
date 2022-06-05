#ifndef NETTOOLHELPER_H
#define NETTOOLHELPER_H
#include "qstring.h"

class NetToolHelper
{
public:
    NetToolHelper(){}
    static QString get_locale_ipv4(QString serverIP);
};

#endif // NETTOOLHELPER_H
