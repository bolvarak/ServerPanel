#-------------------------------------------------
#
# Project created by QtCreator 2012-05-27T14:46:20
#
#-------------------------------------------------

QT          += core network sql
TARGET       = ServerPanelService
CONFIG      += console app_bundle release
TEMPLATE     = app
SOURCES     += main.cpp \
    Json.cpp \
    ServerPanelService.cpp \
    ServerPanel.cpp

HEADERS     += \
    Json.h \
    ServerPanelService.h \
    ServerPanel.h \
    ServerPanelStructures.h \
    ServerPanelStructures.h

OTHER_FILES += \
    ServerPanel.ini \
    Access.log \
    ServerPanel.ini \
    Access.log

RESOURCES += \
    ServerPanelService.qrc
