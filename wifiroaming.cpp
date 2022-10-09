#include "wifiroaming.h"
#include "pingAGVS.h"
#include "iwlistValuation.h"
#include <QDebug>
#include <QTimer>
#include "wlanChoice.h"
#include <QMutex>
#include "mymsleep.h"
#include <QDateTime>
#include <QReadWriteLock>
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>

QString wifiStatePath = "/agvs/myPython/data/iwconfig.conf";
QString needSwitchPath = "/agvs/myPython/data/allowSwitch.conf";

wifiRoaming::wifiRoaming(QWidget *parent) :
    QMainWindow(parent)
{
    this->moveToThread(&wifiThread);
    wifiThread.start();
    cycleTime = 0;
    leveCurrent = 0;
    strongestDBM = 0;
    scanCycleIDTemp = -1;//记录上次scan的循环号
    iwconfigCycleTemp = -1;//记录上次iwconfig的循环号
    dbmWeakIndex = 0;//信号弱指数（2次弱切换）
    isSingleCard = false;
    firstDisType = true;//刚开机第一次连接
    isSwitchSuccessFlag = true;
    wlanStatus.agvs = GViNum;
    disStatistics.agvs = GViNum;
    qDebug()<<"wifiRoaming start";
}


/*******************************************************/
//检测网络状态，查看网络是否需要切换
/*******************************************************/
void wifiRoaming::deal_wifi()
{
    cycleTime = 3000;
    if(GAllowSwitThread){
        //新iwconfig
        QSettings* wifiStateConf = new QSettings(wifiStatePath,QSettings::IniFormat);
        GIwconfigCycle = wifiStateConf->value("key1/cycle").toInt();
        if(GIwconfigCycle > iwconfigCycleTemp){
            GCurrentSSID = wifiStateConf->value("key1/WlanName").toString();
            wlanChoice cho;
            cho.getWlanSite(true);//将当前在用的AP(GCurrentSSID)添加到过滤表
            iwconfigCycleTemp = GIwconfigCycle;
        }
        if(wifiStateConf){
            delete wifiStateConf;
        }
        bool isNeedSwitch = false;
        int disType = 0;//0掉网切换 1信号弱切换
        qDebug() <<"deal_wifi:"<<GPingResult;
        if(GPingResult){
            //新scan到的网
            if(GScanCycleID > scanCycleIDTemp){
                //取当前连接AP在搜索的AP列表中的信号强度
                for (int i = 0; !wifiInfo[i].wifiName.isEmpty() && 0 != wifiInfo[i].wifidBm; i++){
                    QString currentName = wifiInfo[i].wifiName;
                    if(currentName == GCurrentSSID){
                        leveCurrent = wifiInfo[i].wifidBm;
                    }
                }
                qDebug()<<"strongestDBM:"<<strongestDBM<<"leveCurrent:"<<leveCurrent;
                if(strongestDBM - leveCurrent > GDBdiff){
                    dbmWeakIndex++;
                }else{
                    dbmWeakIndex = 0;
                }
                scanCycleIDTemp = GScanCycleID;
            }
            //当前AP信号与最强AP信号对比超10超2次
            if(dbmWeakIndex >= 2){
                isNeedSwitch = true;
                disType = 1;
            }
        }else{
            isNeedSwitch = true;
        }

        if(isNeedSwitch){
            printCurrentWifiList();
            qDebug()<<"disType:"<<disType;
            dbmWeakIndex = 0;
            //上切换失败，如果是AP连接不上（非iwconfig引起），本次切换跳过该AP
            if(GSwitchCount > 0 && GThisIwconfigIsNormal){
                wlanChoice cho;
                cho.getWlanSite(false);//将之前切换用过的AP(GSwitchSSID)添加到过滤表
            }
            QSettings* needSwitchConf = new QSettings(needSwitchPath,QSettings::IniFormat);
            needSwitchConf->setValue("key1/needSwitch","True");//打开切换开关
            if(needSwitchConf){
                delete needSwitchConf;
            }

            //连网检测
            int myCount = 0;
            while(!GPingResult){
                if(myCount > 25*1000){//连网等待25秒超时
                    break;
                }
                myMsleep::msleep(1);
                myCount++;
            }
            if(GPingResult){
                GLoseAPIPList.clear();
                GSwitchCount = 0;
                isSwitchSuccessFlag = true;
                qDebug()<<"switchWlan success";
            }else{
                GSwitchCount++;
                qDebug()<<"switchWlan fail"<<GSwitchCount;
                isSwitchSuccessFlag = false;
                cycleTime = 0;
            }
        }
    }
    QTimer::singleShot(cycleTime,this,SLOT(deal_wifi()));
}


/*******************************************************/
//打印当前所有网络AP信息
/*******************************************************/
void wifiRoaming::printCurrentWifiList()
{
    qDebug()<<"allWifi:"<<GScanCycleID<<"GCurrentSSID:"<<GCurrentSSID;
    strongestDBM = wifiInfo[0].wifidBm;
    for (int i = 0; !wifiInfo[i].wifiName.isEmpty() && 0 != wifiInfo[i].wifidBm; i++){
        qDebug() <<"wifiName:"<<wifiInfo[i].wifiName<<"wifiAddress:"<<wifiInfo[i].wifiAddress<<"wifiDBm:"<<wifiInfo[i].wifidBm;
    }
}

/*******************************************************/
//清空切换过滤表
/*******************************************************/
void wifiRoaming::clearLoseAPIPList()
{
    while(0 != GLoseAPIPList.size()){
        GLoseAPIPList.removeFirst();
    }
}

