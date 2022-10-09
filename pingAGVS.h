#ifndef PINGAGVS_H
#define PINGAGVS_H

#include <QObject>
#include <QThread>

class pingAGVS : public QObject
{
    Q_OBJECT
public:
    explicit pingAGVS(QObject *parent = 0);
    //bool ping_ip(QString ip);
signals:

public slots:
    void ping_ip();
private:
    QThread pingThread;
};

#endif // PINGAGVS_H
