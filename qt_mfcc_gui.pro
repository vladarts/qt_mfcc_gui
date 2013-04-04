#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T23:18:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_mfcc_gui
TEMPLATE = app


SOURCES += \
    qcustomplot.cpp \
    mfcc_mainwindow.cpp \
    main.cpp \
    vWAV.cpp \
    vmfcc.cpp \
    qt_vmfcc.cpp

HEADERS  += \
    qcustomplot.h \
    mfcc_mainwindow.h \
    vWAV.h \
    vmfcc.h \
    qt_vmfcc.h \
    fftw3.h

OTHER_FILES += \
    README.md

FORMS +=

RESOURCES +=
