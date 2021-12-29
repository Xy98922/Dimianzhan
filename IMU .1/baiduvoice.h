#ifndef BAIDUVOICE_H
#define BAIDUVOICE_H
//语音识别 语音合成两大功能
//先语音识别，再语音合成
#include<QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include<QUrl>
#include <QtNetwork>
class baiduVoice
{
public:
    baiduVoice();
    //这里要填入百度语音apikey secretkey
         const QString API_KEY="1YtgeUMAp110tbqvL6K0L7tO";
         const QString Secret_Key="qLlwTUBQHCVttG1FYl3c7ckfC51ruunI";
         QString Access_Token="25.0dd82db6dbb497943ea10967f148e12a.315360000.1946972054.282335-24849030";
        //vedio to text
        const QString VOP_URL="http://vop.baidu.com/server_api";
        //Text to vedio
        const QString TSN_URL="http://tsn.baidu.com/text2audio";
        QString Get_Token_URL="https://aip.baidubce.com/oauth/2.0/token";
        QString MAC_cuid="4C-ED-FB-6E-89-81";
};

#endif // BAIDUVOICE_H

