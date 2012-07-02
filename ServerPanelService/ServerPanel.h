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
#include <QVariantList>
#include <QVariantMap>
#include <QSettings>
#include <QFile>
#include <QProcess>
#include <Json.h>
#include <stdlib.h>
#include <iostream>
#include <ServerPanelStructures.h>

///////////////////////////////////////////////////////////////////////////////
/// ServerPanel Definition ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanel : public QObject {
    // Make sure this is seen as a QObject
    Q_OBJECT
// Public
public:
    // Properties
    static QString MethodLoadAccount;
    static QString MethodLoadDnsRecord;
    static QString MethodLoadDnsRecords;
    static QString MethodLoadDomain;
    static QString MethodLoadDomains;
    static QString MethodLoadMailBox;
    static QString MethodLoadMailBoxes;
    static QString MethodLoadMailDomain;
    static QString MethodLoadMailDomains;
    static QString MethodPing;
    static QString MethodSaveAccount;
    static QString MethodSaveDnsRecord;
    static QString MethodSaveDomain;
    static QString MethodSaveMailBox;
    static QString MethodSaveMailDomain;
    // Singleton
    static ServerPanel* Instance();
    // Destructor
                ~ServerPanel    ();
    // Methods
    QVariantMap HandleCliRequest(QStringList qslArguments);
    QByteArray  HandleRequest   (QString sRequest);
// Protected
protected :
    // Properties
    QSettings*           mConfig;   // System Configuration
    QSqlDatabase         mDbc;      // Database Connection
    static ServerPanel*  mInstance; // Singleton Instane
    // Constructor
    ServerPanel(QObject* cParent = 0);
    // Methods
    QVariantMap AuthenticateUser(SpAccount    spAccount);
    QVariantMap DecodeRequest   (QString      sRequest);
    QByteArray  EncodeResponse  (QVariantMap  qvmResponse);
    QBool       ExecuteSystemCmd(QString      sProgram,  QStringList qslArguments);
    QVariantMap LoadAccount     (SpAccount    spAccount);
    QVariantMap LoadDnsRecord   (SpDnsRecord  spDnsRecord);
    QVariantMap LoadDnsRecords  (SpDnsRecord  spDnsRecord);
    QVariantMap LoadDomain      (SpDomain     spDomain);
    QVariantMap LoadDomains     (SpDomain     spDomain);
    QVariantMap LoadMailBox     (SpMailBox    spMailBox);
    QVariantMap LoadMailBoxes   (SpMailBox    spMailBox);
    QVariantMap LoadMailDomain  (SpMailDomain spMailDomain);
    QVariantMap LoadMailDomains (SpMailDomain spMailDomain);
    void        LogMessage      (QByteArray   qbaMessage);
    QVariantMap SaveAccount     (SpAccount    spAccount);
    QVariantMap SaveDnsRecord   (SpDnsRecord  spDnsRecord);
    QVariantMap SaveDomain      (SpDomain     spDomain);
    QVariantMap SaveMailBox     (SpMailBox    spMailBox);
    QVariantMap SaveMailDomain  (SpMailDomain spMailDomain);
};
#endif
