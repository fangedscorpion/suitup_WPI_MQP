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
    overlay.cpp \
    overlaywidget.cpp \
    customshadoweffect.cpp \
    glwidget.cpp \
    rectangularprism.cpp \
    glcamera.cpp \
    networkmonitor.cpp \
    bluetoothmanager.cpp \
    playbackcontroller.cpp \
    motion.cpp \
    visualization.cpp \
    positionsnapshot.cpp \
    tabcontent.cpp

HEADERS  += mainwindow.h \
    superslider.h \
    glwidget.h \
    rectangularprism.h \
    glcamera.h \
    networkmonitor.h \
    bluetoothmanager.h \
    playbackcontroller.h \
    motion.h \
    visualization.h \
    positionsnapshot.h \
    overlay.h \
    overlaywidget.h \
    customshadoweffect.h \
    tabcontent.h

FORMS    +=

RESOURCES += \
    icons.qrc

CONFIG += console
