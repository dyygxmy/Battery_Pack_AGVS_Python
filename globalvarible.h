#ifndef GLOBALVARIBLE
#define GLOBALVARIBLE
#include <QList>
#include <QSettings>
#include <QReadWriteLock>
extern int GLevelListLen;
extern int GDefaultLevel;
extern int GDBdiff;
extern  QList<int>GPingResultList;
extern bool GPingResult;
extern QString GIP;//服务器ip
extern QString GScanWlan;
extern bool GScanSuccessFlag;//
extern int GStrongestDBM;
extern QString GSwitchSSID;//准备切换用的AP名
extern QString GSwitchBSSID;//准备切换用的AP MAC
extern QString GCurrentSSID;//当前连接的AP
extern QStringList GLoseAPIPList;
extern int GSwitchCount;//记录切换次数
extern int GScanCycleID;//scan到网络的循环号
extern int GIwconfigCycle;//iwconfig的循环号
extern int GSignalLevel;
extern bool GAllowPingThread;//开机启动连网后再开始ping
extern bool GAllowScanThread;
extern bool GAllowSwitThread;//有了有效的ssid，可以开始连网（处理刚开机）
extern bool GThisIwconfigIsNormal;//本次iwconfig命令正常使用
extern bool GReadyReboot;//准备重启
extern int GViNum;//小车号
extern QString GSsidKey;//提取ssid的关键字
struct wifi {
    QString wifiAddress;
    int wifidBm;
    QString wifiName;
};
extern wifi wifiInfo[100];

struct wlanStatusList{
    int agvs;
    QString wlan;
    int singls;
    QString APName;
    QString thisTime;
};

struct disStatisticsList{
    int agvs;
    QString wlan;
    int disType;
    QString APName;
    QString thisTime;
};

extern QSettings* GConfigIni;
extern bool GHaveConnection;//有连接上网过

extern QReadWriteLock GWlanStatusLock;
extern QReadWriteLock GDisStatusLock;
#endif // GLOBALVARIBLE

