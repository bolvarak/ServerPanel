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
 * @paragraph This method runs the query and logic for authenticating a user
 * @brief ServerPanel::AuthenticateUser
 * @param QString sUsername
 * @param QString sPassword
 * @return QVariantMap qvmReturn
 */
QVariantMap ServerPanel::AuthenticateUser(QString sUsername, QString sPassword) {
    // Set the return map
    QVariantMap qvmReturn;
    // Setup the account object
    SpAccount spAccount;
    // Set the username
    spAccount.sUsername = sUsername;
    // Set the password
    spAccount.sPassword = sPassword;
    // Grab the query object
    QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/accountAuthentication").toString());
    // Try to execute the query
    if (!qsqAccount.exec()) {
        // Setup the error
        qvmReturn.insert("sError",   qsqAccount.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        std::cout << qsqAccount.lastQuery().toStdString() << std::endl;
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
            spAccount.iAccountId          = qsqAccount.value(qsrAccount.indexOf("iAccountId")).toInt();             // Account ID
            spAccount.sEmailAddress       = qsqAccount.value(qsrAccount.indexOf("sEmailAddress")).toString();       // Email Address
            spAccount.sFirstName          = qsqAccount.value(qsrAccount.indexOf("sFirstName")).toString();          // First Name
            spAccount.sLastName           = qsqAccount.value(qsrAccount.indexOf("sLastName")).toString();           // Last Name
            spAccount.sPhoneNumber        = qsqAccount.value(qsrAccount.indexOf("sPhoneNumber")).toString();        // Phone Number
            spAccount.sStreetAddress      = qsqAccount.value(qsrAccount.indexOf("sStreetAddress")).toString();      // Street Address
            spAccount.sStreetAddressExtra = qsqAccount.value(qsrAccount.indexOf("sStreetAddressExtra")).toString(); // Street Address Extra
            spAccount.sCity               = qsqAccount.value(qsrAccount.indexOf("sCity")).toString();               // City
            spAccount.sState              = qsqAccount.value(qsrAccount.indexOf("sState")).toString();              // State
            spAccount.sPostalCode         = qsqAccount.value(qsrAccount.indexOf("sPostalCode")).toString();         // Zip Code
            spAccount.bEnabled            = qsqAccount.value(qsrAccount.indexOf("bEnabled")).toBool();              // Account Enabled
            spAccount.iAccountLevel       = qsqAccount.value(qsrAccount.indexOf("iAccountLevel")).toInt();          // Account Privilege Level
            // Reset the password
            spAccount.sPassword.clear();
        }
        // Return the account map
        return spAccount.toMap();
    }
    // Clear the map
    qvmReturn.clear();
    // Set the error
    qvmReturn.insert("sError",   "I was unable to find your account, please check your username and password and try again.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
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
    }
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

QVariantMap ServerPanel::SaveAccount(SpAccount spAccount) {
    // Setup the return map
    QVariantMap qvmReturn;
    // Check for an ID
    if (spAccount.iAccountId != 0) {
        // Grab the query object
        QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/updateAccount").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqAccount.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Set the system command
        QString sCommand = "usermod";
        // Setup the command arguments placeholder
        QStringList qslArguments;
        // Add the arguments
        qslArguments.append(QString("-p ").append(spAccount.sPassword));                  // User's Password
        // Try to add the system user
        if (!this->ExecuteSystemCmd(sCommand, qslArguments)) {
            // Set the error message
            qvmReturn.insert("sError",   "Failed to update the system account.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // All is well, set the message
        qvmReturn.insert("sMessage", "The account has been updated successfully.");
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // We are creating an account, so grab the query object
    QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/insertAccount").toString());
    // Try to execute the query
    if (!qsqAccount.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqAccount.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Set the system command
    QString sCommand = "useradd";
    // Setup the command arguments placeholder
    QStringList qslArguments;
    // Add the arguments
    qslArguments.append(QString("-d ").append("/home/").append(spAccount.sUsername)); // Home Directory
    qslArguments.append("-m");                                                        // Create Home Directory
    qslArguments.append(QString("-p ").append(spAccount.sPassword));                  // User's Password
    qslArguments.append("-s /bin/bash");                                              // Login Shell
    qslArguments.append("-U");                                                        // Create User's Group
    qslArguments.append(spAccount.sUsername);                                         // User's Login Name
    // Try to add the system user
    if (!this->ExecuteSystemCmd(sCommand, qslArguments)) {
        // Set the error message
        qvmReturn.insert("sError",   "Failed to add the system account.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "The account has been successfully created.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}
