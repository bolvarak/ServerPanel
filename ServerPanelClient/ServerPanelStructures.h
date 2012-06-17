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
#include <Json.h>
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
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpAccount()
     * @param QString sJson
     * @return SpAccount
     */
    SpAccount(QString sJson) {
        // Decode and return the structure
        SpAccount(QtJson::Json::parse(sJson).toMap());
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
 * @paragraph This structure maps out an Alias Domain object that is compatible with ServerPanel
 * @brief The SpAliasDomain struct
 */
struct SpAliasDomain {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpAliasDomain()
     * @return SpAliasDomain
     */
    SpAliasDomain() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sAliasDomain",   QString());
        this->mObject->setProperty("sActualDomain",  QString());
        this->mObject->setProperty("sCreated",       QString());
        this->mObject->setProperty("sModified",      QString());
        this->mObject->setProperty("bActive",        bool());
        this->mObject->setProperty("iAccountId",     int());
        this->mObject->setProperty("iAliasDomainId", int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpAliasDomain()
     * @param QString sJson
     * @return SpAliasDomain
     */
    SpAliasDomain(QString sJson) {
        // Decode and return the structure
        SpAliasDomain(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpAliasDomain()
     * @param QSqlRecord qsrAliasDomain
     * @return SpAliasDomain
     */
    SpAliasDomain(QSqlRecord qsrAliasDomain) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sAliasDomain",   qsrAliasDomain.value("sAliasDomain").toString());
        this->mObject->setProperty("sActualDomain",  qsrAliasDomain.value("sActualDomain").toString());
        this->mObject->setProperty("sCreated",       qsrAliasDomain.value("sCreated").toString());
        this->mObject->setProperty("sModified",      qsrAliasDomain.value("sModified").toString());
        this->mObject->setProperty("bActive",        qsrAliasDomain.value("bActive").toBool());
        this->mObject->setProperty("iAccountId",     qsrAliasDomain.value("iAccountId").toInt());
        this->mObject->setProperty("iAliasDomainId", qsrAliasDomain.value("iAliasDomainId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpAliasDomain()
     * @param QVariantMap qvmAliasDomain
     * @return SpAliasDomain
     */
    SpAliasDomain(QVariantMap qvmAliasDomain) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmAliasDomain.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmAliasDomain.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqAliasDomain(qsdConnection);
        // Prepare the SQL statement
        qsqAliasDomain.prepare(sQuery);
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
                qsqAliasDomain.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqAliasDomain;
    }
};

/**
 * @paragraph This structure maps out an Alias object that is compatible with ServerPanel
 * @brief The SpAlias struct
 */
struct SpAlias {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpAlias()
     * @return SpAlias
     */
    SpAlias() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sFormAddress",  QString());
        this->mObject->setProperty("sToAddress",    QString());
        this->mObject->setProperty("sName",         QString());
        this->mObject->setProperty("sModerators",   QString());
        this->mObject->setProperty("sAccessPolicy", QString());
        this->mObject->setProperty("sDomain",       QString());
        this->mObject->setProperty("sCreated",      QString());
        this->mObject->setProperty("sModified",     QString());
        this->mObject->setProperty("sExpires",      QString());
        this->mObject->setProperty("bActive",       bool());
        this->mObject->setProperty("iAliasId",      int());
        this->mObject->setProperty("iDomainId",     int());
        this->mObject->setProperty("iAccountId",    int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpAlias()
     * @param QString sJson
     * @return SpAlias
     */
    SpAlias(QString sJson) {
        // Decode and return the structure
        SpAlias(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpAlias()
     * @param QSqlRecord qsrAlias
     * @return SpAlias
     */
    SpAlias(QSqlRecord qsrAlias) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sFormAddress",  qsrAlias.value("sFormAddress").toString());
        this->mObject->setProperty("sToAddress",    qsrAlias.value("sToAddress").toString());
        this->mObject->setProperty("sName",         qsrAlias.value("sName").toString());
        this->mObject->setProperty("sModerators",   qsrAlias.value("sModerators").toString());
        this->mObject->setProperty("sAccessPolicy", qsrAlias.value("sAccessPolicy").toString());
        this->mObject->setProperty("sDomain",       qsrAlias.value("sDomain").toString());
        this->mObject->setProperty("sCreated",      qsrAlias.value("sCreated").toString());
        this->mObject->setProperty("sModified",     qsrAlias.value("sModified").toString());
        this->mObject->setProperty("sExpires",      qsrAlias.value("sExpires").toString());
        this->mObject->setProperty("bActive",       qsrAlias.value("bActive").toBool());
        this->mObject->setProperty("iAliasId",      qsrAlias.value("iAliasId").toInt());
        this->mObject->setProperty("iDomainId",     qsrAlias.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",    qsrAlias.value("iAccountId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpAlias()
     * @param QVariantMap qvmAlias
     * @return SpAlias
     */
    SpAlias(QVariantMap qvmAlias) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmAlias.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmAlias.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqAlias(qsdConnection);
        // Prepare the SQL statement
        qsqAlias.prepare(sQuery);
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
                qsqAlias.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqAlias;
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
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpDnsRecord()
     * @param QString sJson
     * @return SpDnsRecord
     */
    SpDnsRecord(QString sJson) {
        // Decode and return the structure
        SpDnsRecord(QtJson::Json::parse(sJson).toMap());
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        this->mObject->setProperty("iDomainId",   int());
        this->mObject->setProperty("iAccountId",  int());
        this->mObject->setProperty("sDomain",     QString());
        this->mObject->setProperty("iTtl",        int());
        this->mObject->setProperty("iSerial",     int());
        this->mObject->setProperty("sZoneFile",   QString());
        this->mObject->setProperty("sCreated",    QString());
        this->mObject->setProperty("sModified",   QString());
        this->mObject->setProperty("bEnabled",    bool());
        this->mObject->setProperty("sNameServer", QString());
        this->mObject->setProperty("iRefresh",    int());
        this->mObject->setProperty("iRetry",      int());
        this->mObject->setProperty("iExpire",     int());
        this->mObject->setProperty("iMinimum",    int());
        this->mObject->setProperty("sRoot",       QString());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpDomain()
     * @param QString sJson
     * @return SpDomain
     */
    SpDomain(QString sJson) {
        // Decode and return the structure
        SpDomain(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constrcutor creates a structure out of a QSqlRecord
     * @brief SpDomain()
     * @param QSqlRecord qsrDomain
     * @return SpDomain
     */
    SpDomain(QSqlRecord qsrDomain) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iDomainId",   qsrDomain.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",  qsrDomain.value("iAccountId").toInt());
        this->mObject->setProperty("sDomain",     qsrDomain.value("sDomain").toString());
        this->mObject->setProperty("iTtl",        qsrDomain.value("iTtl").toInt());
        this->mObject->setProperty("iSerial",     qsrDomain.value("iSerial").toInt());
        this->mObject->setProperty("sZoneFile",   qsrDomain.value("sZoneFile").toString());
        this->mObject->setProperty("sCreated",    qsrDomain.value("sCreated").toString());
        this->mObject->setProperty("sModified",   qsrDomain.value("sModified").toString());
        this->mObject->setProperty("bEnabled",    qsrDomain.value("bEnabled").toBool());
        this->mObject->setProperty("sNameServer", qsrDomain.value("sNameServer").toString());
        this->mObject->setProperty("iRefresh",    qsrDomain.value("iRefresh").toInt());
        this->mObject->setProperty("iRetry",      qsrDomain.value("iRetry").toInt());
        this->mObject->setProperty("iExpire",     qsrDomain.value("iExpire").toInt());
        this->mObject->setProperty("iMinimum",    qsrDomain.value("iMinimum").toInt());
        this->mObject->setProperty("sRoot",       qsrDomain.value("sRoot").toString());
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
 * @paragraph This structure maps out a local account object that is compatible with ServerPanel
 * @brief The SpLocalAccount struct
 */
struct SpLocalAccount {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpLocalAccount()
     * @return SpLocalAccount
     */
    SpLocalAccount() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("iAccountId",            int());
        this->mObject->setProperty("iServerId",             int());
        this->mObject->setProperty("sUsername",             QString());
        this->mObject->setProperty("sPassword",             QString());
        this->mObject->setProperty("sCreated",              QString());
        this->mObject->setProperty("sModified",             QString());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpLocalAccount()
     * @param QString sJson
     * @return SpLocalAccount
     */
    SpLocalAccount(QString sJson) {
        // Decode and return the structure
        SpLocalAccount(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constrcutor creates a structure out of a QSqlRecord
     * @brief SpLocalAccount()
     * @param QSqlRecord qsrLocalAccount
     * @return SpLocalAccount
     */
    SpLocalAccount(QSqlRecord qsrLocalAccount) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iAccountId",            qsrLocalAccount.value("iAccountId").toInt());
        this->mObject->setProperty("iServerId",             qsrLocalAccount.value("iServerId").toInt());
        this->mObject->setProperty("sUsername",             qsrLocalAccount.value("sUsername").toString());
        this->mObject->setProperty("sPassword",             qsrLocalAccount.value("sPassword").toString());
        this->mObject->setProperty("sCreated",              qsrLocalAccount.value("sCreated").toString());
        this->mObject->setProperty("sModified",             qsrLocalAccount.value("sModified").toString());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpLocalAccount()
     * @param QVariantMap qvmLocalAccount
     * @return SpLocalAccount
     */
    SpLocalAccount(QVariantMap qvmLocalAccount) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmLocalAccount.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmLocalAccount.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqLocalAccount(qsdConnection);
        // Prepare the SQL statement
        qsqLocalAccount.prepare(sQuery);
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
                qsqLocalAccount.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqLocalAccount;
    }
};

/**
 * @paragraph This structure maps out a local server object that is compatible with ServerPanel
 * @brief The SpLocalServer struct
 */
struct SpLocalServer {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpLocalServer()
     * @return SpLocalServer
     */
    SpLocalServer() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("iServerId", int());
        this->mObject->setProperty("sName",     QString());
        this->mObject->setProperty("sAddress",  QString());
        this->mObject->setProperty("iPort",     int());
        this->mObject->setProperty("sCreated",  QString());
        this->mObject->setProperty("sModified", QString());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpLocalServer()
     * @param QString sJson
     * @return SpLocalServer
     */
    SpLocalServer(QString sJson) {
        // Decode and return the structure
        SpLocalServer(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constrcutor creates a structure out of a QSqlRecord
     * @brief SpLocalAccount()
     * @param QSqlRecord qsrLocalAccount
     * @return SpLocalAccount
     */
    SpLocalServer(QSqlRecord qsrLocalServer) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("iServerId", qsrLocalServer.value("iServerId").toInt());
        this->mObject->setProperty("sName",     qsrLocalServer.value("sName").toString());
        this->mObject->setProperty("sAddress",  qsrLocalServer.value("sAddress").toString());
        this->mObject->setProperty("iPort",     qsrLocalServer.value("iPort").toInt());
        this->mObject->setProperty("sCreated",  qsrLocalServer.value("sCreated").toString());
        this->mObject->setProperty("sModified", qsrLocalServer.value("sModified").toString());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpLocalServer()
     * @param QVariantMap qvmLocalServer
     * @return SpLocalServer
     */
    SpLocalServer(QVariantMap qvmLocalServer) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmLocalServer.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmLocalServer.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqLocalServer(qsdConnection);
        // Prepare the SQL statement
        qsqLocalServer.prepare(sQuery);
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
                qsqLocalServer.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqLocalServer;
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
        this->mObject->setProperty("sUsername",                    QString());
        this->mObject->setProperty("sPassword",                    QString());
        this->mObject->setProperty("sName",                        QString());
        this->mObject->setProperty("sStorageDirectory",            QString());
        this->mObject->setProperty("sStorageNode",                 QString());
        this->mObject->setProperty("sMailDirectory",               QString());
        this->mObject->setProperty("iQuota",                       int());
        this->mObject->setProperty("iBytes",                       int());
        this->mObject->setProperty("iMessages",                    int());
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
        this->mObject->setProperty("sLastLogin",                   QString());
        this->mObject->setProperty("sLastLoginId",                 QString());
        this->mObject->setProperty("sLastLoginProtocol",           QString());
        this->mObject->setProperty("sDisclaimer",                  QString());
        this->mObject->setProperty("sPasswordModified",            QString());
        this->mObject->setProperty("sCreated",                     QString());
        this->mObject->setProperty("sModified",                    QString());
        this->mObject->setProperty("sExpires",                     QString());
        this->mObject->setProperty("bActive",                      bool());
        this->mObject->setProperty("sLocalPartition",              QString());
        this->mObject->setProperty("iMailBoxId",                   int());
        this->mObject->setProperty("iAccountId",                   int());
        this->mObject->setProperty("iDomainId",                    int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpMailBox()
     * @param QString sJson
     * @return SpMailBox
     */
    SpMailBox(QString sJson) {
        // Decode and return the structure
        SpMailBox(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord
     * @brief SpMailBox()
     * @param QSqlRecord qsr
     * @return SpMailBox
     */
    SpMailBox(QSqlRecord qsrMailBox) {
        // Set the properties
        this->mObject->setProperty("sUsername",                    qsrMailBox.value("sUsername").toString());
        this->mObject->setProperty("sPassword",                    qsrMailBox.value("sPassword").toString());
        this->mObject->setProperty("sName",                        qsrMailBox.value("sName").toString());
        this->mObject->setProperty("sStorageDirectory",            qsrMailBox.value("sStorageDirectory").toString());
        this->mObject->setProperty("sStorageNode",                 qsrMailBox.value("sStorageNode").toString());
        this->mObject->setProperty("sMailDirectory",               qsrMailBox.value("sMailDirectory").toString());
        this->mObject->setProperty("iQuota",                       qsrMailBox.value("iQuota").toInt());
        this->mObject->setProperty("iBytes",                       qsrMailBox.value("iBytes").toInt());
        this->mObject->setProperty("iMessages",                    qsrMailBox.value("iMessages").toInt());
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
        this->mObject->setProperty("sLastLogin",                   qsrMailBox.value("sLastLogin").toString());
        this->mObject->setProperty("sLastLoginIp",                 qsrMailBox.value("sLastLoginIp").toString());
        this->mObject->setProperty("sLastLoginProtocol",           qsrMailBox.value("sLastLoginProtocol").toString());
        this->mObject->setProperty("sDisclaimer",                  qsrMailBox.value("sDisclaimer").toString());
        this->mObject->setProperty("sPasswordModified",            qsrMailBox.value("sPasswordModified").toString());
        this->mObject->setProperty("sCreated",                     qsrMailBox.value("sCreated").toString());
        this->mObject->setProperty("sModified",                    qsrMailBox.value("sModified").toString());
        this->mObject->setProperty("sExpires",                     qsrMailBox.value("sExpired").toString());
        this->mObject->setProperty("bActive",                      qsrMailBox.value("bActive").toBool());
        this->mObject->setProperty("sLocalPartition",              qsrMailBox.value("sLocalPartition").toString());
        this->mObject->setProperty("iMailBoxId",                   qsrMailBox.value("iMailBoxId").toInt());
        this->mObject->setProperty("iAccountId",                   qsrMailBox.value("iAccountId").toInt());
        this->mObject->setProperty("iDomainId",                    qsrMailBox.value("iDomainId").toInt());
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        this->mObject->setProperty("sCreated",               QString());
        this->mObject->setProperty("sModified",              QString());
        this->mObject->setProperty("bActive",                bool());
        this->mObject->setProperty("iAccountId",             int());
        this->mObject->setProperty("iDomainId",              int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpMailDomain()
     * @param QString sJson
     * @return SpMailDomain
     */
    SpMailDomain(QString sJson) {
        // Decode and return the structure
        SpMailDomain(QtJson::Json::parse(sJson).toMap());
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
        this->mObject->setProperty("sCreated",               qsrMailDomain.value("sCreated").toString());
        this->mObject->setProperty("sModified",              qsrMailDomain.value("sModified").toString());
        this->mObject->setProperty("bActive",                qsrMailDomain.value("bActive").toBool());
        this->mObject->setProperty("iAccountId",             qsrMailDomain.value("iAccountId").toInt());
        this->mObject->setProperty("iDomainId",              qsrMailDomain.value("iDomainId").toInt());
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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

/**
 * @paragraph This structure converts a Recipient BCC Domain into an object compatible with ServerPanel
 * @brief The SpRecipientBccDomain struct
 */
struct SpRecipientBccDomain {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpRecipientBccDomain()
     * @return SpRecipientBccDomain
     */
    SpRecipientBccDomain() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sDomain",               QString());
        this->mObject->setProperty("sBccAddress",           QString());
        this->mObject->setProperty("sCreated",              QString());
        this->mObject->setProperty("sModified",             QString());
        this->mObject->setProperty("sExpires",              QString());
        this->mObject->setProperty("bActive",               bool());
        this->mObject->setProperty("iRecipientBccDomainId", int());
        this->mObject->setProperty("iDomainId",             int());
        this->mObject->setProperty("iAccountId",            int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpRecipientBccDomain()
     * @param QString sJson
     * @return SpRecipientBccDomain
     */
    SpRecipientBccDomain(QString sJson) {
        // Decode and return the structure
        SpRecipientBccDomain(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpRecipientBccDomain()
     * @param QSqlRecord qsrRecipientBccDomain
     * @return SpRecipientBccDomain
     */
    SpRecipientBccDomain(QSqlRecord qsrRecipientBccDomain) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sDomain",               qsrRecipientBccDomain.value("sDomain").toString());
        this->mObject->setProperty("sBccAddress",           qsrRecipientBccDomain.value("sBccAddress").toString());
        this->mObject->setProperty("sCreated",              qsrRecipientBccDomain.value("sCreated").toString());
        this->mObject->setProperty("sModified",             qsrRecipientBccDomain.value("sModified").toString());
        this->mObject->setProperty("sExpires",              qsrRecipientBccDomain.value("sExpires").toString());
        this->mObject->setProperty("bActive",               qsrRecipientBccDomain.value("bActive").toBool());
        this->mObject->setProperty("iRecipientBccDomainId", qsrRecipientBccDomain.value("iRecipientBccDomainId").toInt());
        this->mObject->setProperty("iDomainId",             qsrRecipientBccDomain.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",            qsrRecipientBccDomain.value("iAccountId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpRecipientBccDomain()
     * @param QVariantMap qvmRecipientBccDomain
     * @return SpRecipientBccDomain
     */
    SpRecipientBccDomain(QVariantMap qvmRecipientBccDomain) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmRecipientBccDomain.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmRecipientBccDomain.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqRecipientBccDomain(qsdConnection);
        // Prepare the SQL statement
        qsqRecipientBccDomain.prepare(sQuery);
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
                qsqRecipientBccDomain.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqRecipientBccDomain;
    }
};

/**
 * @paragraph This structure converts a Recipient BCC User into an object compatible with ServerPanel
 * @brief The SpRecipientBccUser struct
 */
struct SpRecipientBccUser {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpRecipientBccUser()
     * @return SpRecipientBccUser
     */
    SpRecipientBccUser() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sUsername",           QString());
        this->mObject->setProperty("sBccAddress",         QString());
        this->mObject->setProperty("sDomain",             QString());
        this->mObject->setProperty("sCreated",            QString());
        this->mObject->setProperty("sModified",           QString());
        this->mObject->setProperty("sExpires",            QString());
        this->mObject->setProperty("bActive",             bool());
        this->mObject->setProperty("iRecipientBccUserId", int());
        this->mObject->setProperty("iDomainId",           int());
        this->mObject->setProperty("iAccountId",          int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpRecipientBccUser()
     * @param QString sJson
     * @return SpRecipientBccUser
     */
    SpRecipientBccUser(QString sJson) {
        // Decode and return the structure
        SpRecipientBccUser(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpRecipientBccUser()
     * @param QSqlRecord qsrRecipientBccUser
     * @return SpRecipientBccUser
     */
    SpRecipientBccUser(QSqlRecord qsrRecipientBccUser) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sUsername",           qsrRecipientBccUser.value("sUsername").toString());
        this->mObject->setProperty("sBccAddress",         qsrRecipientBccUser.value("sBccAddress").toString());
        this->mObject->setProperty("sDomain",             qsrRecipientBccUser.value("sDomain").toString());
        this->mObject->setProperty("sCreated",            qsrRecipientBccUser.value("sCreated").toString());
        this->mObject->setProperty("sModified",           qsrRecipientBccUser.value("sModified").toString());
        this->mObject->setProperty("sExpires",            qsrRecipientBccUser.value("sExpires").toString());
        this->mObject->setProperty("bActive",             qsrRecipientBccUser.value("bActive").toBool());
        this->mObject->setProperty("iRecipientBccUserId", qsrRecipientBccUser.value("iRecipientBccUserId").toInt());
        this->mObject->setProperty("iDomainId",           qsrRecipientBccUser.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",          qsrRecipientBccUser.value("iAccountId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpRecipientBccUser()
     * @param QVariantMap qvmRecipientBccUser
     * @return SpRecipientBccUser
     */
    SpRecipientBccUser(QVariantMap qvmRecipientBccUser) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmRecipientBccUser.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmRecipientBccUser.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqRecipientBccUser(qsdConnection);
        // Prepare the SQL statement
        qsqRecipientBccUser.prepare(sQuery);
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
                qsqRecipientBccUser.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqRecipientBccUser;
    }
};

/**
 * @paragraph This structure converts a Recipient BCC Domain into an object compatible with ServerPanel
 * @brief The SpRecipientBccDomain struct
 */
struct SpSenderBccDomain {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpSenderBccDomain()
     * @return SpSenderBccDomain
     */
    SpSenderBccDomain() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sDomain",            QString());
        this->mObject->setProperty("sBccAddress",        QString());
        this->mObject->setProperty("sCreated",           QString());
        this->mObject->setProperty("sModified",          QString());
        this->mObject->setProperty("sExpires",           QString());
        this->mObject->setProperty("bActive",            bool());
        this->mObject->setProperty("iSenderBccDomainId", int());
        this->mObject->setProperty("iDomainId",          int());
        this->mObject->setProperty("iAccountId",         int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpSenderBccDomain()
     * @param QString sJson
     * @return SpSenderBccDomain
     */
    SpSenderBccDomain(QString sJson) {
        // Decode and return the structure
        SpSenderBccDomain(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpSenderBccDomain()
     * @param QSqlRecord qsrSenderBccDomain
     * @return SpSenderBccDomain
     */
    SpSenderBccDomain(QSqlRecord qsrSenderBccDomain) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sDomain",            qsrSenderBccDomain.value("sDomain").toString());
        this->mObject->setProperty("sBccAddress",        qsrSenderBccDomain.value("sBccAddress").toString());
        this->mObject->setProperty("sCreated",           qsrSenderBccDomain.value("sCreated").toString());
        this->mObject->setProperty("sModified",          qsrSenderBccDomain.value("sModified").toString());
        this->mObject->setProperty("sExpires",           qsrSenderBccDomain.value("sExpires").toString());
        this->mObject->setProperty("bActive",            qsrSenderBccDomain.value("bActive").toBool());
        this->mObject->setProperty("iSenderBccDomainId", qsrSenderBccDomain.value("iSenderBccDomainId").toInt());
        this->mObject->setProperty("iDomainId",          qsrSenderBccDomain.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",         qsrSenderBccDomain.value("iAccountId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpRecipientBccUser()
     * @param QVariantMap qvmRecipientBccDomain
     * @return SpRecipientBccDomain
     */
    SpSenderBccDomain(QVariantMap qvmSenderBccDomain) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmSenderBccDomain.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmSenderBccDomain.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqSenderBccDomain(qsdConnection);
        // Prepare the SQL statement
        qsqSenderBccDomain.prepare(sQuery);
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
                qsqSenderBccDomain.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqSenderBccDomain;
    }
};

/**
 * @paragraph This structure converts a Recipient BCC User into an object compatible with ServerPanel
 * @brief The SpRecipientBccUser struct
 */
struct SpSenderBccUser {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpSenderBccUser()
     * @return SpSenderBccUser
     */
    SpSenderBccUser() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sUsername",        QString());
        this->mObject->setProperty("sBccAddress",      QString());
        this->mObject->setProperty("sDomain",          QString());
        this->mObject->setProperty("sCreated",         QString());
        this->mObject->setProperty("sModified",        QString());
        this->mObject->setProperty("sExpires",         QString());
        this->mObject->setProperty("bActive",          bool());
        this->mObject->setProperty("iSenderBccUserId", int());
        this->mObject->setProperty("iDomainId",        int());
        this->mObject->setProperty("iAccountId",       int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpSenderBccUser()
     * @param QString sJson
     * @return SpSenderBccUser
     */
    SpSenderBccUser(QString sJson) {
        // Decode and return the structure
        SpSenderBccUser(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpSenderBccUser()
     * @param QSqlRecord qsrSenderBccUser
     * @return SpSenderBccUser
     */
    SpSenderBccUser(QSqlRecord qsrSenderBccUser) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sUsername",        qsrSenderBccUser.value("sUsername").toString());
        this->mObject->setProperty("sBccAddress",      qsrSenderBccUser.value("sBccAddress").toString());
        this->mObject->setProperty("sDomain",          qsrSenderBccUser.value("sDomain").toString());
        this->mObject->setProperty("sCreated",         qsrSenderBccUser.value("sCreated").toString());
        this->mObject->setProperty("sModified",        qsrSenderBccUser.value("sModified").toString());
        this->mObject->setProperty("sExpires",         qsrSenderBccUser.value("sExpires").toString());
        this->mObject->setProperty("bActive",          qsrSenderBccUser.value("bActive").toBool());
        this->mObject->setProperty("iSenderBccUserId", qsrSenderBccUser.value("iSenderBccUserId").toInt());
        this->mObject->setProperty("iDomainId",        qsrSenderBccUser.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",       qsrSenderBccUser.value("iAccountId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpRecipientBccUser()
     * @param QVariantMap qvmRecipientBccUser
     * @return SpRecipientBccUser
     */
    SpSenderBccUser(QVariantMap qvmSenderBccUser) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmSenderBccUser.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmSenderBccUser.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqSenderBccUser(qsdConnection);
        // Prepare the SQL statement
        qsqSenderBccUser.prepare(sQuery);
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
                qsqSenderBccUser.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqSenderBccUser;
    }
};

/**
 * @paragraph This structure converts a Shared Folder into an object compatible with ServerPanel
 * @brief The SpSharedFolder struct
 */
struct SpSharedFolder {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpSharedFolder()
     * @return SpSharedFolder
     */
    SpSharedFolder() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sSender",         QString());
        this->mObject->setProperty("sRecipient",      QString());
        this->mObject->setProperty("sDummy",          QString());
        this->mObject->setProperty("iSharedFolderId", int());
        this->mObject->setProperty("iAccountId",      int());
        this->mObject->setProperty("bActive",         bool());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpSharedFolder()
     * @param QString sJson
     * @return SpSharedFolder
     */
    SpSharedFolder(QString sJson) {
        // Decode and return the structure
        SpSharedFolder(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpSharedFolder()
     * @param QSqlRecord qsrSharedFolder
     * @return SpSharedFolder
     */
    SpSharedFolder(QSqlRecord qsrSharedFolder) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sSender",         qsrSharedFolder.value("sSender").toString());
        this->mObject->setProperty("sRecipient",      qsrSharedFolder.value("sRecipient").toString());
        this->mObject->setProperty("sDummy",          qsrSharedFolder.value("sDummy").toString());
        this->mObject->setProperty("iSharedFolderId", qsrSharedFolder.value("iSharedFolderId").toInt());
        this->mObject->setProperty("iAccountId",      qsrSharedFolder.value("iAccountId").toInt());
        this->mObject->setProperty("bActive",         qsrSharedFolder.value("bActive").toBool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpSharedFolder()
     * @param QVariantMap qvmSharedFolder
     * @return SpSharedFolder
     */
    SpSharedFolder(QVariantMap qvmSharedFolder) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmSharedFolder.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmSharedFolder.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqSharedFolder(qsdConnection);
        // Prepare the SQL statement
        qsqSharedFolder.prepare(sQuery);
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
                qsqSharedFolder.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqSharedFolder;
    }
};

/**
 * @paragraph This structure converts a Vacation Message into an object compatible with ServerPanel
 * @brief The SpVacationMessage struct
 */
struct SpVacationMessage {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpVacationMessage()
     * @return SpVacationMessage
     */
    SpVacationMessage() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sEmailAddress",      QString());
        this->mObject->setProperty("sSubject",           QString());
        this->mObject->setProperty("sBody",              QString());
        this->mObject->setProperty("sCache",             QString());
        this->mObject->setProperty("sDomain",            QString());
        this->mObject->setProperty("sCreated",           QString());
        this->mObject->setProperty("sModified",          QString());
        this->mObject->setProperty("bActive",            bool());
        this->mObject->setProperty("iVacationMessageId", int());
        this->mObject->setProperty("iMailBoxId",         int());
        this->mObject->setProperty("iDomainId",          int());
        this->mObject->setProperty("iAccountId",         int());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpVacationMessage()
     * @param QString sJson
     * @return SpVacationMessage
     */
    SpVacationMessage(QString sJson) {
        // Decode and return the structure
        SpVacationMessage(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpVacationMessage()
     * @param QSqlRecord qsrVacationMessage
     * @return SpVacationMessage
     */
    SpVacationMessage(QSqlRecord qsrVacationMessage) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sEmailAddress",      qsrVacationMessage.value("sEmailAddress").toString());
        this->mObject->setProperty("sSubject",           qsrVacationMessage.value("sSubject").toString());
        this->mObject->setProperty("sBody",              qsrVacationMessage.value("sBody").toString());
        this->mObject->setProperty("sCache",             qsrVacationMessage.value("sCache").toString());
        this->mObject->setProperty("sDomain",            qsrVacationMessage.value("sDomain").toString());
        this->mObject->setProperty("sCreated",           qsrVacationMessage.value("sCreated").toString());
        this->mObject->setProperty("sModified",          qsrVacationMessage.value("sModified").toString());
        this->mObject->setProperty("bActive",            qsrVacationMessage.value("bActive").toBool());
        this->mObject->setProperty("iVacationMessageId", qsrVacationMessage.value("iVacationMessageId").toInt());
        this->mObject->setProperty("iMailBoxId",         qsrVacationMessage.value("iMailBox").toInt());
        this->mObject->setProperty("iDomainId",          qsrVacationMessage.value("iDomainId").toInt());
        this->mObject->setProperty("iAccountId",         qsrVacationMessage.value("iAccountId").toInt());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpVacationMessage()
     * @param QVariantMap qvmVacationMessage
     * @return SpVacationMessage
     */
    SpVacationMessage(QVariantMap qvmVacationMessage) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmVacationMessage.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmVacationMessage.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqVacationMessage(qsdConnection);
        // Prepare the SQL statement
        qsqVacationMessage.prepare(sQuery);
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
                qsqVacationMessage.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqVacationMessage;
    }
};

/**
 * @paragraph This structure converts a Vacation Message into an object compatible with ServerPanel
 * @brief The SpVacationMessage struct
 */
struct SpVacationNotification {
    QObject* mObject;
    /**
     * @paragraph This is the base constructor
     * @brief SpVacationNotification()
     * @return SpVacationNotification
     */
    SpVacationNotification() : mObject(new QObject()) {
        // Initialize the properties
        this->mObject->setProperty("sOnVacation",             QString());
        this->mObject->setProperty("sNotified",               QString());
        this->mObject->setProperty("sNotifiedTimestamp",      QString());
        this->mObject->setProperty("iVacationNotificationId", int());
        this->mObject->setProperty("iMailBoxId",              int());
        this->mObject->setProperty("iAccountId",              int());
        this->mObject->setProperty("bEnabled",                bool());
    }
    /**
     * @paragraph This constructor creates a structure from a JSON string
     * @brief SpVacationNotification()
     * @param QString sJson
     * @return SpVacationNotification
     */
    SpVacationNotification(QString sJson) {
        // Decode and return the structure
        SpVacationNotification(QtJson::Json::parse(sJson).toMap());
    }
    /**
     * @paragraph This constructor creates a structure out of a QSqlRecord object
     * @brief SpVacationNotification()
     * @param QSqlRecord qsrVacationNotification
     * @return SpVacationNotification
     */
    SpVacationNotification(QSqlRecord qsrVacationNotification) : mObject(new QObject()) {
        // Set the properties
        this->mObject->setProperty("sOnVacation",             qsrVacationNotification.value("sOnVacation").toString());
        this->mObject->setProperty("sNotified",               qsrVacationNotification.value("sNotified").toString());
        this->mObject->setProperty("sNotifiedTimestamp",      qsrVacationNotification.value("sNotifiedTimestamp").toString());
        this->mObject->setProperty("iVacationNotificationId", qsrVacationNotification.value("iVacationNotificationId").toInt());
        this->mObject->setProperty("iMailBoxId",              qsrVacationNotification.value("iMailBoxId").toInt());
        this->mObject->setProperty("iAccountId",              qsrVacationNotification.value("iAccountId").toInt());
        this->mObject->setProperty("bEnabled",                qsrVacationNotification.value("bEnabled").toBool());
    }
    /**
     * @paragraph This constructor creates a structure out of a QVariantMap
     * @brief SpVacationNotification()
     * @param QVariantMap qvmVacationNotification
     * @return SpVacationNotification
     */
    SpVacationNotification(QVariantMap qvmVacationNotification) : mObject(new QObject()) {
        // Create an iterator
        QVariantMap::ConstIterator itrProperty     = qvmVacationNotification.constBegin();
        QVariantMap::ConstIterator itrLastProperty = qvmVacationNotification.constEnd();
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
     * @paragraph This routine converts this structure into a JSON string
     * @brief toJson
     * @return QByteArray
     */
    QByteArray toJson() {
        // Convert and return this structure as JSON
        return QtJson::Json::serialize(this->toMap());
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
        QSqlQuery qsqVacationNotification(qsdConnection);
        // Prepare the SQL statement
        qsqVacationNotification.prepare(sQuery);
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
                qsqVacationNotification.bindValue(QString(itrProperty.key()).prepend(":"), itrProperty.value());
            }
        }
        // Return the query object
        return qsqVacationNotification;
    }
};

#endif // SERVERPANELSTRUCTURES_H
