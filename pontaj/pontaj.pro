#-------------------------------------------------
#
# Project created by QtCreator 2016-11-28T16:02:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pontaj
TEMPLATE = app

CONFIG -= app_bundle

include(../usb/libQDeviceWatcher.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    MFRC522.cpp \
    atprc522thread.cpp \
    atpinorout.cpp \
    atpsettings.cpp \
    atpdb.cpp \
    atpini.cpp \
    atplogdetails.cpp

HEADERS  += mainwindow.h \
    MFRC522.h \
    atprc522thread.h \
    atpinorout.h \
    atpsettings.h \
    atpdb.h \
    atpini.h \
    atplogdetails.h

FORMS    += mainwindow.ui \
    atpinorout.ui \
    atpsettings.ui \
    atplogdetails.ui


INCLUDEPATH += $$[QT_SYSROOT]/usr/local/include
LIBS += -L$$[QT_SYSROOT]/usr/local/lib -lbcm2835
#LIBS += -L$$[QT_SYSROOT]/usr/local/lib -lQDeviceWatcher

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    img.qrc
