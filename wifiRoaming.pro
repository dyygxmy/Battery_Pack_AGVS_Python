#-------------------------------------------------
#
# Project created by QtCreator 2017-06-22T15:10:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = wifiRoaming_python_7
TEMPLATE = app


SOURCES += main.cpp\
        wifiroaming.cpp \
    pingAGVS.cpp \
    iwlistValuation.cpp \
    wlanChoice.cpp \
    mymsleep.cpp

HEADERS  += wifiroaming.h \
    globalvarible.h \
    pingAGVS.h \
    iwlistValuation.h \
    wlanChoice.h \
    mymsleep.h

FORMS    +=
