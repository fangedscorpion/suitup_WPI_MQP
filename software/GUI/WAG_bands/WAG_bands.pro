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
    editingcontroller.cpp \
    recordingcontroller.cpp \
    customWidgets/customshadoweffect.cpp \
    customWidgets/overlay.cpp \
    customWidgets/overlaywidget.cpp \
    customWidgets/smartpushbutton.cpp \
    customWidgets/smartradiobutton.cpp \
    customWidgets/superslider.cpp \
    tabcontent-record.cpp \
    tabcontent-edit.cpp \
    tabcontent-playback.cpp \
    communications/bandmessage.cpp \
    customWidgets/motionviewer.cpp

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
    editingcontroller.h \
    recordingcontroller.h \
    customWidgets/customshadoweffect.h \
    customWidgets/overlay.h \
    customWidgets/overlaywidget.h \
    customWidgets/smartpushbutton.h \
    customWidgets/smartradiobutton.h \
    customWidgets/superslider.h \
    playbackcontroller.h \
    communications/bandmessage.h \
    customWidgets/motionviewer.h

FORMS    +=

RESOURCES += \
    icons.qrc

CONFIG += console

LIBS += -L/usr/lib/ -lboost_filesystem -lboost_system

DISTFILES += \
    README.txt
