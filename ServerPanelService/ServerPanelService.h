#ifndef SERVERPANELSERVICE_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELSERVICE_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QtNetwork/QTcpServer>

///////////////////////////////////////////////////////////////////////////////
/// ServerPanelService Definition ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanelService : public QTcpServer {
    // Make sure this is seen as a QObject
    Q_OBJECT
// Public
public:
    // Singleton
    static ServerPanelService* Instance();
// Protected
protected :
    // Properties
    static ServerPanelService* mInstance; // Singleton Instance
    // Constructor
    ServerPanelService(QObject* cParent = 0);
    // Methods
    void incomingConnection(int iSocket);
};
#endif
