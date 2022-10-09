#include "wifiroaming.h"
#include <QApplication>
#include "globalvarible.h"
#include "pingAGVS.h"
#include <QTimer>
#include "iwlistValuation.h"
#include <QtCore/QtGlobal>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QDebug>
#include <QSettings>
#include <unistd.h>

int GLevelListLen = 5;
int GDefaultLevel = -69;
int GDBdiff = 10;
QList<int>GPingResultList;
bool GPingResult = false;
bool GScanSuccessFlag = false;
int GStrongestDBM = 0;
QString GSwitchSSID = "";//准备切换用的AP名
QString GSwitchBSSID = "";//准备切换用的AP MAC
QString GCurrentSSID = "";//当前连接的AP
QStringList GLoseAPIPList;
wifi wifiInfo[100];
int GSwitchCount = 0;//记录切换次数
int GScanCycleID = 0;//scan到网络的循环号
int GIwconfigCycle = 0;//iwconfig的循环号
int GSignalLevel = GDefaultLevel;

bool GAllowPingThread = false;//开机启动连网后再开始ping
bool GAllowScanThread = true;
bool GAllowSwitThread = false;//有了有效的ssid，可以开始连网（处理刚开机）

bool GThisIwconfigIsNormal = false;//本次iwconfig命令正常使用
bool GReadyReboot = false;//准备重启


QSettings* GConfigIni = new QSettings("/agvs/agvsConfig.ini",QSettings::IniFormat);
int GViNum = GConfigIni->value("baseinfo/ViNum").toInt();//小车号
QString GIP = GConfigIni->value("baseinfo/AGVSServer").toString();//服务器IP
bool GHaveConnection = false;//有连接上网过

QString GSsidKey = "BIW-AP";//提取ssid的关键字
QString versionNUM = "Version:V_python_2019.07.31_1";

/******************日志打印级别函数****************************/
void outputMessage(QtMsgType type,const QMessageLogContext &context,const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text("");
    switch(type){
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        abort();
    }
    if(text != QString("Warning:"))
    {
        QString message = QString("[%1] %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz ddd")).arg(text).arg(msg);
        QDateTime time = QDateTime::currentDateTime();
        QString date = time.toString("yyyy-MM-dd");
        QFile file(QString("/agvs/log/")+date+QString("wifi.log"));
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QTextStream text_stream(&file);
        text_stream << message << endl;
        file.flush();
        file.close();
    }
    mutex.unlock();
}

//void getAGVSServerIP()
//{
//    QString indexIP = "AGVSServer=";
//    QString indexViNum = "ViNum=";
//    QFile configFile("/agvs/agvsConfig.ini");
//    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
//        QByteArray readData = configFile.readAll();
//        QString data = readData;
//        data = data.trimmed();
//        QStringList dataList= data.split("\n");
//        for(int i =0;i<dataList.size();i++){
//            QString lineData = dataList.at(i);
//            if(-1 < lineData.indexOf(indexIP)){
//                GIP = lineData.right(lineData.size()-indexIP.size());
//            }
//            if(lineData.contains(indexViNum)){
//                GViNum = lineData.right(lineData.size()-indexViNum.size()).toInt();
//            }
//        }
//        configFile.close();
//    }else{
//        qDebug()<<"open /agvs/agvsConfig.ini:"<<configFile.errorString();
//    }
//}

//void getAllAPList()
//{
//    QFile apFile("/agvs/APLIST.txt");
//    if(apFile.open(QIODevice::ReadOnly | QIODevice::Text)){
//        QByteArray readData = apFile.readAll();
//        QString data = readData;
//        data = data.trimmed();
//        QStringList dataList= data.split("\n");
//        for(int i = 0;i<dataList.size();i++){
//            QString lineData =dataList.at(i);
//            QStringList lineList = lineData.split(",");
//            allAP[i].IP = lineList.at(0);
//            allAP[i].BSSID = lineList.at(1);
//            allAP[i].SSID = lineList.at(2);
//        }
//        apFile.close();
//    }else{
//        qDebug()<<"open APLIST:"<<apFile.errorString();
//    }
//}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<wlanStatusList>("wlanStatusList");
    qRegisterMetaType<disStatisticsList>("disStatisticsList");
    //重定向日志到文件
    qInstallMessageHandler(outputMessage);
    qDebug()<<"thisversionNUM:"<<versionNUM;

    iwlistValuation myIwlist;
    QTimer::singleShot(0,&myIwlist,SLOT(get_scanningwifi_information()));
    wifiRoaming w;
    QTimer::singleShot(0,&w,SLOT(deal_wifi()));
    pingAGVS myPing;
    QTimer::singleShot(0,&myPing,SLOT(ping_ip()));


//    interfaceUpload interUpload;
//    QTimer::singleShot(0,&interUpload,SLOT(uploadCurWlanStatus()));
//    QTimer::singleShot(0,&interUpload,SLOT(uploadDisWlanStatus()));

    QString setCableIP = "ifconfig eth0 192.168.2." + QString::number(100+GViNum);
    system(setCableIP.toLatin1().data());

    return a.exec();
}
