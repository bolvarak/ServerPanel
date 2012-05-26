#-------------------------------------------------
#
# Project created by QtCreator 2012-05-25T19:01:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerPanel
TEMPLATE = app


SOURCES += main.cpp\
        ServerPanelMainWindow.cpp \
    ServerPanelAuthenticatedWindow.cpp

HEADERS  += ServerPanelMainWindow.h \
    ServerPanelAuthenticatedWindow.h

FORMS    += ServerPanelMainWindow.ui \
    ServerPanelAuthenticatedWindow.ui
