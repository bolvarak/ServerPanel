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
    bool bEnabled;
    // Initializer
    SpAccount() : iAccountId(0), sStreetAddress(), sStreetAddressExtra(), sCity(), sState(), sPostalCode(), sCountry("USA"), iAccountLevel(3), bEnabled(true) {}
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
    SpDnsRecord() : iRecordId(0), sDirection("IN"), sType("A"), iPriority(), bEnabled(true) {}
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
    SpDomain() : iDomainId(0), bEnabled(true) {}
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
    SpMailBox() : iMailBoxId(0), sName(), sStorageDirectory("/var/vmail"), sStorageNode("serverpanel"), sMailDirectory(), iQuota(1024), iBytes(0), sTransport(), sDepartment(), sRank("normal"), sEmployeeId(), bEnableSmtp(true), bEnableSecureSmtp(true), bEnablePop3(true), bEnableSecurePop3(true), bEnableImap(true), bEnableSecureImap(true), bEnableDelivery(true), bEnableSieveManagement(true), bEnableSecureSieveManagement(true), bEnableInternalDelivery(true), sDisclaimer(), bActive(true), sLocalPartition() {}
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
    SpMailDomain() : iMailDomainId(0), sDescription(), sDisclaimer(), iMaxQuota(1024), iQuota(1024), sTransport("dovecot"), bBackupMx(false), iDefaultUserQuota(1024), sDefaultPasswordScheme("md5"), iMinimumPasswordLength(5), iMaximumPasswordLength(75), bActive(true) {}
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
