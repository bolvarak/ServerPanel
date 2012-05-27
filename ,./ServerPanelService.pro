#-------------------------------------------------
#
# Project created by QtCreator 2012-05-26T20:40:25
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ServerPanelService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ServerPanelRpcServer.cpp \
    Json.cpp \
    ServerPanelRpc.cpp

HEADERS += \
    ServerPanelRpcServer.h \
    Json.h \
    ServerPanelRpc.h
