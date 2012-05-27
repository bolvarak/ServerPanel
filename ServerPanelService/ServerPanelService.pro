#-------------------------------------------------
#
# Project created by QtCreator 2012-05-27T14:46:20
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = ServerPanelService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Json.cpp \
    ServerPanelRpc.cpp \
    ServerPanelServer.cpp \
    ServerPanelService.cpp

HEADERS += \
    Json.h \
    ServerPanelRpc.h \
    ServerPanelServer.h \
    ServerPanelService.h
