#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T19:37:22
#
#-------------------------------------------------

QT       += core gui

TARGET = pMarker
TEMPLATE = app

RC_FILE = pMarker.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    picform.cpp \
    values.cpp \
    limage.cpp \
    dockform.cpp \
    preview.cpp \
    lnote.cpp \
    lmark.cpp \
    markdialog.cpp

HEADERS  += mainwindow.h \
    picform.h \
    values.h \
    limage.h \
    dockform.h \
    lmath.h \
    preview.h \
    lnote.h \
    lmark.h \
    markdialog.h


LIBS +=./exlib/opencv_core231.lib
LIBS +=./exlib/opencv_imgproc231.lib
LIBS +=./exlib/opencv_highgui231.lib


INCLUDEPATH += ./exinclude
INCLUDEPATH += ./exinclude/opencv
INCLUDEPATH += ./exinclude/opencv2
