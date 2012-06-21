#-------------------------------------------------
#
# Project created by QtCreator 2012-06-15T15:41:50
#
#-------------------------------------------------

QT               += core gui sql network
TARGET            = ServerPanelClient
TEMPLATE          = app
CONFIG           += crypto

unix {
    INCLUDEPATH += $$quote(qca-2.0.3/include/QtCrypto)
    LIBS        += -L$$quote(qca-2.0.3/lib) -lqca
}

macx {
    INCLUDEPATH += $$quote(/opt/local/include/QtCrypto)
    INCLUDEPATH += $$quote(/opt/local/include/qjson)
    LIBS        += -L$$quote(/opt/local/lib) -lqca -lqjson
}

win32 {
    INCLUDEPATH += $$quote(qca-2.0.3/include/QtCrypto)
    LIBS        += -L$$quote(qca-2.0.3/lib) -lqca2
}

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

FORMS            +=

OTHER_FILES      += \
    ServerPanelClient.sp \
    ServerPanel.ini

RESOURCES        += \
    ServerPanelClientResources.qrc
