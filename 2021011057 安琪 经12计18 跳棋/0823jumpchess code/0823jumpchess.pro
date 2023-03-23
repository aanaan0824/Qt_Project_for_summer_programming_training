QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog_connect_to_server.cpp \
    dialog_create_connection.cpp \
    dialog_win_lose.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialog_connect_to_server.h \
    dialog_create_connection.h \
    dialog_win_lose.h \
    mainwindow.h

FORMS += \
    dialog_connect_to_server.ui \
    dialog_create_connection.ui \
    dialog_win_lose.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
