#include "wlanChoice.h"
#include "globalvarible.h"
#include <QDebug>
#include <QFile>

wlanChoice::wlanChoice(QObject *parent) : QObject(parent)
{
}

/*******************************************************/
//获取scan的网络在APLIST.txt对应的IP
/*******************************************************/
void wlanChoice::getWlanSite(bool isFirst)
{
    bool isDuplicates = false;
    //获取需要忽略的AP的IP
    if(isFirst){//第一次切换，过滤当前在用的AP
        for(int li = 0;li<GLoseAPIPList.size();li++){
            if(GLoseAPIPList.at(li) == GCurrentSSID){
                isDuplicates = true;
                break;
            }
        }
        if(!GCurrentSSID.isEmpty() && !isDuplicates){
            GLoseAPIPList<<GCurrentSSID;
        }
    }else{
        //非第1次切换，即没切换成功，将该AP存入到GLoseAPIPList表中
        for(int li = 0;li<GLoseAPIPList.size();li++){
            if(GLoseAPIPList.at(li) == GSwitchSSID){
                isDuplicates = true;
                break;
            }
        }
        if(!GSwitchSSID.isEmpty() && !isDuplicates){
            GLoseAPIPList<<GSwitchSSID;
        }
    }
    int ssidCount = 0;
    bool isLoseIP = false;
    bool isForEnd = true;
    //遍历wifiInfo[]获取一个可用AP的位置
    for(int i = 0;!wifiInfo[i].wifiName.isEmpty();i++){
        //遍历allAP[]用BSSID来获取该AP的IP
        QString thisSwitchSSID = wifiInfo[i].wifiName;
        //查看该AP是否有在GLoseAPIPList有记录过
        for(int k = 0;k<GLoseAPIPList.size();k++){
            if(thisSwitchSSID == GLoseAPIPList.at(k)){
                isLoseIP = true;
                break;
            }
        }

        //看IP在GLoseAPIPList是否记录过，换下一个可用AP；没记录过可以用来连网
        if(isLoseIP){
            ssidCount++;
            isLoseIP = false;
        }else{
            //本次可用BSSID与上次不一样，即可更新wpa_supplicant_new.conf文件
            QString switchSSIDThis = wifiInfo[ssidCount].wifiName;
            if(switchSSIDThis != GSwitchSSID){
                replace_ssid(ssidCount);
                qDebug()<<"GLoseAPIPList:"<<GLoseAPIPList;
            }
            isForEnd = false;
            break;
        }
    }
    if(isForEnd){
        qDebug()<<"no usable wlan"<<ssidCount<<GLoseAPIPList;
    }
}


/*******************************************************/
//将当前可用AP的信息放到wpa_supplicant_new.conf中备用
/*******************************************************/
void wlanChoice::replace_ssid(int ssidCount)
{
    GSwitchSSID =wifiInfo[ssidCount].wifiName;
    GSwitchBSSID = wifiInfo[ssidCount].wifiAddress;
    int switchDBM = wifiInfo[ssidCount].wifidBm;
    qDebug()<<"wpa_supplicant:"<<GSwitchSSID<<GSwitchBSSID<<switchDBM;
    QString strAll;
    QStringList strList;
    //提取标准格式配置文件
    QFile readFile("/agvs/wpa/wpa_supplicant_bak.conf");
    if(readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        strAll = readFile.readAll();
        strAll = strAll.trimmed();
        strList = strAll.split("\n");
        readFile.close();
    }else{
        qDebug()<<"read wpa_supplicant_bak.conf error:"<<readFile.errorString();
    }

    //保存连网信息到配置文件
    QFile writeFile("/agvs/wpa/wpa_supplicant_new.conf");
    if(writeFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream in(&writeFile);
        QString tempLine;
        for(int i=0;i<strList.size();i++){
            tempLine=strList[i];
            int point = tempLine.indexOf("=");
            if(-1 < point){
                if(tempLine.left(point) == "ssid"){
                    tempLine.replace(6,tempLine.size()-7,GSwitchSSID);
                }
            }
            in<<tempLine<<endl;
        }
        writeFile.flush();
        writeFile.close();
        if(GSwitchSSID.size() > 0){
            GAllowSwitThread = true;
        }
    }else{
        qDebug()<<"Write wpa_supplicant_new.conf error:"<<writeFile.errorString();
    }
}
