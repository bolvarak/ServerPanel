#ifndef SERVERPANELRPC_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELRPC_H
#define RPC_ENDPOINT_HOST "github.com"
#define RPC_ENDPOINT_PORT 80

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QtNetwork/QHttp>
#include <QUrl>
// #include <QtCrypto/QtCrypto>
#include "Json.h"

///////////////////////////////////////////////////////////////////////////////
/// ServerPanelRpc Definition ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanelRpc : public QObject {
    // Make sure we load a Qt Object
    Q_OBJECT
// Public
public :
    // Singleton
    static ServerPanelRpc* Instance();
    // Destructor
    ~ServerPanelRpc();
    // Methods
    QVariantMap DecodeJson         (QString sJson);
    QString     EncodeJson         (QVariantMap mapJson);
    void        GetRequestData     (QString sResource);
    void        SendRequestData    (QString sResource, QVariantMap mapJson);
    // Getters
    QVariantMap GetDecodedResponse ();
    QString     GetEncodedResponse ();
// Protected
protected :
    // Constructor
    ServerPanelRpc(QObject* cParent = 0);
    // Properties
    static ServerPanelRpc* mInstance;        // Singleton Instance
    QHttp*                 mNetwork;         // Network Access Manager
    QVariantMap            mDecodedResponse; // JSON Decoded Response
    QString                mEncodedResponse; // JSON Encoded Response
// Slots
protected slots :
    void ProcessResponseData(bool bError);
};
#endif
