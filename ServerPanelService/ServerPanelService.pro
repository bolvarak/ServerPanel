#-------------------------------------------------
#
# Project created by QtCreator 2012-05-26T21:14:23
#
#-------------------------------------------------

QT       -= gui core network

TARGET = ServerPanelService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ServerPanelServer.cpp \
    Json.cpp \
    ServerPanelRpc.cpp \
    ServerPanelService.cpp

HEADERS += \
    ServerPanelServer.h \
    Json.h \
    ServerPanelRpc.h \
    ServerPanelService.h
