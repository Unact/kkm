TEMPLATE = lib

QT       += core

QT       -= gui

TARGET = kkm_interface

CONFIG += plugin

CONFIG   += console
CONFIG   -= app_bundle

SOURCES += kkm_interface.cpp

INCLUDEPATH += ./include/

HEADERS += \
    kkm_interface.h
