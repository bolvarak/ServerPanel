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
        this->mObject->setProperty("sStreetAddress",      QString());
        this->mObject->setProperty("sStreetAddressExtra", QString());
        this->mObject->setProperty("sState",              QString());
        this->mObject->setProperty("sPostalCode",         QString());
        this->mObject->setProperty("sCountry",            QString());
        this->mObject->setProperty("iAccountLevel",       int());
        this->mObject->setProperty("sCity",               QString());
        this->mObject->setProperty("sHomeDirectory",      QString());
        this->mObject->setProperty("sApiKey",             QString());
        this->mObject->setProperty("bEnabled",            bool());
        this->mObject->setProperty("sCreated",            QString());
        this->mObject->setProperty("sModified",           QString());
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
        this->mObject->setProperty("sStreetAddress",      qsrAccount.value("sStreetAddress").toString());
        this->mObject->setProperty("sStreetAddressExtra", qsrAccount.value("sStreetAddressExtra").toString());
        this->mObject->setProperty("sState",              qsrAccount.value("sState").toString());
        this->mObject->setProperty("sPostalCode",         qsrAccount.value("sPostalCode").toString());
        this->mObject->setProperty("sCountry",            qsrAccount.value("sCountry").toString());
        this->mObject->setProperty("iAccountLevel",       qsrAccount.value("iAccountLevel").toInt());
        this->mObject->setProperty("sCity",               qsrAccount.value("sCity").toString());
        this->mObject->setProperty("sHomeDirectory",      qsrAccount.value("sHomeDirectory").toString());
        this->mObject->setProperty("sApiKey",             qsrAccount.value("sApiKey").toString());
        this->mObject->setProperty("bEnabled",            qsrAccount.value("bEnabled").toBool());
        this->mObject->setProperty("sCreated",            qsrAccount.value("sCreated").toString());
        this->mObject->setProperty("sModified",           qsrAccount.value("sModified").toString());
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
     * @paragraph This routine grabs a property from the structure
     * @brief getProperty()
     * @param QString sPropertyName
     * @return QVariant
     */
    QVariant getProperty(QString sPropertyName) {
        // Grab and return the property
        return this->mObject->property(sPropertyName.toLatin1());
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
        this->mObject->setProperty("iRecordId",  int());
        this->mObject->setProperty("iAccountId", int());
        this->mObject->setProperty("iDomainId",  int());
        this->mObject->setProperty("sHostName",  QString());
        this->mObject->setProperty("sDirection", QString());
        this->mObject->setProperty("sType",      QString());
        this->mObject->setProperty("iPriority",  int());
        this->mObject->setProperty("sAddress",   QString());
        this->mObject->setProperty("sCreated",   QString());
        this->mObject->setProperty("sModified",  QString());
        this->mObject->setProperty("bEnabled",   bool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpDnsRecord()
     * @param QSqlRecord qsrDnsRecord
     * @return SpDnsRecord
     */
    SpDnsRecord(QSqlRecord qsrDnsRecord) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iRecordId",  qsrDnsRecord.value("iRecordId").toInt());
        this->mObject->setProperty("iAccountId", qsrDnsRecord.value("iAccountId").toInt());
        this->mObject->setProperty("iDomainId",  qsrDnsRecord.value("iDomainId").toInt());
        this->mObject->setProperty("sHostName",  qsrDnsRecord.value("sHostName").toString());
        this->mObject->setProperty("sDirection", qsrDnsRecord.value("sDirection").toString());
        this->mObject->setProperty("sType",      qsrDnsRecord.value("sType").toString());
        this->mObject->setProperty("iPriority",  qsrDnsRecord.value("iPriority").toInt());
        this->mObject->setProperty("sAddress",   qsrDnsRecord.value("sAddress").toString());
        this->mObject->setProperty("sCreated",   qsrDnsRecord.value("sCreated").toString());
        this->mObject->setProperty("sModified",  qsrDnsRecord.value("sModified").toString());
        this->mObject->setProperty("bEnabled",   qsrDnsRecord.value("bEnabled").toBool());
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
     * @paragraph This routine grabs a property from the structure
     * @brief getProperty()
     * @param QString sPropertyName
     * @return QVariant
     */
    QVariant getProperty(QString sPropertyName) {
        // Grab and return the property
        return this->mObject->property(sPropertyName.toLatin1());
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
     * @paragraph This routine grabs a property from the structure
     * @brief getProperty()
     * @param QString sPropertyName
     * @return QVariant
     */
    QVariant getProperty(QString sPropertyName) {
        // Grab and return the property
        return this->mObject->property(sPropertyName.toLatin1());
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
        // Initialize the properties
        this->mObject->setProperty("iMailBoxId",                   int());
        this->mObject->setProperty("iAccountId",                   int());
        this->mObject->setProperty("iDomainId",                    int());
        this->mObject->setProperty("sUsername",                    QString());
        this->mObject->setProperty("sPassword",                    QString());
        this->mObject->setProperty("sName",                        QString());
        this->mObject->setProperty("sStorageDirectory",            QString());
        this->mObject->setProperty("sStorageNode",                 QString());
        this->mObject->setProperty("sMailDirectory",               QString());
        this->mObject->setProperty("iQuota",                       int());
        this->mObject->setProperty("iBytes",                       int());
        this->mObject->setProperty("sDomain",                      QString());
        this->mObject->setProperty("sTransport",                   QString());
        this->mObject->setProperty("sDepartment",                  QString());
        this->mObject->setProperty("sRank",                        QString());
        this->mObject->setProperty("sEmployeeId",                  QString());
        this->mObject->setProperty("bEnableSmtp",                  bool());
        this->mObject->setProperty("bEnableSecureSmtp",            bool());
        this->mObject->setProperty("bEnablePop3",                  bool());
        this->mObject->setProperty("bEnableSecurePop3",            bool());
        this->mObject->setProperty("bEnableImap",                  bool());
        this->mObject->setProperty("bEnableSecureImap",            bool());
        this->mObject->setProperty("bEnableDelivery",              bool());
        this->mObject->setProperty("bEnableSieveManagement",       bool());
        this->mObject->setProperty("bEnableSecureSieveManagement", bool());
        this->mObject->setProperty("bEnableInternalDelivery",      bool());
        this->mObject->setProperty("sDisclaimer",                  QString());
        this->mObject->setProperty("bActive",                      bool());
        this->mObject->setProperty("sLocalPartition",              QString());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord
     * @brief SpMailBox()
     * @param QSqlRecord qsr
     * @return SpMailBox
     */
    SpMailBox(QSqlRecord qsrMailBox) {
        // Set the properties
        this->mObject->setProperty("iMailBoxId",                   qsrMailBox.value("iMailBoxId").toInt());
        this->mObject->setProperty("iAccountId",                   qsrMailBox.value("iAccountId").toInt());
        this->mObject->setProperty("iDomainId",                    qsrMailBox.value("iDomainId").toInt());
        this->mObject->setProperty("sUsername",                    qsrMailBox.value("sUsername").toString());
        this->mObject->setProperty("sPassword",                    qsrMailBox.value("sPassword").toString());
        this->mObject->setProperty("sName",                        qsrMailBox.value("sName").toString());
        this->mObject->setProperty("sStorageDirectory",            qsrMailBox.value("sStorageDirectory").toString());
        this->mObject->setProperty("sStorageNode",                 qsrMailBox.value("sStorageNode").toString());
        this->mObject->setProperty("sMailDirectory",               qsrMailBox.value("sMailDirectory").toString());
        this->mObject->setProperty("iQuota",                       qsrMailBox.value("iQuota").toInt());
        this->mObject->setProperty("iBytes",                       qsrMailBox.value("iBytes").toInt());
        this->mObject->setProperty("sDomain",                      qsrMailBox.value("sDomain").toString());
        this->mObject->setProperty("sTransport",                   qsrMailBox.value("sTransport").toString());
        this->mObject->setProperty("sDepartment",                  qsrMailBox.value("sDepartment").toString());
        this->mObject->setProperty("sRank",                        qsrMailBox.value("sRank").toString());
        this->mObject->setProperty("sEmployeeId",                  qsrMailBox.value("sEmployeeId").toString());
        this->mObject->setProperty("bEnableSmtp",                  qsrMailBox.value("bEnableSmtp").toBool());
        this->mObject->setProperty("bEnableSecureSmtp",            qsrMailBox.value("bEnableSecureSmtp").toBool());
        this->mObject->setProperty("bEnablePop3",                  qsrMailBox.value("bEnablePop3").toBool());
        this->mObject->setProperty("bEnableSecurePop3",            qsrMailBox.value("bEnableSecurePop3").toBool());
        this->mObject->setProperty("bEnableImap",                  qsrMailBox.value("bEnableImap").toBool());
        this->mObject->setProperty("bEnableSecureImap",            qsrMailBox.value("bEnableSecureImap").toBool());
        this->mObject->setProperty("bEnableDelivery",              qsrMailBox.value("bEnableDelivery").toBool());
        this->mObject->setProperty("bEnableSieveManagement",       qsrMailBox.value("bEnableSieveManagement").toBool());
        this->mObject->setProperty("bEnableSecureSieveManagement", qsrMailBox.value("bEnableSecureSieveManagement").toBool());
        this->mObject->setProperty("bEnableInternalDelivery",      qsrMailBox.value("bEnableInternalDelivery").toBool());
        this->mObject->setProperty("sDisclaimer",                  qsrMailBox.value("sDisclaimer").toString());
        this->mObject->setProperty("bActive",                      qsrMailBox.value("bActive").toBool());
        this->mObject->setProperty("sLocalPartition",              qsrMailBox.value("sLocalPartition").toString());
    }
    /**
     * @paragraph This creates a structure out of a QVariantMap
     * @brief SpMailBox()
     * @param QVariantMap qvmMailBox
     * @return SpMailBox
     */
    SpMailBox(QVariantMap qvmMailBox) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmMailBox.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmMailBox.constEnd();
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
     * @paragraph This routine grabs a property from the structure
     * @brief getProperty()
     * @param QString sPropertyName
     * @return QVariant
     */
    QVariant getProperty(QString sPropertyName) {
        // Grab and return the property
        return this->mObject->property(sPropertyName.toLatin1());
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
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpMailDomain()
     * @return SpMailDomain
     */
    SpMailDomain() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("iMailDomainId",          int());
        this->mObject->setProperty("iAccountId",             int());
        this->mObject->setProperty("iDomainId",              int());
        this->mObject->setProperty("sDescription",           QString());
        this->mObject->setProperty("sDisclaimer",            QString());
        this->mObject->setProperty("iMaxQuota",              int());
        this->mObject->setProperty("iQuota",                 int());
        this->mObject->setProperty("sTransport",             QString());
        this->mObject->setProperty("bBackupMx",              bool());
        this->mObject->setProperty("iDefaultUserQuota",      int());
        this->mObject->setProperty("sDefaultPasswordScheme", QString());
        this->mObject->setProperty("iMinimumPasswordLength", int());
        this->mObject->setProperty("iMaximumPasswordLength", int());
        this->mObject->setProperty("bActive",                bool());
    }
    /**
     * @paragraph This structure creates a structure out of a QSqlRecord
     * @brief SpMailDomain()
     * @param QSqlRecord qsrMailDomain
     * @return SpMailDomain
     */
    SpMailDomain(QSqlRecord qsrMailDomain) : mObject(new QObject()) {
        // Convert the properties
        this->mObject->setProperty("iMailDomainId",          qsrMailDomain.value("iMailDomainId").toInt());
        this->mObject->setProperty("iAccountId",             qsrMailDomain.value("iAccountId").toInt());
        this->mObject->setProperty("iDomainId",              qsrMailDomain.value("iDomainId").toInt());
        this->mObject->setProperty("sDescription",           qsrMailDomain.value("sDescription").toString());
        this->mObject->setProperty("sDisclaimer",            qsrMailDomain.value("sDisclaimer").toString());
        this->mObject->setProperty("iMaxQuota",              qsrMailDomain.value("iMaxQuota").toInt());
        this->mObject->setProperty("iQuota",                 qsrMailDomain.value("iQuota").toInt());
        this->mObject->setProperty("sTransport",             qsrMailDomain.value("sTransport").toString());
        this->mObject->setProperty("bBackupMx",              qsrMailDomain.value("bBackupMx").toBool());
        this->mObject->setProperty("iDefaultUserQuota",      qsrMailDomain.value("iDefaultUserQuota").toInt());
        this->mObject->setProperty("sDefaultPasswordScheme", qsrMailDomain.value("sDefaultPasswordScheme").toString());
        this->mObject->setProperty("iMinimumPasswordLength", qsrMailDomain.value("iMinimumPasswordLength").toInt());
        this->mObject->setProperty("iMaximumPasswordLength", qsrMailDomain.value("iMaximumPasswordLength").toInt());
        this->mObject->setProperty("bActive",                qsrMailDomain.value("bActive").toBool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpMailDomain()
     * @param QVariantMap qvmMailDomain
     * @return SpMailDomain
     */
    SpMailDomain(QVariantMap qvmMailDomain) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmMailDomain.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmMailDomain.constEnd();
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
     * @paragraph This routine grabs a property from the structure
     * @brief getProperty()
     * @param QString sPropertyName
     * @return QVariant
     */
    QVariant getProperty(QString sPropertyName) {
        // Grab and return the property
        return this->mObject->property(sPropertyName.toLatin1());
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
