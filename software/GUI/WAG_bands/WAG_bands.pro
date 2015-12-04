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
    networkmonitor.cpp \
    bluetoothmanager.cpp \
    playbackcontroller.cpp \
    motion.cpp \
    positionsnapshot.cpp \
    visualization/suitsimulation.cpp \
    visualization/glcamera.cpp \
    visualization/glwidget.cpp \
    visualization/visualization.cpp \
    visualization/vertextranslator.cpp\
    tabcontent.cpp

HEADERS  += mainwindow.h \
    superslider.h \
    networkmonitor.h \
    bluetoothmanager.h \
    playbackcontroller.h \
    motion.h \
    positionsnapshot.h \
    overlay.h \
    overlaywidget.h \
    customshadoweffect.h \
    visualization/suitsimulation.h \
    visualization/glcamera.h \
    visualization/glwidget.h \
    visualization/visualization.h \
    visualization/vertextranslator.h \
    tabcontent.h

FORMS    +=

RESOURCES += \
    icons.qrc

CONFIG += console
