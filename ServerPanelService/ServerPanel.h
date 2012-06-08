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
#include <QProcess>
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
    QVariantMap     AddAccount      (
            QString sUsername,
            QString sPassword,
            QString sEmailAddress,
            QString sFirstName,
            QString sLastName,
            QString sPhoneNumber,
            QString sHomeDirectory,
            QString sStreetAddress      = NULL,
            QString sStreetAddressExtra = NULL,
            QString sCity               = NULL,
            QString sState              = NULL,
            QString sPostalCode         = NULL,
            QString sCountry            = "USA",
            int iAccountLevel           = 3,
            QBool bEnabled              = true
    );
    QVariantMap     AddDnsRecord    (
            int     iAccountId,
            int     iDomainId,
            QString sHostName,
            QString sAddress,
            QString sDirection = "IN",
            QString sType      = "A",
            int     iPriority  = NULL,
            QBool   bEnabled   = true
    );
    QVariantMap     AddDomain       (
            int iAccountId,
            QString sDomain,
            QBool bEnabled = true
    );
    QVariantMap     AddMailBox      (
            QString sUsername,
            QString sPassword,
            QString sName                        = NULL,
            QString sStorageDirectory            = "/var/vmail",
            QString sStorageNode                 = "vmail1",
            QString sMailDirectory               = NULL,
            int     iQuota                       = 1024,
            int     iBytes                       = 0,
            QString sDomain,
            QString sTransport                   = NULL,
            QString sDepartment                  = NULL,
            QString sRank                        = "normal",
            QString sEmployeeId                  = NULL,
            QBool   bEnableSmtp                  = true,
            QBool   bEnableSecureSmtp            = true,
            QBool   bEnablePop3                  = true,
            QBool   bEnableSecurePop3            = true,
            QBool   bEnableImap                  = true,
            QBool   bEnableSecureImap            = true,
            QBool   bEnableDelivery              = true,
            QBool   bEnableSieveManagement       = true,
            QBool   bEnableSecureSieveManagement = true,
            QBool   bEnableInternalDelivery      = true,
            QString sDisclaimer                  = NULL,
            QBool   bActive                      = true,
            QString sLocalPartition              = NULL
    );
    QVariantMap     AddMailDomain   (
            int     iAccountId,
            int     iDomainId,
            QString sDescription           = NULL,
            QString sDisclaimer            = NULL,
            int     iMaxQuota              = 1024,
            int     iQuota                 = 1024,
            QString sTransport             = "dovecot",
            QBool   bBackupMx              = false,
            int     iDefaultUserQuota      = 1024,
            QString sDefaultPasswordScheme = "md5",
            int     iMinimumPasswordLength = 5,
            int     iMaximumPasswordLength = 75,
            QBool   bActive                = true
    );
    QVariantMap AuthenticateUser(QString sUsername, QString sPassword);
    QVariantMap CreateAccount   (
            QString sUsername,
            QString sPassword,
            QString sEmailAddress,
            QString sFirstName,
            QString sLastName,
            QString sHomeDirectory,
            QString sPhoneNumber        = NULL,
            QString sStreetAddress      = NULL,
            QString sStreetAddressExtra = NULL,
            QString sCity               = NULL,
            QString sState              = NULL,
            QString sPostalCode         = NULL,
            QString sCountry            = "USA",
            int     iAccountLevel       = 3,
            QBool   bEnabled            = true
    );
    QVariantMap DecodeRequest   (QString sRequest);
    QByteArray  EncodeResponse  (QVariantMap qvmResponse);
    QBool       ExecuteSystemCmd(QString sProgram, QStringList qslArguments);
    void        LogMessage      (QByteArray qbaMessage);
};
#endif
