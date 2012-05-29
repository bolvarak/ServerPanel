#ifndef SERVERPANEL_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANEL_H
#define SERVERPANEL_METHOD_NOTATION_KEY "sMethod"

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QtSql>
#include <QVariantMap>
#include <QSettings>
#include <QFile>
#include <Json.h>
#include <stdlib.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// ServerPanel Definition ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanel : public QObject {
    // Make sure this is seen as a QObject
    Q_OBJECT
// Public
public:
    // Singleton
    static ServerPanel* Instance      ();
    // Destructor
    ~ServerPanel                      ();
    // Methods
    QByteArray          HandleRequest (QString sRequest);
// Protected
protected :
    // Properties
    QSettings*           mConfig;   // System Configuration
    QSqlDatabase         mDbc;      // Database Connection
    static ServerPanel*  mInstance; // Singleton Instane
    // Constructor
    ServerPanel(QObject* cParent = 0);
    // Methods
    QVariantMap AuthenticateUser(QString sUsername, QString sPassword);
    QVariantMap DecodeRequest   (QString sRequest);
    QByteArray  EncodeResponse  (QVariantMap qvmResponse);
    void        LogMessage      (QByteArray qbaMessage);
};
#endif
