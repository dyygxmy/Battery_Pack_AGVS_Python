#include "iwlistValuation.h"
#include <QTimer>
#include <QTextStream>
#include "globalvarible.h"
#include <QDebug>
#include "wlanChoice.h"
#include <QMutex>
#include <QFile>

QString scanPath = "/agvs/myPython/data/scaning.conf";
iwlistValuation::iwlistValuation(QObject *parent) : QObject(parent)
{
    this->moveToThread(&iwlistThread);
    iwlistThread.start();
    GStrongestDBM = wifiInfo[0].wifidBm;
    noScanDebugFlag = true;
    getScanDebugFlag = true;
    scanTime = 0;
    isFirstON = true;
    qDebug()<<"iwlistValuation start";
}


/*******************************************************/
//提取scan的数据
/*******************************************************/
void iwlistValuation::get_scanningwifi_information()
{
    scanTime = 20000;
    QSettings* scanConf = new QSettings(scanPath,QSettings::IniFormat);
    QString scanInfo = scanConf->value("key1/scanData").toString();
    if(scanConf){
        delete scanConf;
    }
    if(scanInfo.size() > 0){
        scanDataProcessing(scanInfo);
    }else{
        qDebug()<<"scanData is NULL";
    }
    QTimer::singleShot(scanTime,this,SLOT(get_scanningwifi_information()));
}


/*******************************************************/
//排序（按dbm降序）
/*******************************************************/
void iwlistValuation::wifi_sort()
{
    for (int i=0;i<wifi_num;i++)
    {
        for(int j=i+1;j<wifi_num;j++)
        {
            if(wifiInfo[i].wifidBm<wifiInfo[j].wifidBm)
            {
                QString tempAdderss = wifiInfo[i].wifiAddress;
                wifiInfo[i].wifiAddress = wifiInfo[j].wifiAddress;
                wifiInfo[j].wifiAddress =tempAdderss;

                int temp = wifiInfo[i].wifidBm;
                wifiInfo[i].wifidBm = wifiInfo[j].wifidBm;
                wifiInfo[j].wifidBm = temp;

                QString tempstr = wifiInfo[i].wifiName;
                wifiInfo[i].wifiName = wifiInfo[j].wifiName;
                wifiInfo[j].wifiName=tempstr;
            }
        }
    }
}


/*******************************************************/
//处理iwlist scan返回的结果
/*******************************************************/
void iwlistValuation::scanDataProcessing(QString str_output)
{
    //qDebug()<<"str_output:"<<str_output;
    QStringList lineWifi;
    QString line = "";
    QString wifiAddress = "";
    int j = 0;
    if(str_output.contains("Signal level=") && str_output.contains(GSsidKey)) {
        str_output = str_output.trimmed();
        lineWifi = str_output.split('\n');
        QSettings* scanConf = new QSettings(scanPath,QSettings::IniFormat);
        GScanCycleID = scanConf->value("key1/cycle").toInt();
        if(scanConf){
            delete scanConf;
        }
        for(int i = 0;i<50;i++){
            wifiInfo[i].wifiAddress = "";
            wifiInfo[i].wifidBm = 0;
            wifiInfo[i].wifiName = "";
        }
        for(int i=2;i<lineWifi.size();i++){
            QString lineStr = lineWifi.at(i);
            lineStr = lineStr.trimmed();
            if(lineStr.contains(GSsidKey)){
                wifiAddress = lineWifi.at(i-2);// get address
                line=lineWifi.at(i-1);// get level
                wifiInfo[j].wifiAddress=wifiAddress.mid(wifiAddress.indexOf("Address:")+9,17);
                wifiInfo[j].wifidBm=line.mid(line.indexOf("Signal level=")+13,3).toInt();
                wifiInfo[j].wifiName=lineStr.remove("ESSID:").remove("\"");
                j++;
            }
        }
        wifi_num = j;
        wifi_sort();
        GStrongestDBM = wifiInfo[0].wifidBm;
        qDebug()<<"scan is success";
        getScanDebugFlag = false;
        noScanDebugFlag = true;
        GScanSuccessFlag = true;

        wlanChoice cho;
        cho.getWlanSite(true);
    }else{
        qDebug() <<"scan is not found:"<<str_output;
        noScanDebugFlag = false;
        getScanDebugFlag = true;
    }
}


/*******************************************************/
//打印当前所有网络AP信息
/*******************************************************/
//void iwlistValuation::printCurrentWifiList()
//{
//    qDebug()<<"thisIwlistscanAllWifi";
//    for (int i = 0; !wifiInfo[i].wifiName.isEmpty() && 0 != wifiInfo[i].wifidBm; i++){
//        qDebug() <<"wifiName:" <<wifiInfo[i].wifiName<<"wifiAddress:"<<wifiInfo[i].wifiAddress<<"    wifiDBm:"<<wifiInfo[i].wifidBm;
//    }
//}
