#-------------------------------------------------
#
# Project created by QtCreator 2015-11-06T13:09:43
#
#-------------------------------------------------

QT       += core gui bluetooth network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WAG_bands
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    playbackcontroller.cpp \
    motion.cpp \
    positionsnapshot.cpp \
    tabcontent.cpp \
    wagfile.cpp \
    visualization/suitsimulation.cpp \
    visualization/glcamera.cpp \
    visualization/glwidget.cpp \
    visualization/visualization.cpp \
    visualization/vertextranslator.cpp\
    mainwindowslots.cpp \
    band/absband.cpp \
    band/abspose.cpp \
    band/shoulderband.cpp \
    band/chestband.cpp \
    band/armband.cpp \
    band/suit.cpp \
    communications/wifimanager.cpp \
    communications/bluetoothmanager.cpp \
    communications/networkmonitor.cpp \
    editingcontroller.cpp \
    communications/messageinterpreter.cpp \
    recordingcontroller.cpp \
    customWidgets/customshadoweffect.cpp \
    customWidgets/overlay.cpp \
    customWidgets/overlaywidget.cpp \
    customWidgets/smartpushbutton.cpp \
    customWidgets/smartradiobutton.cpp \
    customWidgets/superslider.cpp

HEADERS  += mainwindow.h \
    motion.h \
    positionsnapshot.h \
    tabcontent.h \
    wagfile.h \
    visualization/suitsimulation.h \
    visualization/glcamera.h \
    visualization/glwidget.h \
    visualization/visualization.h \
    visualization/vertextranslator.h \
    user.h \
    band/absband.h \
    band/abspose.h \
    band/suit.h \
    communications/wifimanager.h \
    communications/networkmonitor.h \
    communications/bluetoothmanager.h \
    editingcontroller.h \
    communications/messageinterpreter.h \
    recordingcontroller.h \
    customWidgets/customshadoweffect.h \
    customWidgets/overlay.h \
    customWidgets/overlaywidget.h \
    customWidgets/smartpushbutton.h \
    customWidgets/smartradiobutton.h \
    customWidgets/superslider.h

FORMS    +=

RESOURCES += \
    icons.qrc

CONFIG += console

LIBS += -L/usr/lib/ -lboost_filesystem -lboost_system

DISTFILES += \
    README.txt
