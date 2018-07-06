QT += core
QT -= gui
QT += serialbus serialport
QT       += network

CONFIG -= c++11

TARGET = modbus_master
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mqtt_client.cpp \
    mqtt_settings.cpp \
    client.cpp \
    modbus_settings.cpp \
    webdownload.cpp \
    modbus_master.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    client.h \
    mqtt_client.h \
    modbus_settings.h \
    mqtt_settings.h \
    webdownload.h \
    vantagedata.h \
    modbus_master.h \
    utils.h \
    main.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/ -lmosquittopp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/ -lmosquittopp
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lmosquittopp

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include
