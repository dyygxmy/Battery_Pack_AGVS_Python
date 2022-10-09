#include "mymsleep.h"
#include <QTime>
#include <QApplication>

myMsleep::myMsleep()
{

}


/*******************************************************/
//死等，什么都不做
/*******************************************************/
myMsleep myMsleep::msleep(unsigned int msec)
{
    QTime outTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < outTime);
}


/*******************************************************/
//等待中，不断强制进入当前线程的事件循环，这样可以把堵塞的事件都处理掉，从而避免程序卡死
/*******************************************************/
myMsleep myMsleep::msleepEvent(unsigned int msec)
{
    QTime outTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < outTime){
        //这条语句能够使程序在while等待期间，去处理一下本线程的事件循环，处理事件循环最多100ms必须返回本语句，
        //如果提前处理完毕，则立即返回这条语句。这也就导致了该定时误差可能高达100ms。
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
