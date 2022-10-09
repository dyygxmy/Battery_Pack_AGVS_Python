#ifndef WLANCHOICE_H
#define WLANCHOICE_H

#include <QObject>

class wlanChoice : public QObject
{
    Q_OBJECT
public:
    explicit wlanChoice(QObject *parent = 0);
    void getWlanSite(bool);

signals:

private:
    void replace_ssid(int);

public slots:
};

#endif // WLANCHOICE_H
