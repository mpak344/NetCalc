QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = CalcServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    calcclient.cpp

HEADERS += \
    server.h \
    calcclient.h


