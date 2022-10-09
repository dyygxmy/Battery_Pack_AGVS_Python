#ifndef WIFIROAMING_H
#define WIFIROAMING_H

#include <QMainWindow>
#include <QThread>
#include "globalvarible.h"
class wifiRoaming : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifiRoaming(QWidget *parent = 0);

private slots:
    void deal_wifi();
private:
    QThread wifiThread;
    int leveCurrent;//当前网的信号值
    int strongestDBM;//最强网的信号值
    int scanCycleIDTemp;//记录上次scan的循环号
    int iwconfigCycleTemp;//记录上次iwconfig的循环号
    int dbmWeakIndex;//信号弱指数（2次弱切换）
    int cycleTime;
    bool isSingleCard;
    bool isSwitchSuccessFlag;
    bool firstDisType;//刚开机第一次连接
    wlanStatusList wlanStatus;
    disStatisticsList disStatistics;
    void printCurrentWifiList();
    void clearLoseAPIPList();
};

#endif // WIFIROAMING_H
