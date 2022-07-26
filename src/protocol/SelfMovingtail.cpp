#include "SelfMovingtail.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "ConvertUtil.h"
#include "UWLog.h"
#include <QDebug>

void conver_selfmovetail_to_json(const char *frame_buf,const int len, char *str_json)
{
    selfmoveingtail_t *p;
    char hexs[1024];
    if(len>=sizeof(selfmoveingtail_t)){
       p=(selfmoveingtail_t *)frame_buf;

       QJsonObject selfmoveingtail_json;
       selfmoveingtail_json.insert("version", "1.0");
       selfmoveingtail_json.insert("method", "selfmovedata");

       QJsonObject parmsSelf;

//       to16_hexs(hexs,p->r_40003);
       sprintf(hexs,"%02x %02x",p->r_40003,p->r_40003[1]);
       parmsSelf.insert("风机启动标识",GET_BIT(p->r_40003[0],0));
       parmsSelf.insert("泵站启动标识",GET_BIT(p->r_40003[0],1));
       parmsSelf.insert("整机运行标识",GET_BIT(p->r_40003[0],2));
       parmsSelf.insert("油缸动作到位标识",GET_BIT(p->r_40003[0],3));
       parmsSelf.insert("沿线急停",GET_BIT(p->r_40003[0],4));
       parmsSelf.insert("本地模式",GET_BIT(p->r_40003[0],5));
       parmsSelf.insert("遥控模式",GET_BIT(p->r_40003[0],6));
       parmsSelf.insert("集控模式",GET_BIT(p->r_40003[0],7));
       parmsSelf.insert("油位低",GET_BIT(p->r_40003[1],0));

       sprintf(hexs,"%d",hl_to_int16(p->r_40004[0],p->r_40004[1]));
       parmsSelf.insert("主泵压力",hexs);

       sprintf(hexs,"%d",hl_to_int16(p->r_40005[1],p->r_40005[0]));
       parmsSelf.insert("副泵压力",hexs);

       sprintf(hexs,"%d",hl_to_int16(p->r_40006[1],p->r_40006[0]));
       parmsSelf.insert("油温",hexs);

       sprintf(hexs,"%d",hl_to_int16(p->r_40007[1],p->r_40007[0]));
       parmsSelf.insert("泵站电机温度",hexs);

       sprintf(hexs,"%d",hl_to_int16(p->r_40008[1],p->r_40008[0]));
       parmsSelf.insert("风机电机温度",hexs);

       sprintf(hexs,"%d",hl_to_int16(p->r_40009[1],p->r_40009[0]));
       parmsSelf.insert("激光测距",hexs);

        sprintf(hexs,"%02x %02x",p->r_40010,p->r_40010[1]);
       parmsSelf.insert("左调偏动作标记",GET_BIT(p->r_40010[0],0));
       parmsSelf.insert("右调偏动作标记",GET_BIT(p->r_40010[0],1));
       parmsSelf.insert("左推移伸动作标记",GET_BIT(p->r_40010[0],2));
       parmsSelf.insert("左推移收动作标记",GET_BIT(p->r_40010[0],3));
       parmsSelf.insert("右推移伸动作标记",GET_BIT(p->r_40010[0],4));
       parmsSelf.insert("右推移收动作标记",GET_BIT(p->r_40010[0],5));
       parmsSelf.insert("调偏抬高伸动作标记",GET_BIT(p->r_40010[0],6));
       parmsSelf.insert("调偏抬高收动作标记",GET_BIT(p->r_40010[0],7));
       parmsSelf.insert("抬高伸动作标记",GET_BIT(p->r_40010[1],0));
       parmsSelf.insert("抬高收动作标记",GET_BIT(p->r_40010[1],1));
       parmsSelf.insert("挑带动作标记",GET_BIT(p->r_40010[1],2));
       parmsSelf.insert("下放动作标记",GET_BIT(p->r_40010[1],3));
       parmsSelf.insert("二运支撑伸动作标记",GET_BIT(p->r_40010[1],4));
       parmsSelf.insert("二运支撑收动作标记",GET_BIT(p->r_40010[1],5));

       QJsonDocument doc(parmsSelf);
       QString parms_str = QString(doc.toJson(QJsonDocument::Indented));
       selfmoveingtail_json.insert("params", parmsSelf);
       QJsonDocument document;
       document.setObject(selfmoveingtail_json);
       QByteArray bytes = document.toJson(QJsonDocument::Indented);
       char *str_p =bytes.begin();
       memcpy(str_json,str_p,bytes.size());
    }
}
