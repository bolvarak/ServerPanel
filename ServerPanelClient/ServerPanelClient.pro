#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T15:41:50
#
#-------------------------------------------------

QT               += core gui sql network webkit
TARGET            = ServerPanelClient
TEMPLATE          = app
CONFIG           += crypto
unix:INCLUDEPATH += $$quota(qca-2.0.3/include/QtCrypto)
unix:LIBS        += -L$$quota(qca-2.0.3/lib) -lqca2
macx:INCLUDEPATH += /opt/local/include/
macx:LIBS        += -L/opt/local/lib -lqca
SOURCES          += \
    main.cpp\
    ServerPanelClientLoginWindow.cpp \
    Json.cpp \
    ServerPanel.cpp \
    ServerPanelClientMainWindow.cpp

HEADERS          += \
    ServerPanelClientLoginWindow.h \
    ServerPanelStructures.h \
    ServerPanel.h \
    Json.h \
    ServerPanelClientMainWindow.h

FORMS            += \
    ServerPanelClientLoginWindow.ui \
    ServerPanelClientMainWindow.ui

OTHER_FILES      += \
    ServerPanelClient.sp \
    ServerPanel.ini

RESOURCES        += \
    ServerPanelClientResources.qrc
