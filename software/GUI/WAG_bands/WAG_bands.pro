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
    band/quaterror.cpp \
    math/reflection.cpp \
    test/testreflection.cpp \
    test/runtests.cpp \
    test/testquatpose.cpp \
    customWidgets/closablelabel.cpp \
    test/testquatstate.cpp \
    test/testquaterror.cpp \
    communications/incorrectdatalengthexception.cpp \
    visualization/glwidget.cpp \
    visualization/modelgl.cpp \
    visualization/modelglloader.cpp \
    model/model.cpp \
    model/modelloader.cpp \
    customWidgets/styledgroupbox.cpp \
    customWidgets/styledcheckbox.cpp \
    test/testwagfile.cpp \
    frameupdater.cpp \
    band/absstate.cpp

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
    customWidgets/styledgroupbox.h \
    band/absstate.h \
    visualization/glwidget.h \
    visualization/modelgl.h \
    visualization/modelglloader.h \
    model/model.h \
    model/modelloader.h \
    customWidgets/styledcheckbox.h \
    test/testwagfile.h \
    frameupdater.h

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
