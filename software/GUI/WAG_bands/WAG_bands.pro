#-------------------------------------------------
#
# Project created by QtCreator 2015-11-06T13:09:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WAG_bands
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    superslider.cpp \
    overlay.cpp \
    overlaywidget.cpp \
    customshadoweffect.cpp

HEADERS  += mainwindow.h \
    superslider.h \
    overlay.h \
    overlaywidget.h \
    customshadoweffect.h

FORMS    +=

RESOURCES += \
    icons.qrc
