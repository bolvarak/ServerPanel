///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include "ServerPanel.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel* ServerPanel::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel* ServerPanel::Instance() {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanel();
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel::ServerPanel(QObject* cParent) : QObject(cParent) {
    // Load the configuration
    this->mConfig = new QSettings("/home/tbrown/Documents/ServerPanel/ServerPanelService/ServerPanel.ini", QSettings::IniFormat);
    // Setup the databae
    this->mDbc    = QSqlDatabase::addDatabase(this->mConfig->value("databaseSettings/driver").toString());
    // Set the host
    this->mDbc.setHostName(this->mConfig->value("databaseSettings/host").toString());
    // Set the port
    this->mDbc.setPort(this->mConfig->value("databaseSettings/port").toInt());
    // Set the database
    this->mDbc.setDatabaseName(this->mConfig->value("databaseSettings/db").toString());
    // Set the username
    this->mDbc.setUserName(this->mConfig->value("databaseSettings/user").toString());
    // Set the password
    this->mDbc.setPassword(this->mConfig->value("databaseSettings/pass").toString());
    // We're done with the database configuration
    // Try to open the database
    if (!this->mDbc.open()) {
        // Setup the error placeholder
        QByteArray qbaError;
        // Add the error
        qbaError.append(this->mDbc.lastError().text());
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel::~ServerPanel() {
    // Close the database
    this->mDbc.close();
    // Close the configuration file
    delete this->mConfig;
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Methods ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method adds a new account to the system
 * @brief ServerPanel::AddAccount
 * @param QString sUsername
 * @param QString sPassword
 * @param QString sEmailAddress
 * @param QString sFirstName
 * @param QString sLastName
 * @param QString sPhoneNumber
 * @param QString sHomeDirectory
 * @param QString sStreetAddress
 * @param QString sStreetAddressExtra
 * @param QString sCity
 * @param QString sState
 * @param QString sPostalCode
 * @param QString sCountry
 * @param int iAccountLevel
 * @param QBool bEnabled
 * @return QVariantMap
 */
QVariantMap ServerPanel::AddAccount(QString sUsername, QString sPassword, QString sEmailAddress, QString sFirstName, QString sLastName, QString sPhoneNumber, QString sHomeDirectory, QString sStreetAddress, QString sStreetAddressExtra, QString sCity, QString sState, QString sPostalCode, QString sCountry, int iAccountLevel, QBool bEnabled) {
    // Set the return map
    QVariantMap qvmReturn;
    // First we need to check the username
    QSqlQuery qsqUsername(this->mDbc);
    // Prepare the username query
    qsqUsername.prepare(this->mConfig->value("sqlQueries/checkForExistingUsername").toString());
    // Add the username to the query
    qsqUsername.bindValue(0, sUsername);
    // Try to execute the query
    if (!qsqUsername.exec()) {
        // Set the error text
        qvmReturn.insert("sError",   qsqUsername.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for rows
    if (qsqUsername.size()) {
        // We have a username, let's notify the caller
        qvmReturn.insert("sError",   "The username you chose already exists in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // We're done with the username query, so close it
    qsqUsername.finish();
    // Now we will setup a new query to create the account
    QSqlQuery qsqAccount(this->mDbc);
    // Prepare the statement
    qsqAccount.prepare(this->mConfig->value("sqlQueries/addAccount"));
    // Add the values
    qsqAccount.bindValue(0,  sUsername);           // Username
    qsqAccount.bindValue(1,  sPassword);           // Password
    qsqAccount.bindValue(2,  sEmailAddress);       // Email Address
    qsqAccount.bindValue(3,  sFirstName);          // First Name
    qsqAccount.bindValue(4,  sLastName);           // Last Name
    qsqAccount.bindValue(5,  sPhoneNumber);        // Phone Number
    qsqAccount.bindValue(6,  sHomeDirectory);      // User Home Directory
    qsqAccount.bindValue(7,  sStreetAddress);      // Street Address
    qsqAccount.bindValue(8,  sStreetAddressExtra); // Street Address 2
    qsqAccount.bindValue(9,  sCity);               // City
    qsqAccount.bindValue(10, sState);              // State
    qsqAccount.bindValue(11, sPostalCode);         // Zip Code
    qsqAccount.bindValue(12, sCountry);            // Country
    qsqAccount.bindValue(13, iAccountLevel);       // Account Level
    qsqAccount.bindValue(14, bEnabled);            // Enable Account
    // Try to execute the query
    if (!qsqAccount.exec()) {
        // Set the error string
        qvmReturn.insert("sError",   qsqAccount.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Create the argument string to add the system user
    QStringList qslArguments;
    // Add the arguments
    qslArguments.append(QString("-d /home/").append(sUsername)); // Home Directory
    qslArguments.append("-m");                                   // Create Home Directory
    qslArguments.append(QString("-p ").append(sPassword));       // Password
    qslArguments.append(QString("-s /bin/bash"));                // Shell
    qslArguments.append("-U");                                   // Create User's Group
    qslArguments.append(sUsername);                              // Login
    // Try to add the user and group
    if (!ExecuteSystemCmd("useradd", qslArguments)) {
        // Set the error string
        qvmReturn.insert("sError",   "Unable to add the system account for this user.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // The account has been created, set the message
    qvmReturn.insert("sMessage", "System account has been successfully added.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method adds a DNS record for a domain into the system
 * @brief ServerPanel::AddDnsRecord
 * @param int iAccountId
 * @param int iDomainId
 * @param QString sHostName
 * @param QString sAddress
 * @param QString sDirection
 * @param QString sType
 * @param int iPriority
 * @param QBool bEnabled
 * @return QVariantMap
 */
QVariantMap ServerPanel::AddDnsRecord(int iAccountId, int iDomainId, QString sHostName, QString sAddress, QString sDirection, QString sType, int iPriority, QBool bEnabled) {
    // Set the return map
    QVariantMap qvmReturn;
    // Setup the SQL query to check for the DNS record
    QSqlQuery qsqDnsRecord(this->mDbc);
    // Prepare the SQL
    qsqDnsRecord.prepare(this->mConfig->value("sqlQueries/checkForExistingDnsRecord"));
    // Add the values'
    qsqDnsRecord.bindValue(0, iDomainId); // Domain Identifer
    qsqDnsRecord.bindValue(1, sHostName); // Host name
    // Try to execute the query
    if (!qsqDnsRecord.exec()) {
        // Set the error message
        qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqDnsRecord.size()) {
        // Set the error
        qvmReturn.insert("sError",   "The DNS record you are trying to add already exists in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // We're done with the check query, close it
    qsqDnsRecord.finish();
    // Now we'll create a new query to insert the new DNS record
    QSqlQuery qsqNewDnsRecord(this->mDbc);
    // Prepare the sql
    qsqNewDnsRecord.prepare(this->mConfig->value("sqlQueries/addDnsRecord"));
    // Bind the values
    qsqNewDnsRecord.bindValue(0, iAccountId); // Account ID of associated account
    qsqNewDnsRecord.bindValue(1, iDomainId);  // Domain ID of associated domain
    qsqNewDnsRecord.bindValue(2, sHostName);  // New host name
    qsqNewDnsRecord.bindValue(3, sAddress);   // IP Address
    qsqNewDnsRecord.bindValue(4, sDirection); // Direction of the record (IN/OUT)
    qsqNewDnsRecord.bindValue(5, sType);      // Type of the record (A, AAAA, NS, MX, etc)
    qsqNewDnsRecord.bindValue(6, iPriority);  // MX priority
    qsqNewDnsRecord.bindValue(7, bEnabled);   // Active status
    // Try to execute the query
    if (!qsqNewDnsRecord.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqNewDnsRecord.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "New DNS record added successfully.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method adds a DNS zone into the system
 * @brief ServerPanel::AddDomain
 * @param int iAccountId
 * @param QString sDomain
 * @param QBool bEnabled
 * @return QVariantMap
 */
QVariantMap ServerPanel::AddDomain(int iAccountId, QString sDomain, QBool bEnabled) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Setup a query to check for the domain
    QSqlQuery qsqDomain(this->mDbc);
    // Prepare the SQL statement
    qsqDomain.prepare(this->mConfig->value("sqlQueries/checkForExistingDomain"));
    // Bind the values
    qsqDomain.bindValue(0, sDomain); // Domain Name
    // Try to execute the query
    if (!qsqDomain.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqDomain.size()) {
        // Set the error message
        qvmReturn.insert("sError",   "The domain already exists in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // We're done with that query, close it
    qsqDomain.finish();
    // Create a new query to add the new domain
    QSqlQuery qsqNewDomain(this->mDbc);
    // Prepare the SQL statement
    qsqNewDomain.prepare(this->mConfig->value("sqlQueries/addNewDomain"));
    // Bind the values
    qsqNewDomain.bindValue(0, iAccountId); // Associated account identifier
    qsqNewDomain.bindValue(1, sDomain);    // Domain name
    qsqNewDomain.bindValue(2, bEnabled);   // Active status
    // Try to execute the query
    if (!qsqNewDomain.exec()) {
        // Set  the error message
        qvmReturn.insert("sError",   qsqNewDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "New domain added successfully.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method adds a new mailbox into the system
 * @brief ServerPanel::AddMailBox
 * @param QString sUsername
 * @param QString sPassword
 * @param QString sName
 * @param QString sStorageDirectory
 * @param QString sStorageNode
 * @param QString sMailDirectory
 * @param int iQuota
 * @param int iBytes
 * @param QString sDomain
 * @param QString sTransport
 * @param QString sDepartment
 * @param QString sRank
 * @param QString sEmployeeId
 * @param QBool bEnableSmtp
 * @param QBool bEnableSecureSmtp
 * @param QBool bEnablePop3
 * @param QBool bEnableSecurePop3
 * @param QBool bEnableImap
 * @param QBool bEnableSecureImap
 * @param QBool bEnableDelivery
 * @param QBool bEnableSieveManagement
 * @param QBool bEnableSecureSieveManagement
 * @param QBool bEnableInternalDelivery
 * @param QString sDisclaimer
 * @param QBool bActive
 * @param QString sLocalPartition
 * @return QVariantMap
 */
QVariantMap ServerPanel::AddMailBox(QString sUsername, QString sPassword, QString sName, QString sStorageDirectory, QString sStorageNode, QString sMailDirectory, int iQuota, int iBytes, QString sDomain, QString sTransport, QString sDepartment, QString sRank, QString sEmployeeId, QBool bEnableSmtp, QBool bEnableSecureSmtp, QBool bEnablePop3, QBool bEnableSecurePop3, QBool bEnableImap, QBool bEnableSecureImap, QBool bEnableDelivery, QBool bEnableSieveManagement, QBool bEnableSecureSieveManagement, QBool bEnableInternalDelivery, QString sDisclaimer, QBool bActive, QString sLocalPartition) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Setup a query to check for the mailbox
    QSqlQuery qsqMailBox(this->mDbc);
    // Prepare the SQL statement
    qsqMailBox.prepare(this->mConfig->value("sqlQueries/checkForExistingMailBox"));
    // Bind the values
    qsqMailBox.bindValue(0, sDomain);   // Associated mail domain
    qsqMailBox.bindValue(1, sUsername); // Email Address
    // Try to execute the query
    if (!qsqMailBox.exec()) {
        // Set the error message
        qvmReturn.insert("sError",   qsqMailBox.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqMailBox.size()) {
        // Set the error message
        qvmReturn.insert("sError",   "A mailbox with that username and domain already exists in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // We're done with that query, so close it
    qsqMailBox.finish();
    // Create a new query to add the new mailbox
    QSqlQuery qsqNewMailBox(this->mDbc);
    // Prepare the SQL statement
    qsqNewMailBox.prepare(this->mConfig->value("sqlQueries/addNewMailBox"));
    // Bind the values
    qsqNewMailBox.bindValue(0,  sUsername);                    // Email address
    qsqNewMailBox.bindValue(1,  sPassword);                    // Mailbox password
    qsqNewMailBox.bindValue(2,  sName);                        // User's name
    qsqNewMailBox.bindValue(3,  sStorageDirectory);            // Storage node parent directory
    qsqNewMailBox.bindValue(4,  sStorageNode);                 // Storage subdirectory
    qsqNewMailBox.bindValue(5,  sMailDirectory);               // Maildir
    qsqNewMailBox.bindValue(6,  iQuota);                       // Mailbox quota
    qsqNewMailBox.bindValue(7,  iBytes);                       // Bytes of quota used
    qsqNewMailBox.bindValue(8,  sDomain);                      // Associated mail domain identifer
    qsqNewMailBox.bindValue(9,  sTransport);                   // Transprt application
    qsqNewMailBox.bindValue(10, sDepartment);                  // Department identifier
    qsqNewMailBox.bindValue(11, sRank);                        // Priority rank
    qsqNewMailBox.bindValue(12, sEmployeeId);                  // Employee identifier
    qsqNewMailBox.bindValue(13, bEnableSmtp);                  // Enable SMTP login and delivery
    qsqNewMailBox.bindValue(14, bEnableSecureSmtp);            // Enable secure SMTP login and delivery
    qsqNewMailBox.bindValue(15, bEnablePop3);                  // Enable POP3 login
    qsqNewMailBox.bindValue(16, bEnableSecurePop3);            // Enable secure POP3 login
    qsqNewMailBox.bindValue(17, bEnableImap);                  // Enable IMAP login
    qsqNewMailBox.bindValue(18, bEnableSecureImap);            // Enable secure IMAP login
    qsqNewMailBox.bindValue(19, bEnableDelivery);              // Enable mail delivery
    qsqNewMailBox.bindValue(20, bEnableSieveManagement);       // Allow Sieve management
    qsqNewMailBox.bindValue(21, bEnableSecureSieveManagement); // Allow secure Sieve management
    qsqNewMailBox.bindValue(22, bEnableInternalDelivery);      // Allow internal system delivery
    qsqNewMailBox.bindValue(23, sDisclaimer);                  // Disclaimer text
    qsqNewMailBox.bindValue(24, bActive);                      // Active status
    qsqNewMailBox.bindValue(25, sLocalPartition);              // Local storage partition
    // Try to execute the query
    if (!qsqNewMailBox.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqNewMailBox.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "New mailbox has been successfully added.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method enables mailboxes on a domain
 * @brief ServerPanel::AddMailDomain
 * @param int iAccountId
 * @param int iDomainId
 * @param QString sDescription
 * @param QString sDisclaimer
 * @param int iMaxQuota
 * @param int iQuota
 * @param QString sTransport
 * @param QBool bBackupMx
 * @param int iDefaultUserQuota
 * @param QString sDefaultPasswordScheme
 * @param int iMinimumPasswordLength
 * @param int iMaximumPasswordLength
 * @param QBool bActive
 * @return QVariantMap
 */
QVariantMap ServerPanel::AddMailDomain(int iAccountId, int iDomainId, QString sDescription, QString sDisclaimer, int iMaxQuota, int iQuota, QString sTransport, QBool bBackupMx, int iDefaultUserQuota, QString sDefaultPasswordScheme, int iMinimumPasswordLength, int iMaximumPasswordLength, QBool bActive) {
    // Set the return map
    QVariantMap qvmReturn;
    // Setup a SQL query to check for the mail domain
    QSqlQuery qsqMailDomain(this->mDbc);
    // Prepare the SQL statement
    qsqMailDomain.prepare(this->mConfig->value("sqlQueries/checkForExistingMailDomain"));
    // Bind the values
    qsqMailDomain.bindValue(0, iDomainId); // Associated domain identifier
    // Try to execute the query
    if (!qsqMailDomain.exec()) {
        // Set the error message
        qvmReturn.insert("sError",   qsqMailDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqMailDomain.size()) {
        // Set the error message
        qvmReturn.insert("sError",   "Mail accounts have already been enabled on this domain.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // We're done with the check query, close it
    qsqMailDomain.finish();
    // Create a new query to add the new mail domain
    QSqlQuery qsqNewMailDomain(this->mDbc);
    // Prepare the SQL statement
    qsqNewMailDomain.prepare(this->mConfig->value("sqlQueries/addNewMailDomain"));
    // Bind the values
    qsqNewMailDomain.bindValue(0,  iAccountId);             // Associated account identifier
    qsqNewMailDomain.bindValue(1,  iDomainId);              // Associated domain identifier
    qsqNewMailDomain.bindValue(2,  sDescription);           // Mail domain description
    qsqNewMailDomain.bindValue(3,  sDisclaimer);            // Disclaimer notice
    qsqNewMailDomain.bindValue(4,  iMaxQuota);              // Max domain quota
    qsqNewMailDomain.bindValue(5,  iQuota);                 // Domain quota
    qsqNewMailDomain.bindValue(6,  sTransport);             // Transport application
    qsqNewMailDomain.bindValue(7,  bBackupMx);              // Is this a backup mail domain
    qsqNewMailDomain.bindValue(8,  iDefaultUserQuota);      // Default mailbox quota
    qsqNewMailDomain.bindValue(9,  sDefaultPasswordScheme); // Default password scheme (MD5)
    qsqNewMailDomain.bindValue(10, iMinimumPasswordLength); // Min password length
    qsqNewMailDomain.bindValue(11, iMaximumPasswordLength); // Max password length
    qsqNewMailDomain.bindValue(12, bEnabled);               // Active status
    // Try to execute the query
    if (!qsqNewMailDomain.exec()) {
        // Set the error message
        qvmReturn.insert("sError",   qsqNewMailDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "Mail sending has been successfully activated on this domain.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method runs the query and logic for authenticating a user
 * @brief ServerPanel::AuthenticateUser
 * @param QString sUsername
 * @param QString sPassword
 * @return QVariantMap qvmReturn
 */
QVariantMap ServerPanel::AuthenticateUser(QString sUsername, QString sPassword) {
    // Set the return map
    QVariantMap qvmReturn;
    // Setup the query
    QSqlQuery qsqAccount(this->mDbc);
    // Prepare the query
    qsqAccount.prepare(this->mConfig->value("sqlQueries/accountAuthentication").toString());
    // Bind the values
    qsqAccount.bindValue(0, sUsername); // Username
    qsqAccount.bindValue(1, sPassword); // Password
    // Try to execute the query
    if (!qsqAccount.exec()) {
        // Setup the error
        qvmReturn.insert("sError",   qsqAccount.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqAccount.size()) {
        // Loop through the rows
        while (qsqAccount.next()) {
            // Grab the record
            QSqlRecord qsrAccount = qsqAccount.record();
            // Assign the data to the map
            qvmReturn.insert("iAccountId",          qsqAccount.value(qsrAccount.indexOf("iAccountId")).toInt());             // Account ID
            qvmReturn.insert("sUsername",           qsqAccount.value(qsrAccount.indexOf("sUsername")).toString());           // Username
            qvmReturn.insert("sEmailAddress",       qsqAccount.value(qsrAccount.indexOf("sEmailAddress")).toString());       // Email Address
            qvmReturn.insert("sFirstName",          qsqAccount.value(qsrAccount.indexOf("sFirstName")).toString());          // First Name
            qvmReturn.insert("sLastName",           qsqAccount.value(qsrAccount.indexOf("sLastName")).toString());           // Last Name
            qvmReturn.insert("sPhoneNumber",        qsqAccount.value(qsrAccount.indexOf("sPhoneNumber")).toString());        // Phone Number
            qvmReturn.insert("sStreetAddress",      qsqAccount.value(qsrAccount.indexOf("sStreetAddress")).toString());      // Street Address
            qvmReturn.insert("sStreetAddressExtra", qsqAccount.value(qsrAccount.indexOf("sStreetAddressExtra")).toString()); // Street Address Extra
            qvmReturn.insert("sCity",               qsqAccount.value(qsrAccount.indexOf("sCity")).toString());               // City
            qvmReturn.insert("sState",              qsqAccount.value(qsrAccount.indexOf("sState")).toString());              // State
            qvmReturn.insert("sPostalCode",         qsqAccount.value(qsrAccount.indexOf("sPostalCode")).toString());         // Zip Code
            qvmReturn.insert("bEnabled",            qsqAccount.value(qsrAccount.indexOf("bEnabled")).toBool());              // Account Enabled
            qvmReturn.insert("iAccountLevel",       qsqAccount.value(qsrAccount.indexOf("iAccountLevel")).toInt());          // Account Privilege Level
        }
    } else {
        // Clear the map
        qvmReturn.clear();
        // Set the error
        qvmReturn.insert("sError",   "I was unable to find your account, please check your username and password and try again.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
    }
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method decodes a JSON request made to the system
 * @brief ServerPanel::DecodeRequest
 * @param QString sRequest
 * @return QVarianMap qvmResuest
 */
QVariantMap ServerPanel::DecodeRequest(QString sRequest) {
    // Setup a boolean identifier
    bool bDecoded;
    // Decode the JSON
    QVariantMap qvmRequest = QtJson::Json::parse(sRequest, bDecoded).toMap();
    // Make sure the JSON was decoded properly
    if (!bDecoded) {
        // Reset the request
        qvmRequest.clear();
        // Set the error into the map
        qvmRequest.insert("sError",   "Your request was not able to be decoded, please check your request and ensure that it is valid.");
        // Set the success status
        qvmRequest.insert("bSuccess", false);
    }
    // Return the request map
    return qvmRequest;
}

/**
 * @paragraph This method encodes a QVariantMap into a JSON string to be sent across the network
 * @brief ServerPanel::EncodeResponse
 * @param QVariantMap qvmResponse
 * @return QByteArray
 */
QByteArray ServerPanel::EncodeResponse(QVariantMap qvmResponse) {
    // Encode the map and return it
    return QtJson::Json::serialize(qvmResponse);
}

QBool ServerPanel::ExecuteSystemCmd(QString sProgram, QStringList qslArguments) {
    // Try to execute the command
    if (QProcess::execute(sProgram, qslArguments) == QProcess::NormalExit) {
        // Everything's good, we're done
        return QBool(true);
    }
    // The execution failed
    return QBool(false);
}

/**
 * @paragraph This method is the work horse of the system.  It takes the request JSON, decodes it and determines what method in the system should be executed.
 * @brief ServerPanel::HandleRequest
 * @param QString sRequest
 * @return QByteArray sResponse
 */
QByteArray ServerPanel::HandleRequest(QString sRequest) {
    // Create a byte array of the request
    QByteArray qbaRequest;
    // Add the request
    qbaRequest.append(sRequest);
    // Log the request
    this->LogMessage(qbaRequest);
    // Decode the request
    QVariantMap qvmRequest = this->DecodeRequest(sRequest);
    // Make sure there is a method
    if (qvmRequest.contains(SERVERPANEL_METHOD_NOTATION_KEY)) {
        // Set the method
        QString sMethod = qvmRequest[SERVERPANEL_METHOD_NOTATION_KEY].toString();
        // Determine the method to execute
        if (sMethod.contains("AuthenticateUser", Qt::CaseInsensitive)) { // User authentication
            // Grab the response
            QByteArray qbaResponse = this->EncodeResponse(      // Call to EncodeResponse
                        this->AuthenticateUser(                 // Call to AuthenticateUser
                            qvmRequest["sUsername"].toString(), // Username
                            qvmRequest["sPassword"].toString()  // Password
                        )
                   );                                           // The response is sent
            // Log the response
            this->LogMessage(qbaResponse);
            // Send the response
            return qbaResponse;
        } else if (sMethod.contains("Ping", Qt::CaseInsensitive)) {      // Ping test
            // Create a map with the time stamps
            QVariantMap qvmPing;
            // Add the human readable time stamp
            qvmPing.insert("sTimestamp", QDateTime::currentDateTime());
            // Add the UTC timestamp
            qvmPing.insert("sUtcTimestamp", QDateTime::currentDateTimeUtc());
            // Add the seconds since epoch
            qvmPing.insert("sEpochTimestamp", QDateTime::currentMSecsSinceEpoch());
            // Grab the response
            QByteArray qbaResponse = this->EncodeResponse(qvmPing);
            // Log the response
            this->LogMessage(qbaResponse);
            // Send the encoded timestamps
            return qbaResponse;
        }
    } else {
        // Create a map
        QVariantMap qvmResponse;
        // Set the error
        QString sError = "No \"";
        sError.append(SERVERPANEL_METHOD_NOTATION_KEY);
        sError.append("\" was present in your request.");
        // Add the error
        qvmResponse.insert("sError", sError);
        // Add the timestamp
        qvmResponse.insert("sTimestamp", QDateTime::currentDateTime());
        // Grab the response
        QByteArray qbaResponse = this->EncodeResponse(qvmResponse);
        // Log the response
        this->LogMessage(qbaResponse);
        // Send the response
        return qbaResponse;
    }
}

/**
 * @paragraph This method simply logs messages to the log file, generally errors, requests and responses
 * @brief ServerPanel::LogMessage
 * @param QByteArray sMessage
 * @return void
 */
void ServerPanel::LogMessage(QByteArray qbaMessage) {
    // Load the log file
    QFile qfsLogFile(this->mConfig->value("systemPaths/logFile").toString());
    // Make sure we can open the file
    if (!qfsLogFile.open(QIODevice::Append | QIODevice::Text)) {
        // Send the error to the socket
        std::cout << "Cannot open log file for writing." << std::endl;
        // We're done
        return;
    }
    // Create a stream for the file
    QTextStream qtsOutput(&qfsLogFile);
    // Stream the message
    qtsOutput << QDateTime::currentDateTime().toString() << " : " << qbaMessage;
    // Close the log file
    qfsLogFile.close();
}
