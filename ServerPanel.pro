#-------------------------------------------------
#
# Project created by QtCreator 2012-05-25T19:01:44
#
#-------------------------------------------------

QT       += core gui network script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerPanel
TEMPLATE = app


SOURCES += main.cpp\
    ServerPanelMainWindow.cpp \
    ServerPanelAuthenticatedWindow.cpp \
    ServerPaneRpc.cpp \
    Json.cpp

HEADERS  += ServerPanelMainWindow.h \
    ServerPanelAuthenticatedWindow.h \
    ServerPanelRpc.h \
    Json.h

FORMS    += ServerPanelMainWindow.ui \
    ServerPanelAuthenticatedWindow.ui

LIBS += -L$$quote(qca-2.0.3/lib) -lqca
