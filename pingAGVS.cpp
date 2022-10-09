#include "pingAGVS.h"
#include "globalvarible.h"
#include <QDebug>
#include <QTimer>
#include <QMutex>
QString pingPath = "/agvs/myPython/data/pingResult.conf";

pingAGVS::pingAGVS(QObject *parent) : QObject(parent)
{
    this->moveToThread(&pingThread);
    pingThread.start();
    qDebug()<<"pingAGVS start";
}


/*******************************************************/
//提取ping结果
/*******************************************************/
void pingAGVS::ping_ip()
{
    int pingTime = 1000;
    QSettings* pingConf = new QSettings(pingPath,QSettings::IniFormat);
    GPingResult = pingConf->value("key1/pingServerResult").toBool();
    if(pingConf){
        delete pingConf;
    }
    QTimer::singleShot(pingTime,this,SLOT(ping_ip()));
}
