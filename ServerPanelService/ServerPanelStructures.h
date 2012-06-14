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
 * @brief SpAccount()
 * @return SpAccount
 */
struct SpAccount {
    QObject* mObject;
    // Initializer
    SpAccount() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("iAccountId",          int());
        this->mObject->setProperty("sUsername",           QString());
        this->mObject->setProperty("sPassword",           QString());
        this->mObject->setProperty("sEmailAddress",       QString());
        this->mObject->setProperty("sFirstName",          QString());
        this->mObject->setProperty("sLastName",           QString());
        this->mObject->setProperty("sPhoneNumber",        QString());
        this->mObject->setProperty("sHomeDirectory",      QString());
        this->mObject->setProperty("sStreetAddress",      QString());
        this->mObject->setProperty("sStreetAddressExtra", QString());
        this->mObject->setProperty("sCity",               QString());
        this->mObject->setProperty("sState",              QString());
        this->mObject->setProperty("sPostalCode",         QString());
        this->mObject->setProperty("sCountry",            QString());
        this->mObject->setProperty("bEnabled",            bool());
    }
    /**
     * @paragraph This routine creates a structure out of a QSqlRecord object
     * @brief SpAccount()
     * @param QSqlRecord qsrAccount
     * @return SpAccount
     */
    SpAccount(QSqlRecord qsrAccount) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iAccountId",          qsrAccount.value("iAccountId").toInt());
        this->mObject->setProperty("sUsername",           qsrAccount.value("sUsername").toString());
        this->mObject->setProperty("sPassword",           qsrAccount.value("sPassword").toString());
        this->mObject->setProperty("sEmailAddress",       qsrAccount.value("sEmailAddress").toString());
        this->mObject->setProperty("sFirstName",          qsrAccount.value("sFirstName").toString());
        this->mObject->setProperty("sLastName",           qsrAccount.value("sLastName").toString());
        this->mObject->setProperty("sPhoneNumber",        qsrAccount.value("sPhoneNumber").toString());
        this->mObject->setProperty("sHomeDirectory",      qsrAccount.value("sHomeDirectory").toString());
        this->mObject->setProperty("sStreetAddress",      qsrAccount.value("sStreetAddress").toString());
        this->mObject->setProperty("sStreetAddressExtra", qsrAccount.value("sStreetAddressExtra").toString());
        this->mObject->setProperty("sCity",               qsrAccount.value("sCity").toString());
        this->mObject->setProperty("sState",              qsrAccount.value("sState").toString());
        this->mObject->setProperty("sPostalCode",         qsrAccount.value("sPostalCode").toString());
        this->mObject->setProperty("sCountry",            qsrAccount.value("sCountry").toString());
        this->mObject->setProperty("bEnabled",            qsrAccount.value("bEnabled").toBool());
    }
    /**
     * @paragraph This routine creates a structure out of a QVariantMap
     * @brief SpAccount()
     * @param QVariantMap qvmAccount
     * @return SpAccount
     */
    SpAccount(QVariantMap qvmAccount) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmAccount.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmAccount.constEnd();
        // Iterate through the properties
        for (; itrProperty != itrLastProperty; ++itrProperty) {
            // Set a name placeholder
            QByteArray qbaPropertyName;
            // Set the name
            qbaPropertyName.append(itrProperty.key());
            // Set the property
            this->mObject->setProperty(qbaPropertyName, itrProperty.value());
        }
    }
    /**
     * @paragraph This routine adds or updates a property
     * @brief saveProperty()
     * @param QString sPropertyName
     * @param QVariant qvValue
     * @return void
     */
    void saveProperty(QString sPropertyName, QVariant qvValue) {
        // Set or update the property
        this->mObject->setProperty(sPropertyName.toLatin1(), qvValue);
    }
    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Loop through the properties
        foreach (QString sPropertyName, this->mObject->dynamicPropertyNames()) {
            // Add the property to the map
            qvmProperties.insert(sPropertyName, this->mObject->property(sPropertyName.toLatin1()));
        }
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
    QObject* mObject;
    /**
     * @paragraph This is just the default constructor
     * @brief SpDnsRecord()
     * @return SpAccount()
     */
    SpDnsRecord() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("iRecordId", int());
        this->mObject->setProperty("iAccountId",int());
        this->mObject->setProperty("iDomainId", int());
        this->mObject->setProperty("sHostName", QString());
        this->mObject->setProperty("sAddress",  QString());
        this->mObject->setProperty("sDirection",QString());
        this->mObject->setProperty("sType",     QString());
        this->mObject->setProperty("iPriority", int());
        this->mObject->setProperty("bEnabled",  bool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpDnsRecord()
     * @param QSqlRecord qsrDnsRecord
     * @return SpDnsRecord
     */
    SpDnsRecord(QSqlRecord qsrDnsRecord) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iRecordId", qsrDnsRecord.value("iRecordId").toInt());
        this->mObject->setProperty("iAccountId",qsrDnsRecord.value("iAccountId").toInt());
        this->mObject->setProperty("iDomainId", qsrDnsRecord.value("iDomainId").toInt());
        this->mObject->setProperty("sHostName", qsrDnsRecord.value("sHostName").toString());
        this->mObject->setProperty("sAddress",  qsrDnsRecord.value("sAddress").toString());
        this->mObject->setProperty("sDirection",qsrDnsRecord.value("sDirection").toString());
        this->mObject->setProperty("sType",     qsrDnsRecord.value("sType").toString());
        this->mObject->setProperty("iPriority", qsrDnsRecord.value("iPriority").toInt());
        this->mObject->setProperty("bEnabled",  qsrDnsRecord.value("bEnabled").toBool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpDnsRecord()
     * @param QVariantMap qvmDnsRecord
     * @return SpDnsRecord
     */
    SpDnsRecord(QVariantMap qvmDnsRecord) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmDnsRecord.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmDnsRecord.constEnd();
        // Iterate through the properties
        for (; itrProperty != itrLastProperty; ++itrProperty) {
            // Set a name placeholder
            QByteArray qbaPropertyName;
            // Set the name
            qbaPropertyName.append(itrProperty.key());
            // Set the property
            this->mObject->setProperty(qbaPropertyName, itrProperty.value());
        }
    }
    /**
     * @paragraph This routine adds or updates a property
     * @brief saveProperty()
     * @param QString sPropertyName
     * @param QVariant qvValue
     * @return void
     */
    void saveProperty(QString sPropertyName, QVariant qvValue) {
        // Set or update the property
        this->mObject->setProperty(sPropertyName.toLatin1(), qvValue);
    }
    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Loop through the properties
        foreach (QString sPropertyName, this->mObject->dynamicPropertyNames()) {
            // Add the property to the map
            qvmProperties.insert(sPropertyName, this->mObject->property(sPropertyName.toLatin1()));
        }
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
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpDomain()
     * @return SpDomain
     */
    SpDomain() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("iDomainId",  int());
        this->mObject->setProperty("iAccountId", int());
        this->mObject->setProperty("sDomain",    QString());
        this->mObject->setProperty("bEnabled",   bool());
    }
    /**
     * @paragraph This constrcutor creates a structure out of a QSqlRecord
     * @brief SpDomain()
     * @param QSqlRecord qsrDomain
     * @return SpDomain
     */
    SpDomain(QSqlRecord qsrDomain) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iDomainId",  qsrDomain.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId", qsrDomain.value("iAccountId").toInt());
        this->mObject->setProperty("sDomain",    qsrDomain.value("sDomain").toString());
        this->mObject->setProperty("bEnabled",   qsrDomain.value("bEnabled").toBool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpDomain()
     * @param QVariantMap qvmDomain
     * @return SpDomain
     */
    SpDomain(QVariantMap qvmDomain) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmDomain.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmDomain.constEnd();
        // Iterate through the properties
        for (; itrProperty != itrLastProperty; ++itrProperty) {
            // Set a name placeholder
            QByteArray qbaPropertyName;
            // Set the name
            qbaPropertyName.append(itrProperty.key());
            // Set the property
            this->mObject->setProperty(qbaPropertyName, itrProperty.value());
        }
    }
    /**
     * @paragraph This routine adds or updates a property
     * @brief saveProperty()
     * @param QString sPropertyName
     * @param QVariant qvValue
     * @return void
     */
    void saveProperty(QString sPropertyName, QVariant qvValue) {
        // Set or update the property
        this->mObject->setProperty(sPropertyName.toLatin1(), qvValue);
    }
    /**
     * @paragraph This function simply converts the properties in the a quick loopable QSqlQuery property map
     * @brief toMap
     * @return QVariantMap
     */
    QVariantMap toMap() {
        // Create a map placeholder
        QVariantMap qvmProperties;
        // Loop through the properties
        foreach (QString sPropertyName, this->mObject->dynamicPropertyNames()) {
            // Add the property to the map
            qvmProperties.insert(sPropertyName, this->mObject->property(sPropertyName.toLatin1()));
        }
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
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpMailBox()
     * @return SpMailBox
     */
    SpMailBox() : mObject(new QObject()) {

    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpMailBox()
     * @param QSqlRecord qsr
     *
     */
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
     * @paragraph This routine adds or updates a property
     * @brief saveProperty()
     * @param QString sPropertyName
     * @param QVariant qvValue
     * @return void
     */
    void saveProperty(QString sPropertyName, QVariant qvValue) {
        // Set or update the property
        this->mObject->setProperty(sPropertyName.toLatin1(), qvValue);
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
     * @paragraph This routine adds or updates a property
     * @brief saveProperty()
     * @param QString sPropertyName
     * @param QVariant qvValue
     * @return void
     */
    void saveProperty(QString sPropertyName, QVariant qvValue) {
        // Set or update the property
        this->mObject->setProperty(sPropertyName.toLatin1(), qvValue);
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
