#ifndef IWLISTVALUATION_H
#define IWLISTVALUATION_H

#include <QObject>
#include <QThread>

class iwlistValuation : public QObject
{
    Q_OBJECT
public:
    explicit iwlistValuation(QObject *parent = 0);

signals:

public slots:
    void get_scanningwifi_information();
private:
    QThread iwlistThread;
    int wifi_num;
    bool noScanDebugFlag;
    bool getScanDebugFlag;
    bool isFirstON;
    void wifi_sort();
//    void printCurrentWifiList();
    void scanDataProcessing(QString);
//    bool resetWlan(QString);
    int scanTime;

};

#endif // IWLISTVALUATION_H
