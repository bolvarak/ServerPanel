#-------------------------------------------------
#
# Project created by QtCreator 2012-05-26T21:14:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ServerPanelService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ServerPanelServer.cpp \
    Json.cpp \
    ServerPanelRpc.cpp

HEADERS += \
    ServerPanelServer.h \
    Json.h \
    ServerPanelRpc.h
