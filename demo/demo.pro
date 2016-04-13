QT += core
QT -= gui

TARGET = demo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    hexstring.cpp \
    helloworld.cpp

HEADERS += \
    hexstring.h \
    helloworld.h

DISTFILES += \
    readme.txt

