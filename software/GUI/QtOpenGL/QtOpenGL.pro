#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T21:26:22
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    window.cpp \
    modelloader.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    window.h \
    modelloader.h

FORMS    += mainwindow.ui

LIBS += -L/usr/lib/ -lassimp

OTHER_FILES += ads_fragment.vert \
    ads_fragment.frag \
    es_ads_fragment.frag \
    es_ads_fragment.vert

RESOURCES += \
    resources.qrc
