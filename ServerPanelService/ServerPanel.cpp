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
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Methods ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

QBool ServerPanel::AddDnsRecord(int iAccountId, int iDomainId, QString sHostName, QString sAddress, QString sDirection, QString sType, int iPriority, QBool bEnabled) {

}

QBool ServerPanel::AddDomain(int iAccountId, QString sDomain, QBool bEnabled) {

}

QBool ServerPanel::AddMailBox(QString sUsername, QString sPassword, QString sName, QString sStorageDirectory, QString sStorageNode, QString sMailDirectory, int iQuota, int iBytes, QString sDomain, QString sTransport, QString sDepartment, QString sRank, QString sEmployeeId, QBool bEnableSmtp, QBool bEnableSecureSmtp, QBool bEnablePop3, QBool bEnableSecurePop3, QBool bEnableImap, QBool bEnableSecureImap, QBool bEnableDelivery, QBool bEnableSieveManagement, QBool bEnableSecureSieveManagement, QBool bEnableInternalDelivery, QString sDisclaimer, QBool bActive, QString sLocalPartition) {

}

QBool ServerPanel::AddMailDomain(int iDomainId, QString sDescription, QString sDisclaimer, int iMaxQuota, int iQuota, QString sTransport, QBool bBackupMx, int iDefaultUserQuota, QString sDefaultPasswordScheme, int iMinimumPasswordLength, int iMaximumPasswordLength, QBool bActive) {

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
        qvmReturn.insert("sError", qsqAccount.lastError().text());
        qvmReturn.insert("sQuery", qsqAccount.lastQuery());
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
        qvmReturn.insert("sError", "I was unable to find your account, please check your username and password and try again.");
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
        qvmRequest.insert("sError", "Your request was not able to be decoded, please check your request and ensure that it is valid.");
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
