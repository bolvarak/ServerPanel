#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T15:41:50
#
#-------------------------------------------------

QT       += core gui sql network

TARGET = ServerPanelClient
TEMPLATE = app
CONFIG += crypto
INCLUDEPATH += $$quota(qca-2.0.3/include/QtCrypto)
LIBS += -L$$quota(qca-2.0.3/lib) -lqca2

SOURCES += main.cpp\
        ServerPanelClientLoginWindow.cpp \
    Json.cpp \
    ServerPanel.cpp

HEADERS  += ServerPanelClientLoginWindow.h \
    ServerPanelStructures.h \
    ServerPanel.h \
    Json.h

FORMS    += ServerPanelClientLoginWindow.ui

OTHER_FILES += \
    ServerPanelClient.sp \
    ServerPanel.ini
