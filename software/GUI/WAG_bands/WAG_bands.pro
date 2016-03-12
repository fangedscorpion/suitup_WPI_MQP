#-------------------------------------------------
#
# Project created by QtCreator 2015-11-06T13:09:43
#
#-------------------------------------------------

QT       += core gui bluetooth network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WAG_bands
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    playbackcontroller.cpp \
    positionsnapshot.cpp \
    tabcontent.cpp \
    wagfile.cpp \
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
    customWidgets/motionviewer.cpp \
    band/quatpose.cpp \
    band/nullband.cpp \
    band/quaterror.cpp \
    math/reflection.cpp \
    test/testreflection.cpp \
    test/runtests.cpp \
    test/testquatpose.cpp \
    customWidgets/closablelabel.cpp \
    test/testquatstate.cpp \
    test/testquaterror.cpp \
    communications/incorrectdatalengthexception.cpp \
    visualization/model.cpp \
    visualization/modelloader.cpp \
    visualization/glwidget.cpp

HEADERS  += mainwindow.h \
    positionsnapshot.h \
    tabcontent.h \
    wagfile.h \
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
    customWidgets/motionviewer.h \
    band/absstate.h \
    band/abserror.h \
    math/reflection.h \
    test/testreflection.h \
    test/runtests.h \
    test/testquatpose.h \
    test/testquatstate.h \
    test/testquaterror.h \
    test/testband.h \
    customWidgets/closablelabel.h \
    localtesting.h \
    communications/incorrectdatalengthexception.h \
    band/absstate.h \
    visualization/model.h \
    visualization/modelloader.h \
    visualization/glwidget.h

FORMS    +=

RESOURCES += \
    icons.qrc \
    visualization/resources.qrc

CONFIG += console

LIBS += -L/usr/lib/ -lboost_filesystem -lboost_system -lassimp

DISTFILES += \
    README.txt \
    visualization/ads_fragment.frag \
    visualization/ads_fragment.vert
