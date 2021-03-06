#ifndef SERVERPANELSERVICE_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELSERVICE_H
#define SERVERPANELSERVICE_LISTEN_PORT 1597

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QtNetwork>
#include <QtCore>
#include <Json.h>
#include <ServerPanel.h>
#include <stdlib.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// ServerPanelService Definition ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanelService : public QObject {
    // Make sure this is seen as a QObject
    Q_OBJECT
// Public
public:
    // Singleton
    static ServerPanelService* Instance();
    // Destructor
    ~ServerPanelService();
// Protected
protected :
    // Properties
    quint16                    mBlockSize; // Block Size
    QTcpSocket*                mClient;    // Client Socket
    static ServerPanelService* mInstance;  // Singleton Instance
    QTcpServer                 mServer;    // Socket Server
    // Constructor
    ServerPanelService      (QObject* cParent = 0);
    // Methods
    QByteArray HandleRequest(QString sJson);
// Slots
public slots :
    void acceptConnection();
    void startRead();
};
#endif
