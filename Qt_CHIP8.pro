#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T18:58:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_CHIP8
TEMPLATE = app


SOURCES += main.cpp\
    emulatorscreenwidget.cpp \
    chip8core.cpp \
    emulatorthread.cpp \
    emulatorwindow.cpp \
    chip8keyboarddisplay.cpp

HEADERS  += \
    emulatorscreenwidget.h \
    chip8core.h \
    emulatorthread.h \
    emulatorwindow.h \
    chip8keyboarddisplay.h

FORMS    += emulatordisplay.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
