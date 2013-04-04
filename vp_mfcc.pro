#-------------------------------------------------
#
# Project created by QtCreator 2013-01-22T17:06:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vp_mfcc
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    main.cpp \
    vmfcc.cpp

HEADERS  += mainwindow.h \
    fftw3.h \
    vmfcc.h

FORMS    += mainwindow.ui

LIBS    +=  "D:/Windows_Docs/Documents/QT/vp_mfcc/libfftw-3.3.lib" \
            "D:/Windows_Docs/Documents/QT/vp_mfcc/libfftwf-3.3.lib" \

