#ifndef SERVERPANELSTRUCTURES_H
///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELSTRUCTURES_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QtSql>
#include <stdlib.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// Structures ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This structure maps out a user account object that is compatible with ServerPanel
 * @brief The SpAccount struct
 */
struct SpAccount {
    int     iAccountId;
    QString sUsername;
    QString sPassword;
    QString sEmailAddress;
    QString sFirstName;
    QString sLastName;
    QString sPhoneNumber;
    QString sHomeDirectory;
    QString sStreetAddress;
    QString sStreetAddressExtra;
    QString sCity;
    QString sState;
    QString sPostalCode;
    QString sCountry;
    int     iAccountLevel;
    bool    bEnabled;
    // Initializer
    SpAccount() : iAccountId(0), sCountry("USA"), iAccountLevel(3), bEnabled(true) {}
    SpAccount(QSqlRecord qsrAccount) {
        // Set the properties
        this->iAccountId          = qsrAccount.value("iAccountId").toInt();
        this->sUsername           = qsrAccount.value("sUsername").toString();
        this->sPassword.clear();
        this->sEmailAddress       = qsrAccount.value("sEmailAddress").toString();
        this->sFirstName          = qsrAccount.value("sFirstName").toString();
        this->sLastName           = qsrAccount.value("sLastName").toString();
        this->sPhoneNumber        = qsrAccount.value("sPhoneNumber").toString();
        this->sHomeDirectory      = qsrAccount.value("sHomeDirectory").toString();
        this->sStreetAddress      = qsrAccount.value("sStreetAddress").toString();
        this->sStreetAddressExtra = qsrAccount.value("sStreetAddressExtra").toString();
        this->sCity               = qsrAccount.value("sCity").toString();
        this->sState              = qsrAccount.value("sState").toString();
        this->sPostalCode         = qsrAccount.value("sPostalCode").toString();
        this->sCountry            = qsrAccount.value("sCountry").toString();
        this->bEnabled            = qsrAccount.value("bEnabled").toBool();
    }
    SpAccount(QVariantMap qvmAccount) {
        // Convert the account id
        if (qvmAccount.contains("iAccountId")) {
            this->iAccountId = qvmAccount["iAccountId"].toInt();
        }
        // Convert the username
        if (qvmAccount.contains("sUsername")) {
            this->sUsername = qvmAccount["sUsername"].toString();
        }
        // Convert the password
        if (qvmAccount.contains("sPassword")) {
            this->sPassword = qvmAccount["sPassword"].toString();
        }
        // Convert the email address
        if (qvmAccount.contains("sEmailAddress")) {
            this->sEmailAddress = qvmAccount["sEmailAddress"].toString();
        }
        // Convert the first name
        if (qvmAccount.contains("sFirstName")) {
            this->sFirstName = qvmAccount["sFirstName"].toString();
        }
        // Convert the last name
        if (qvmAccount.contains("sLastName")) {
            this->sLastName = qvmAccount["sLastName"].toString();
        }
        // Convert the phone number
        if (qvmAccount.contains("sPhoneNumber")) {
            this->sPhoneNumber = qvmAccount["sPhoneNumber"].toString();
        }
        // Convert the home directory
        if (qvmAccount.contains("sHomeDirectory")) {
            this->sHomeDirectory = qvmAccount["sHomeDirectory"].toString();
        }
        // Convert the street address
        if (qvmAccount.contains("sStreetAddress")) {
            this->sStreetAddress = qvmAccount["sStreetAddress"].toString();
        }
        // Convert the extra street address
        if (qvmAccount.contains("sStreetAddressExtra")) {
            this->sStreetAddressExtra = qvmAccount["sStreetAddressExtra"].toString();
        }
        // Convert the city
        if (qvmAccount.contains("sCity")) {
            this->sCity = qvmAccount["sCity"].toString();
        }
        // Convert the state
        if (qvmAccount.contains("sState")) {
            this->sState = qvmAccount["sState"].toString();
        }
        // Convert the postal code
        if (qvmAccount.contains("sPostalCode")) {
            this->sPostalCode = qvmAccount["sPostalCode"].toString();
        }
        // Convert the country
        if (qvmAccount.contains("sCountry")) {
            this->sCountry = qvmAccount["sCountry"].toString();
        }
        // Convert the active status
        if (qvmAccount.contains("bEnabled")) {
            this->bEnabled = qvmAccount["bEnabled"].toBool();
        }
    }

    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Set the properties
        qvmProperties.insert("iAccountId",          this->iAccountId);
        qvmProperties.insert("sUsername",           this->sUsername);
        qvmProperties.insert("sPassword",           this->sPassword);
        qvmProperties.insert("sEmailAddress",       this->sEmailAddress);
        qvmProperties.insert("sFirstName",          this->sFirstName);
        qvmProperties.insert("sLastName",           this->sLastName);
        qvmProperties.insert("sPhoneNumber",        this->sPhoneNumber);
        qvmProperties.insert("sHomeDirectory",      this->sHomeDirectory);
        qvmProperties.insert("sStreetAddress",      this->sStreetAddress);
        qvmProperties.insert("sStreetAddressExtra", this->sStreetAddressExtra);
        qvmProperties.insert("sCity",               this->sCity);
        qvmProperties.insert("sState",              this->sState);
        qvmProperties.insert("sPostalCode",         this->sPostalCode);
        qvmProperties.insert("sCountry",            this->sCountry);
        qvmProperties.insert("bEnabled",            this->bEnabled);
        // Return the map
        return qvmProperties;
    }
    /**
     * @paragraph This function converts the structure to a prepared bound QSqlQuery statement
     * @brief toQuery
     * @param QSqlDatabase qsdConnection
     * @param QString sQuery
     * @return QSqlQuery
     */
    QSqlQuery toQuery(QSqlDatabase qsdConnection, QString sQuery) {
        // Setup the SQL Query
        QSqlQuery qsqAccount(qsdConnection);
        // Prepare the SQL statement
        qsqAccount.prepare(sQuery);
        // Grab the map
        QVariantMap qvmProperties(this->toMap());
        // Grab the iterators
        QVariantMap::ConstIterator itrProperty      = qvmProperties.constBegin();
        QVariantMap::ConstIterator itrPropertyEnd   = qvmProperties.constEnd();
        // Loop through the properties
        for (; itrProperty != itrPropertyEnd; ++itrProperty) {
            // Check for the identifier in the SQL statement
            if (sQuery.contains(QString(itrProperty.key()).prepend(":"), Qt::CaseSensitive)) {
                // Bind the value
                qsqAccount.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqAccount;
    }
};

/**
 * @paragraph This structure maps out a DNS record object that is compatible with ServerPanel
 * @brief The SpDnsRecord struct
 */
struct SpDnsRecord {
    int     iRecordId;
    int     iAccountId;
    int     iDomainId;
    QString sHostName;
    QString sAddress;
    QString sDirection;
    QString sType;
    int     iPriority;
    bool    bEnabled;
    // Initializer
    SpDnsRecord() : iRecordId(0), iAccountId(0), iDomainId(0), sDirection("IN"), sType("A"), bEnabled(true) {}
    SpDnsRecord(QSqlRecord qsrDnsRecord) {
        // Set the properties
        this->iRecordId  = qsrDnsRecord.value("iRecordId").toInt();
        this->iAccountId = qsrDnsRecord.value("iAccountId").toInt();
        this->iDomainId  = qsrDnsRecord.value("iDomainId").toInt();
        this->sHostName  = qsrDnsRecord.value("sHostName").toString();
        this->sAddress   = qsrDnsRecord.value("sAddress").toString();
        this->sDirection = qsrDnsRecord.value("sDirection").toString();
        this->sType      = qsrDnsRecord.value("sType").toString();
        this->iPriority  = qsrDnsRecord.value("iPriority").toInt();
        this->bEnabled   = qsrDnsRecord.value("bEnabled").toBool();
    }
    SpDnsRecord(QVariantMap qvmDnsRecord) {
        // Convert record id
        if (qvmDnsRecord.contains("iRecordId")) {
            this->iRecordId = qvmDnsRecord["iRecordId"].toInt();
        }
        // Convert the account id
        if (qvmDnsRecord.contains("iAccountId")) {
            this->iAccountId = qvmDnsRecord["iAccountId"].toInt();
        }
        // Convert the domain id
        if (qvmDnsRecord.contains("iDomainId")) {
            this->iDomainId = qvmDnsRecord["iDomainId"].toInt();
        }
        // Convert the host name
        if (qvmDnsRecord.contains("sHostName")) {
            this->sHostName = qvmDnsRecord["sHostName"].toString();
        }
        // Convert the address
        if (qvmDnsRecord.contains("sAddress")) {
            this->sAddress = qvmDnsRecord["sAddress"].toString();
        }
        // Convert the direction
        if (qvmDnsRecord.contains("sDirection")) {
            this->sDirection = qvmDnsRecord["sDirection"].toString();
        }
        // Convert the type
        if (qvmDnsRecord.contains("sType")) {
            this->sType = qvmDnsRecord["sType"].toString();
        }
        // Convert the priority
        if (qvmDnsRecord.contains("iPriority")) {
            this->iPriority = qvmDnsRecord["iPriority"].toInt();
        }
        // Convert the active status
        if (qvmDnsRecord.contains("bEnabled")) {
            this->bEnabled = qvmDnsRecord["bEnabled"].toBool();
        }
    }
    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Set the properties
        qvmProperties.insert("iRecordId",  this->iRecordId);
        qvmProperties.insert("iAccountId", this->iAccountId);
        qvmProperties.insert("iDomainId",  this->iDomainId);
        qvmProperties.insert("sHostName",  this->sHostName);
        qvmProperties.insert("sAddress",   this->sAddress);
        qvmProperties.insert("sDirection", this->sDirection);
        qvmProperties.insert("sType",      this->sType);
        qvmProperties.insert("iPriority",  this->iPriority);
        qvmProperties.insert("bEnabled",   this->bEnabled);
        // Return the map
        return qvmProperties;
    }
    /**
     * @paragraph This function converts the structure to a prepared bound QSqlQuery statement
     * @brief toQuery
     * @param QSqlDatabase qsdConnection
     * @param QString sQuery
     * @return QSqlQuery
     */
    QSqlQuery toQuery(QSqlDatabase qsdConnection, QString sQuery) {
        // Setup the SQL Query
        QSqlQuery qsqDnsRecord(qsdConnection);
        // Prepare the SQL statement
        qsqDnsRecord.prepare(sQuery);
        // Grab the map
        QVariantMap qvmProperties(this->toMap());
        // Grab the iterators
        QVariantMap::ConstIterator itrProperty      = qvmProperties.constBegin();
        QVariantMap::ConstIterator itrPropertyEnd   = qvmProperties.constEnd();
        // Loop through the properties
        for (; itrProperty != itrPropertyEnd; ++itrProperty) {
            // Check for the identifier in the SQL statement
            if (sQuery.contains(QString(itrProperty.key()).prepend(":"), Qt::CaseSensitive)) {
                // Bind the value
                qsqDnsRecord.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqDnsRecord;
    }
};

/**
 * @paragraph This structure maps out a domain object that is compatible with ServerPanel
 * @brief The SpDomain struct
 */
struct SpDomain {
    int     iDomainId;
    int     iAccountId;
    QString sDomain;
    bool    bEnabled;
    // Initializer
    SpDomain() : iDomainId(0), iAccountId(0), bEnabled(true) {}
    SpDomain(QSqlRecord qsrDomain) {
        // Set the properties
        this->iDomainId  = qsrDomain.value("iDomainId").toInt();
        this->iAccountId = qsrDomain.value("iAccountId").toInt();
        this->sDomain    = qsrDomain.value("sDomain").toString();
        this->bEnabled   = qsrDomain.value("bEnabled").toBool();
    }
    SpDomain(QVariantMap qvmDomain) {
        // Convert the domain id
        if (qvmDomain.contains("iDomainId")) {
            this->iDomainId = qvmDomain["iDomainId"].toInt();
        }
        // Convert the account id
        if (qvmDomain.contains("iAccountId")) {
            this->iAccountId = qvmDomain["iAccountId"].toInt();
        }
        // Convert the domain name
        if (qvmDomain.contains("sDomain")) {
            this->sDomain = qvmDomain["sDomain"].toString();
        }
        // Convert the active status
        if (qvmDomain.contains("bEnabled")) {
            this->bEnabled = qvmDomain["bEnabled"].toBool();
        }
    }

    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Set the properties
        qvmProperties.insert("iDomainId",  this->iDomainId);
        qvmProperties.insert("iAccountId", this->iAccountId);
        qvmProperties.insert("sDomain",    this->sDomain);
        qvmProperties.insert("bEnabled",   this->bEnabled);
        // Return the map
        return qvmProperties;
    }
    /**
     * @paragraph This function converts the structure to a prepared bound QSqlQuery statement
     * @brief toQuery
     * @param QSqlDatabase qsdConnection
     * @param QString sQuery
     * @return QSqlQuery
     */
    QSqlQuery toQuery(QSqlDatabase qsdConnection, QString sQuery) {
        // Setup the SQL Query
        QSqlQuery qsqDomain(qsdConnection);
        // Prepare the SQL statement
        qsqDomain.prepare(sQuery);
        // Grab the map
        QVariantMap qvmProperties(this->toMap());
        // Grab the iterators
        QVariantMap::ConstIterator itrProperty      = qvmProperties.constBegin();
        QVariantMap::ConstIterator itrPropertyEnd   = qvmProperties.constEnd();
        // Loop through the properties
        for (; itrProperty != itrPropertyEnd; ++itrProperty) {
            // Check for the identifier in the SQL statement
            if (sQuery.contains(QString(itrProperty.key()).prepend(":"), Qt::CaseSensitive)) {
                // Bind the value
                qsqDomain.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqDomain;
    }
};

/**
 * @paragraph This structure maps out a mailbox object that is compatible with ServerPanel
 * @brief The SpMailBox struct
 */
struct SpMailBox {
    int     iMailBoxId;
    int     iAccountId;
    int     iDomainId;
    QString sUsername;
    QString sPassword;
    QString sName;
    QString sStorageDirectory;
    QString sStorageNode;
    QString sMailDirectory;
    int     iQuota;
    int     iBytes;
    QString sDomain;
    QString sTransport;
    QString sDepartment;
    QString sRank;
    QString sEmployeeId;
    bool    bEnableSmtp;
    bool    bEnableSecureSmtp;
    bool    bEnablePop3;
    bool    bEnableSecurePop3;
    bool    bEnableImap;
    bool    bEnableSecureImap;
    bool    bEnableDelivery;
    bool    bEnableSieveManagement;
    bool    bEnableSecureSieveManagement;
    bool    bEnableInternalDelivery;
    QString sDisclaimer;
    bool    bActive;
    QString sLocalPartition;
    // Initializer
    SpMailBox() : iMailBoxId(0), iAccountId(0), iDomainId(0), sStorageDirectory("/var/vmail"), sStorageNode("serverpanel"), sMailDirectory(), iQuota(1024), iBytes(0), sRank("normal"), sEmployeeId(), bEnableSmtp(true), bEnableSecureSmtp(true), bEnablePop3(true), bEnableSecurePop3(true), bEnableImap(true), bEnableSecureImap(true), bEnableDelivery(true), bEnableSieveManagement(true), bEnableSecureSieveManagement(true), bEnableInternalDelivery(true), bActive(true) {}
    SpMailBox(QSqlRecord qsrMailBox) {
        // Set the properties
        this->iMailBoxId                   = qsrMailBox.value("iMailBoxId").toInt();
        this->iAccountId                   = qsrMailBox.value("iAccountId").toInt();
        this->iDomainId                    = qsrMailBox.value("iDomainId").toInt();
        this->sUsername                    = qsrMailBox.value("sUsername").toString();
        this->sPassword                    = qsrMailBox.value("sPassword").toString();
        this->sName                        = qsrMailBox.value("sName").toString();
        this->sStorageDirectory            = qsrMailBox.value("sStorageDirectory").toString();
        this->sStorageNode                 = qsrMailBox.value("sStorageNode").toString();
        this->sMailDirectory               = qsrMailBox.value("sMailDirectory").toString();
        this->iQuota                       = qsrMailBox.value("iQuota").toInt();
        this->iBytes                       = qsrMailBox.value("iBytes").toInt();
        this->sDomain                      = qsrMailBox.value("sDomain").toString();
        this->sTransport                   = qsrMailBox.value("sTransport").toString();
        this->sDepartment                  = qsrMailBox.value("sDepartment").toString();
        this->sRank                        = qsrMailBox.value("sRank").toString();
        this->sEmployeeId                  = qsrMailBox.value("sEmployeeId").toString();
        this->bEnableSmtp                  = qsrMailBox.value("bEnableSmtp").toBool();
        this->bEnableSecureSmtp            = qsrMailBox.value("bEnableSecureSmtp").toBool();
        this->bEnablePop3                  = qsrMailBox.value("bEnablePop3").toBool();
        this->bEnableSecurePop3            = qsrMailBox.value("bEnableSecurePop3").toBool();
        this->bEnableImap                  = qsrMailBox.value("bEnableImap").toBool();
        this->bEnableSecureImap            = qsrMailBox.value("bEnableSecureImap").toBool();
        this->bEnableDelivery              = qsrMailBox.value("bEnableDelivery").toBool();
        this->bEnableSieveManagement       = qsrMailBox.value("bEnableSieveManagement").toBool();
        this->bEnableSecureSieveManagement = qsrMailBox.value("bEnableSecureSieveManagement").toBool();
        this->bEnableInternalDelivery      = qsrMailBox.value("bEnableInternalDelivery").toBool();
        this->sDisclaimer                  = qsrMailBox.value("sDisclaimer").toString();
        this->bActive                      = qsrMailBox.value("bActive").toBool();
        this->sLocalPartition              = qsrMailBox.value("sLocalPartition").toString();
    }
    SpMailBox(QVariantMap qvmMailBox) {
        // Convert the mailbox id
        if (qvmMailBox.contains("iMailBoxId")) {
            this->iMailBoxId = qvmMailBox["iMailBoxId"].toInt();
        }
        // Convert the account id
        if (qvmMailBox.contains("iAccountId")) {
            this->iAccountId = qvmMailBox["iAccountId"].toInt();
        }
        // Convert the domain id
        if (qvmMailBox.contains("iDomainId")) {
            this->iDomainId = qvmMailBox["iDomainId"].toInt();
        }
        // Convert the username
        if (qvmMailBox.contains("sUsername")) {
            this->sUsername = qvmMailBox["sUsername"].toString();
        }
        // Convert the password
        if (qvmMailBox.contains("sPassword")) {
            this->sPassword = qvmMailBox["sPassword"].toString();
        }
        // Convert the name
        if (qvmMailBox.contains("sName")) {
            this->sName = qvmMailBox["sName"].toString();
        }
        // Convert the storage directory
        if (qvmMailBox.contains("sStorageDirectory")) {
            this->sStorageDirectory = qvmMailBox["sStorageDirectory"].toString();
        }
        // Convert the storage node
        if (qvmMailBox.contains("sStorageNode")) {
            this->sStorageNode = qvmMailBox["sStorageNode"].toString();
        }
        // Convert the mail directory
        if (qvmMailBox.contains("sMailDirectory")) {
            this->sMailDirectory = qvmMailBox["sMailDirectory"].toString();
        }
        // Convert the quota
        if (qvmMailBox.contains("iQuota")) {
            this->iQuota = qvmMailBox["iQuota"].toInt();
        }
        // Convert the bytes
        if (qvmMailBox.contains("iBytes")) {
            this->iBytes = qvmMailBox["iBytes"].toInt();
        }
        // Convert the domain
        if (qvmMailBox.contains("sDomain")) {
            this->sDomain = qvmMailBox["sDomain"].toString();
        }
        // Convert the transport
        if (qvmMailBox.contains("sTransport")) {
            this->sTransport = qvmMailBox["sTransport"].toString();
        }
        // Convert the department
        if (qvmMailBox.contains("sDepartment")) {
            this->sDepartment = qvmMailBox["sDepartment"].toString();
        }
        // Convert the rank
        if (qvmMailBox.contains("sRank")) {
            this->sRank = qvmMailBox["sRank"].toString();
        }
        // Convert the employee id
        if (qvmMailBox.contains("sEmployeeId")) {
            this->sEmployeeId = qvmMailBox["sEmployeeId"].toString();
        }
        // Convert the SMTP enabler
        if (qvmMailBox.contains("bEnableSmtp")) {
            this->bEnableSmtp = qvmMailBox["bEnableSmtp"].toBool();
        }
        // Convert the secure SMTP enabler
        if (qvmMailBox.contains("bEnableSecureSmtp")) {
            this->bEnableSecureSmtp = qvmMailBox["bEnableSecureSmtp"].toBool();
        }
        // Convert the POP3 enabler
        if (qvmMailBox.contains("bEnablePop3")) {
            this->bEnablePop3 = qvmMailBox["bEnablePop3"].toBool();
        }
        // Convert the secure POP3 enabler
        if (qvmMailBox.contains("bEnableSecurePop3")) {
            this->bEnableSecurePop3 = qvmMailBox["bEnableSecurePop3"].toBool();
        }
        // Convert the IMAP enabler
        if (qvmMailBox.contains("bEnableImap")) {
            this->bEnableImap = qvmMailBox["bEnableImap"].toBool();
        }
        // Convert the secure IMAP enabler
        if (qvmMailBox.contains("bEnableSecureImap")) {
            this->bEnableSecureImap = qvmMailBox["bEnableSecureImap"].toBool();
        }
        // Convert the delivery enabler
        if (qvmMailBox.contains("bEnableDelivery")) {
            this->bEnableDelivery = qvmMailBox["bEnableDelivery"].toBool();
        }
        // Convert the sieve management enabler
        if (qvmMailBox.contains("bEnableSieveManagement")) {
            this->bEnableSieveManagement = qvmMailBox["bEnableSieveManagement"].toBool();
        }
        // Convert the secure sieve management enabler
        if (qvmMailBox.contains("bEnableSecureSieveManagement")) {
            this->bEnableSecureSieveManagement = qvmMailBox["bSecureSieveManagement"].toBool();
        }
        // Convert the internal delivery enabler
        if (qvmMailBox.contains("bEnableInternalDelivery")) {
            this->bEnableInternalDelivery = qvmMailBox["bEnableInternalDelivery"].toBool();
        }
        // Convert the disclaimer
        if (qvmMailBox.contains("sDisclaimer")) {
            this->sDisclaimer = qvmMailBox["sDisclaimer"].toString();
        }
        // Convert the actvie notator
        if (qvmMailBox.contains("bActive")) {
            this->bActive = qvmMailBox["bActive"].toBool();
        }
        // Convert the local partition
        if (qvmMailBox.contains("sLocalPartition")) {
            this->sLocalPartition = qvmMailBox["sLocalPartition"].toString();
        }
    }
    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Set the properties
        qvmProperties.insert("iMailBoxId",                   this->iMailBoxId);
        qvmProperties.insert("iAccountId",                   this->iAccountId);
        qvmProperties.insert("iDomainId",                    this->iDomainId);
        qvmProperties.insert("sUsername",                    this->sUsername);
        qvmProperties.insert("sPassword",                    this->sPassword);
        qvmProperties.insert("sName",                        this->sName);
        qvmProperties.insert("sStorageDirectory",            this->sStorageDirectory);
        qvmProperties.insert("sStorageNode",                 this->sStorageNode);
        qvmProperties.insert("sMailDirectory",               this->sMailDirectory);
        qvmProperties.insert("iQuota",                       this->iQuota);
        qvmProperties.insert("iBytes",                       this->iBytes);
        qvmProperties.insert("sDomain",                      this->sDomain);
        qvmProperties.insert("sTransport",                   this->sTransport);
        qvmProperties.insert("sDepartment",                  this->sDepartment);
        qvmProperties.insert("sRank",                        this->sRank);
        qvmProperties.insert("sEmployeeId",                  this->sEmployeeId);
        qvmProperties.insert("bEnableSmtp",                  this->bEnableSmtp);
        qvmProperties.insert("bEnableSecureSmtp",            this->bEnableSecureSmtp);
        qvmProperties.insert("bEnablePop3",                  this->bEnablePop3);
        qvmProperties.insert("bEnableSecurePop3",            this->bEnableSecurePop3);
        qvmProperties.insert("bEnableImap",                  this->bEnableImap);
        qvmProperties.insert("bEnableSecureImap",            this->bEnableSecureImap);
        qvmProperties.insert("bEnableDelivery",              this->bEnableDelivery);
        qvmProperties.insert("bEnableSieveManagement",       this->bEnableSieveManagement);
        qvmProperties.insert("bEnableSecureSieveManagement", this->bEnableSecureSieveManagement);
        qvmProperties.insert("bEnableInternalDelivery",      this->bEnableInternalDelivery);
        qvmProperties.insert("sDisclaimer",                  this->sDisclaimer);
        qvmProperties.insert("bActive",                      this->bActive);
        qvmProperties.insert("sLocalPartition",              this->sLocalPartition);
        // Return the map
        return qvmProperties;
    }
    /**
     * @paragraph This function converts the structure to a prepared bound QSqlQuery statement
     * @brief toQuery
     * @param QSqlDatabase qsdConnection
     * @param QString sQuery
     * @return QSqlQuery
     */
    QSqlQuery toQuery(QSqlDatabase qsdConnection, QString sQuery) {
        // Setup the SQL Query
        QSqlQuery qsqMailBox(qsdConnection);
        // Prepare the SQL statement
        qsqMailBox.prepare(sQuery);
        // Grab the map
        QVariantMap qvmProperties(this->toMap());
        // Grab the iterators
        QVariantMap::ConstIterator itrProperty      = qvmProperties.constBegin();
        QVariantMap::ConstIterator itrPropertyEnd   = qvmProperties.constEnd();
        // Loop through the properties
        for (; itrProperty != itrPropertyEnd; ++itrProperty) {
            // Check for the identifier in the SQL statement
            if (sQuery.contains(QString(itrProperty.key()).prepend(":"), Qt::CaseSensitive)) {
                // Bind the value
                qsqMailBox.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqMailBox;
    }
};

/**
 * @paragraph This structure maps out a mail domain object that is compatible with ServerPanel
 * @brief The SpMailDomain struct
 */
struct SpMailDomain {
    int     iMailDomainId;
    int     iAccountId;
    int     iDomainId;
    QString sDescription;
    QString sDisclaimer;
    int     iMaxQuota;
    int     iQuota;
    QString sTransport;
    bool    bBackupMx;
    int     iDefaultUserQuota;
    QString sDefaultPasswordScheme;
    int     iMinimumPasswordLength;
    int     iMaximumPasswordLength;
    bool    bActive;
    // Initializer
    SpMailDomain() : iMailDomainId(0), iAccountId(0), iDomainId(0), iMaxQuota(1024), iQuota(1024), sTransport("dovecot"), bBackupMx(false), iDefaultUserQuota(1024), sDefaultPasswordScheme("md5"), iMinimumPasswordLength(5), iMaximumPasswordLength(75), bActive(true) {}
    SpMailDomain(QSqlRecord qsrMailDomain) {
        // Convert the properties
        this->iMailDomainId          = qsrMailDomain.value("iMailDomainId").toInt();
        this->iAccountId             = qsrMailDomain.value("iAccountId").toInt();
        this->iDomainId              = qsrMailDomain.value("iDomainId").toInt();
        this->sDescription           = qsrMailDomain.value("sDescription").toString();
        this->sDisclaimer            = qsrMailDomain.value("sDisclaimer").toString();
        this->iMaxQuota              = qsrMailDomain.value("iMaxQuota").toInt();
        this->iQuota                 = qsrMailDomain.value("iQuota").toInt();
        this->sTransport             = qsrMailDomain.value("sTransport").toString();
        this->bBackupMx              = qsrMailDomain.value("bBackupMx").toBool();
        this->iDefaultUserQuota      = qsrMailDomain.value("iDefaultUserQuota").toInt();
        this->sDefaultPasswordScheme = qsrMailDomain.value("sDefaultPasswordScheme").toString();
        this->iMinimumPasswordLength = qsrMailDomain.value("iMinimumPasswordLength").toInt();
        this->iMaximumPasswordLength = qsrMailDomain.value("iMaximumPasswordLength").toInt();
        this->bActive                = qsrMailDomain.value("bActive").toBool();
    }
    SpMailDomain(QVariantMap qvmMailDomain) {
        // Convert the mail domain id
        if (qvmMailDomain.contains("iMailDomainId")) {
            this->iMailDomainId = qvmMailDomain["iMailDomainId"].toInt();
        }
        // Convert the account id
        if (qvmMailDomain.contains("iAccountId")) {
            this->iAccountId = qvmMailDomain["iAccountId"].toInt();
        }
        // Convert the domain id
        if (qvmMailDomain.contains("iDomainId")) {
            this->iDomainId = qvmMailDomain["iDomainId"].toInt();
        }
        // Convert the description
        if (qvmMailDomain.contains("sDescription")) {
            this->sDescription = qvmMailDomain["sDescription"].toString();
        }
        // Convert the disclaimer
        if (qvmMailDomain.contains("sDisclaimer")) {
            this->sDisclaimer = qvmMailDomain["sDisclaimer"].toString();
        }
        // Convert the max quota
        if (qvmMailDomain.contains("iMaxQuota")) {
            this->iMaxQuota = qvmMailDomain["iMaxQuota"].toInt();
        }
        // Convert the quota
        if (qvmMailDomain.contains("iQuota")) {
            this->iQuota = qvmMailDomain["iQuota"].toInt();
        }
        // Convert the transport
        if (qvmMailDomain.contains("sTransport")) {
            this->sTransport = qvmMailDomain["sTransport"].toString();
        }
        // Convert the backup mx notator
        if (qvmMailDomain.contains("bBackupMx")) {
            this->bBackupMx = qvmMailDomain["bBackupMx"].toBool();
        }
        // Convert the default user quota
        if (qvmMailDomain.contains("iDefaultUserQuota")) {
            this->iDefaultUserQuota = qvmMailDomain["iDefaultUserQuota"].toInt();
        }
        // Convert the default password scheme
        if (qvmMailDomain.contains("sDefaultPasswordScheme")) {
            this->sDefaultPasswordScheme = qvmMailDomain["sDefaultPasswordScheme"].toString();
        }
        // Convert the minimum password length
        if (qvmMailDomain.contains("iMinimumPasswordLength")) {
            this->iMinimumPasswordLength = qvmMailDomain["iMinimumPasswordLength"].toInt();
        }
        // Convert the maximum password length
        if (qvmMailDomain.contains("iMaximumPasswordLength")) {
            this->iMaximumPasswordLength = qvmMailDomain["iMaximumPasswordLength"].toInt();
        }
        // Convert the active state
        if (qvmMailDomain.contains("bActive")) {
            this->bActive = qvmMailDomain["bActive"].toBool();
        }
    }

    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Set the properties
        qvmProperties.insert("iMailDomainId",          this->iMailDomainId);
        qvmProperties.insert("iAccountId",             this->iAccountId);
        qvmProperties.insert("iDomainId",              this->iDomainId);
        qvmProperties.insert("sDescription",           this->sDescription);
        qvmProperties.insert("sDisclaimer",            this->sDisclaimer);
        qvmProperties.insert("iMaxQuota",              this->iMaxQuota);
        qvmProperties.insert("iQuota",                 this->iQuota);
        qvmProperties.insert("sTransport",             this->sTransport);
        qvmProperties.insert("bBackupMx",              this->bBackupMx);
        qvmProperties.insert("iDefaultUserQuota",      this->iDefaultUserQuota);
        qvmProperties.insert("sDefaultPasswordScheme", this->sDefaultPasswordScheme);
        qvmProperties.insert("iMinimumPasswordLength", this->iMinimumPasswordLength);
        qvmProperties.insert("iMaximumPasswordLength", this->iMaximumPasswordLength);
        qvmProperties.insert("bActive",                this->bActive);
        // Return the map
        return qvmProperties;
    }
    /**
     * @paragraph This function converts the structure to a prepared bound QSqlQuery statement
     * @brief toQuery
     * @param QSqlDatabase qsdConnection
     * @param QString sQuery
     * @return QSqlQuery
     */
    QSqlQuery toQuery(QSqlDatabase qsdConnection, QString sQuery) {
        // Setup the SQL Query
        QSqlQuery qsqMailDomain(qsdConnection);
        // Prepare the SQL statement
        qsqMailDomain.prepare(sQuery);
        // Grab the map
        QVariantMap qvmProperties(this->toMap());
        // Grab the iterators
        QVariantMap::ConstIterator itrProperty      = qvmProperties.constBegin();
        QVariantMap::ConstIterator itrPropertyEnd   = qvmProperties.constEnd();
        // Loop through the properties
        for (; itrProperty != itrPropertyEnd; ++itrProperty) {
            // Check for the identifier in the SQL statement
            if (sQuery.contains(QString(itrProperty.key()).prepend(":"), Qt::CaseSensitive)) {
                // Bind the value
                qsqMailDomain.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqMailDomain;
    }
};
#endif // SERVERPANELSTRUCTURES_H
