#-------------------------------------------------
#
# Project created by QtCreator 2015-08-29T13:48:44
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatclient
TEMPLATE = app


SOURCES += main.cpp\
        clientwindow.cpp \
        databuffer.cpp \
        netcom.cpp

HEADERS  += clientwindow.h \
    databuffer.h \
    netcom.h

FORMS    += clientwindow.ui
