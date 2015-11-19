#-------------------------------------------------
#
# Project created by QtCreator 2015-11-06T13:09:43
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WAG_bands
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    superslider.cpp \
    glwidget.cpp \
    rectangularprism.cpp \
    glcamera.cpp \
    networkmonitor.cpp \
    bluetoothmanager.cpp

HEADERS  += mainwindow.h \
    superslider.h \
    glwidget.h \
    rectangularprism.h \
    glcamera.h \
    networkmonitor.h \
    bluetoothmanager.h

FORMS    +=

RESOURCES += \
    icons.qrc

CONFIG += console
