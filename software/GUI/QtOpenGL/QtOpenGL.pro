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
    logo.cpp \
    window.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    logo.h \
    window.h

FORMS    += mainwindow.ui

LIBS += -L/usr/lib/ -lassimp


