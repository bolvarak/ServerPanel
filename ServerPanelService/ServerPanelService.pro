#-------------------------------------------------
#
# Project created by QtCreator 2012-05-27T14:46:20
#
#-------------------------------------------------

QT          += gui core network sql
TARGET       = ServerPanelService
CONFIG      += console app_bundle
TEMPLATE     = app
SOURCES     += main.cpp \
    Json.cpp \
    ServerPanelRpc.cpp \
    ServerPanelService.cpp \
    ServerPanel.cpp

HEADERS     += \
    Json.h \
    ServerPanelRpc.h \
    ServerPanelService.h \
    ServerPanel.h \
    ServerPanelStructures.h

OTHER_FILES += \
    ServerPanel.ini \
    Access.log \
    ServerPanel.ini \
    Access.log
