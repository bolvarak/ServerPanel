///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include "ServerPanel.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel* ServerPanel::mInstance             = NULL;
QString      ServerPanel::MethodLoadAccount     = "LOADACCOUNT";
QString      ServerPanel::MethodLoadDnsRecord   = "LOADDNSRECORD";
QString      ServerPanel::MethodLoadDnsRecords  = "LOADDNSRECORDS";
QString      ServerPanel::MethodLoadDomain      = "LOADDOMAIN";
QString      ServerPanel::MethodLoadDomains     = "LOADDOMAINS";
QString      ServerPanel::MethodLoadMailBox     = "LOADMAILBOX";
QString      ServerPanel::MethodLoadMailBoxes   = "LOADMAILBOXES";
QString      ServerPanel::MethodLoadMailDomain  = "LOADMAILDOMAIN";
QString      ServerPanel::MethodLoadMailDomains = "LOADMAILDOMAINS";
QString      ServerPanel::MethodPing            = "PING";
QString      ServerPanel::MethodSaveAccount     = "SAVEACCOUNT";
QString      ServerPanel::MethodSaveDnsRecord   = "SAVEDNSRECORD";
QString      ServerPanel::MethodSaveDomain      = "SAVEDOMAIN";
QString      ServerPanel::MethodSaveMailBox     = "SAVEMAILBOX";
QString      ServerPanel::MethodSaveMailDomain  = "SAVEMAILDOMAIN";

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
    std::cout << ServerPanelConfig::Get("databaseSettings.driver").toString().toStdString() << std::endl << std::endl;
    // Setup the databae
    this->mDbc    = QSqlDatabase::addDatabase(ServerPanelConfig::Get("databaseSettings.driver").toString());
    // Set the host
    this->mDbc.setHostName(ServerPanelConfig::Get("databaseSettings.host").toString());
    // Set the port
    this->mDbc.setPort(ServerPanelConfig::Get("databaseSettings.port").toInt());
    // Set the database
    this->mDbc.setDatabaseName(ServerPanelConfig::Get("databaseSettings.db").toString());
    // Set the username
    this->mDbc.setUserName(ServerPanelConfig::Get("databaseSettings.user").toString());
    // Set the password
    this->mDbc.setPassword(ServerPanelConfig::Get("databaseSettings.pass").toString());
    // We're done with the database configuration
    // Try to open the database
    if (!this->mDbc.open()) {
        // Setup the error placeholder
        QVariantMap qvmReturn;
        // Add the error
        qvmReturn.insert("sError",   this->mDbc.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Encode and return the response
        std::cout << QString(QtJson::Json::serialize(qvmReturn)).toStdString() << std::endl;
        // Terminate
        exit(1);
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

/**
 * @paragraph This method runs the query and logic for authenticating a user
 * @brief ServerPanel::AuthenticateUser
 * @param QString sUsername
 * @param QString sPassword
 * @return QVariantMap qvmReturn
 */
QVariantMap ServerPanel::AuthenticateUser(SpAccount spAccount) {
    // Set the return map
    QVariantMap qvmReturn;
    // Grab the query object
    QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.accountAuthentication").toString());
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
            // Populate the structure
            spAccount = SpAccount(qsqAccount.record());
        }
        // Set the account object
        qvmReturn.insert("oAccount", spAccount.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the account map
        return qvmReturn;
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

/**
 * @paragraph This method executes a system command and returns a simple boolean
 * @brief ServerPanel::ExecuteSystemCmd
 * @param QString sProgram
 * @param QStringList qslArguments
 * @return QBool
 */
QBool ServerPanel::ExecuteSystemCmd(QString sProgram, QStringList qslArguments) {
    // Try to execute the command
    if (QProcess::execute(sProgram, qslArguments) == QProcess::NormalExit) {
        // Everything's good, we're done
        return QBool(true);
    }
    // The execution failed
    return QBool(false);
}

QVariantMap ServerPanel::HandleCliRequest(QStringList qslArguments) {
    // Set a return map placeholder
    QVariantMap qvmReturn;
    // Loop through the arguments
    for (int iArgument = 1; iArgument < qslArguments.size(); iArgument++) {
        // Print the argument
        std::cout << qslArguments[iArgument].toStdString() << std::endl;
    }
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method is the work horse of the system.  It takes the request JSON, decodes it and determines what method in the system should be executed.
 * @brief ServerPanel::HandleRequest
 * @param QString sRequest
 * @return QByteArray sResponse
 */
QByteArray ServerPanel::HandleRequest(QString sRequest) {
    // Log the request to the socket
    std::cout << "REQUEST [" << QDateTime::currentDateTime().toString().toStdString() << "] => " << sRequest.toStdString() << std::endl;
    // Create a byte array of the request
    QByteArray qbaRequest;
    // Create a response placeholder
    QByteArray qbaResponse;
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
        if (sMethod.toUpper() == ServerPanel::MethodLoadAccount) {            // Load Account
            // Setup the structure
            SpAccount spAccount = SpAccount(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadAccount(spAccount));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadDnsRecord) {   // Load DNS Record
            // Setup the structure
            SpDnsRecord spDnsRecord = SpDnsRecord(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadDnsRecord(spDnsRecord));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadDnsRecords) {  // Load DNS Records
            // Setup the structure
            SpDnsRecord spDnsRecord = SpDnsRecord(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadDnsRecords(spDnsRecord));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadDomain) {      // Load Domain
            // Setup the structure
            SpDomain spDomain = SpDomain(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadDomain(spDomain));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadDomains) {     // Load Domains
            // Setup the structure
            SpDomain spDomain = SpDomain(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadDomains(spDomain));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadMailBox) {     // Load Mailbox
            // Setup the structure
            SpMailBox spMailBox = SpMailBox(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadMailBox(spMailBox));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadMailBoxes) {   // Load Mailboxes
            // Setup the structure
            SpMailBox spMailBox = SpMailBox(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadMailBoxes(spMailBox));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadMailDomain) {  // Load Mail Domain
            // Setup the structure
            SpMailDomain spMailDomain = SpMailDomain(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->LoadMailDomain(spMailDomain));
        } else if (sMethod.toUpper() == ServerPanel::MethodLoadMailDomains) { // Load Mail Domains
            // Setup the structure
            SpMailDomain spMailDomain = SpMailDomain(qvmRequest);
            // Set the response
            qbaResponse = this->EncodeResponse(this->LoadMailDomains(spMailDomain));
        } else if (sMethod.toUpper() == ServerPanel::MethodPing) {            // Ping
            // Create a map with the time stamps
            QVariantMap qvmPing;
            // Add the human readable time stamp
            qvmPing.insert("sTimestamp", QDateTime::currentDateTime());
            // Add the UTC timestamp
            qvmPing.insert("sUtcTimestamp", QDateTime::currentDateTimeUtc());
            // Add the seconds since epoch
            qvmPing.insert("sEpochTimestamp", QDateTime::currentMSecsSinceEpoch());
            // Grab the response
            qbaResponse = this->EncodeResponse(qvmPing);
        } else if (sMethod.toUpper() == ServerPanel::MethodSaveAccount) {     // Save Account
            // Setup the structure
            SpAccount spAccount = SpAccount(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->SaveAccount(spAccount));
        } else if (sMethod.toUpper() == ServerPanel::MethodSaveDnsRecord) {   // Save DNS Record
            // Setup the structure
            SpDnsRecord spDnsRecord = SpDnsRecord(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->SaveDnsRecord(spDnsRecord));
        } else if (sMethod.toUpper() == ServerPanel::MethodSaveDomain) {      // Save Domain
            // Setup the structure
            SpDomain spDomain = SpDomain(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->SaveDomain(spDomain));
        } else if (sMethod.toUpper() == ServerPanel::MethodSaveMailBox) {     // Save Mailbox
            // Setup the structure
            SpMailBox spMailBox = SpMailBox(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->SaveMailBox(spMailBox));
        } else if (sMethod.toUpper() == ServerPanel::MethodSaveMailDomain) {  // Save Mail Domain
            // Setup the structure
            SpMailDomain spMailDomain = SpMailDomain(qvmRequest);
            // Grab the response
            qbaResponse = this->EncodeResponse(this->SaveMailDomain(spMailDomain));
        } else {
            // Set the response map
            QVariantMap qvmResponse;
            // Set the error
            qvmResponse.insert("sError",   "No valid method was provided.");
            // Set the success status
            qvmResponse.insert("bSuccess", false);
            // Encode the response
            qbaResponse = this->EncodeResponse(qvmResponse);
        }
        // Log the response
        this->LogMessage(qbaResponse);
        // Log the response to the socket
        std::cout << "RESPONSE [" << QDateTime::currentDateTime().toString().toStdString() << "] => " << QString(qbaResponse).toStdString() << std::endl;
        // Return the response
        return qbaResponse;
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
    qbaResponse = this->EncodeResponse(qvmResponse);
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
    QFile qfsLogFile(":/Access.log");
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
    qtsOutput << QDateTime::currentDateTime().toString() << " : " << qbaMessage << "\n";
    // Close the log file
    qfsLogFile.close();
}

QVariantMap ServerPanel::LoadAccount(SpAccount spAccount) {
    // Set the return map
    QVariantMap qvmReturn;
    // Check for an account id
    if (spAccount.getProperty("iAccountId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectAccountByAccountId").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqAccount.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqAccount.size()) {
            // Set the error
            qvmReturn.insert("sError",   "The account could not be found in the system.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqAccount.next()) {
            // Populate the structure
            spAccount = SpAccount(qsqAccount.record());
        }
        // Set the account object
        qvmReturn.insert("oAccount", spAccount.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the account as a map
        return qvmReturn;
    }
    // Check for a username and password
    if (!spAccount.getProperty("sUsername").toString().isEmpty() && !spAccount.getProperty("sPassword").toString().isEmpty()) {
        // Grab the query object
        QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectAccountByUsernameAndPassword").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqAccount.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqAccount.size()) {
            // Set the error
            qvmReturn.insert("sError",   "The account could not be found in the system.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqAccount.next()) {
            // Populate the structure
            spAccount = SpAccount(qsqAccount.record());
        }
        // Set the account object
        qvmReturn.insert("oAccount", spAccount.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Check for a username
    if (!spAccount.getProperty("sUsername").toString().isEmpty()) {
        // Grab the query object
        QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectAccountByUsername").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqAccount.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqAccount.size()) {
            // Set the error
            qvmReturn.insert("sError",   "The account could not be found in the system.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqAccount.next()) {
            // Populate the structure
            spAccount = SpAccount(qsqAccount.record());
        }
        // Set the account object
        qvmReturn.insert("oAccount", spAccount.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // No keys were found, set the error
    qvmReturn.insert("sError",   "No key found, at least one is needed.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadDnsRecord(SpDnsRecord spDnsRecord) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for a record id
    if (spDnsRecord.getProperty("iRecordId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqDnsRecord = spDnsRecord.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQuery.selectDnsRecord").toString());
        // Try to execute the query
        if (!qsqDnsRecord.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqDnsRecord.size()) {
            // Set the error
            qvmReturn.insert("sError",   "The DNS record does not exist in the system.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqDnsRecord.next()) {
            // Populate the structure
            spDnsRecord = SpDnsRecord(qsqDnsRecord.record());
        }
        // Set the DNS record object
        qvmReturn.insert("oDnsRecord", spDnsRecord.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess",   true);
        // Return the map
        return qvmReturn;
    }
    // No record id was found so set the error
    qvmReturn.insert("sError",   "No iRecordId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadDnsRecords(SpDnsRecord spDnsRecord) {
    // Set a return list placeholder
    QVariantList qvlReturn;
    // Set a return map placeholder
    QVariantMap qvmReturn;
    // Check for a domain id
    if (spDnsRecord.getProperty("iDomainId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqDnsRecord = spDnsRecord.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectDnsRecordsByDomainId").toString());
        // Try to execute the query
        if (!qsqDnsRecord.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the list
            return qvmReturn;
        }
        // Check for results
        if (!qsqDnsRecord.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No DNS records were found.");
            qvmReturn.insert("sQuery", qsqDnsRecord.lastQuery());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the list
            return qvmReturn;
        }
        // Loop through the results
        while (qsqDnsRecord.next()) {
            // Populate the structure
            spDnsRecord = SpDnsRecord(qsqDnsRecord.record());
            // Append the structure to the list
            qvlReturn.append(spDnsRecord.toMap());
        }
        // Set the records into the map
        qvmReturn.insert("aDnsRecords", qvlReturn);
        // Set the success status
        qvmReturn.insert("bSuccess",    true);
        // Return the list
        return qvmReturn;
    }
    // Check for an account id
    if (spDnsRecord.getProperty("iAccountId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqDnsRecord = spDnsRecord.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectDnsRecordsByAccountId").toString());
        // Try to execute the query
        if (!qsqDnsRecord.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqDnsRecord.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No DNS records were found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqDnsRecord.next()) {
            // Populate the structure
            spDnsRecord = SpDnsRecord(qsqDnsRecord.record());
            // Append the structure to the list
            qvlReturn.append(spDnsRecord.toMap());
        }
        // Set the records
        qvmReturn.insert("aDnsRecords", qvlReturn);
        // Set the success status
        qvmReturn.insert("bSuccess",    true);
        // Return the map
        return qvmReturn;
    }
    // No keys were found, so set the error
    qvmReturn.insert("sError",   "No iDomainId or iAccountId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the list
    return qvmReturn;
}

QVariantMap ServerPanel::LoadDomain(SpDomain spDomain) {
    // Set a return map placeholder
    QVariantMap qvmReturn;
    // Check for a domain id
    if (spDomain.getProperty("iDomainId").toInt() > 0) {
        // Grab a query object
        QSqlQuery qsqDomain = spDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQuery.selectDomain").toString());
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
        if (!qsqDomain.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No DNS records were found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqDomain.next()) {
            // Populate the structure
            spDomain = SpDomain(qsqDomain.record());
        }
        // Set the domain object
        qvmReturn.insert("oDomain",  spDomain.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Set the error
    qvmReturn.insert("sError",   "No iDomainId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadDomains(SpDomain spDomain) {
    // Set a return list placeholder
    QVariantList qvlReturn;
    // Set a return map placeholder
    QVariantMap qvmReturn;
    // Check for an account id
    if (spDomain.getProperty("iAccountId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqDomains = spDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectDomains").toString());
        // Try to execute the query
        if (!qsqDomains.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqDomains.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqDomains.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No domains were found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqDomains.next()) {
            // Populate the structure
            spDomain = SpDomain(qsqDomains.record());
            // Append the record to the list
            qvlReturn.append(spDomain.toMap());
        }
        // Set the domains
        qvmReturn.insert("aDomains", qvlReturn);
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Set the error
    qvmReturn.insert("sError",   "No iAccountId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadMailBox(SpMailBox spMailBox) {
    // Set a return map placeholder
    QVariantMap qvmReturn;
    // Check for a mailbox id
    if (spMailBox.getProperty("iMailBoxId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqMailBox = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailBoxByMailBoxId").toString());
        // Try to execute the query
        if (!qsqMailBox.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailBox.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for resutls
        if (!qsqMailBox.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mailbox was found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailBox.next()) {
            // Populate the structure
            spMailBox = SpMailBox(qsqMailBox.record());
        }
        // Set the mailbox object
        qvmReturn.insert("oMailBox", spMailBox.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Check for a username and password
    if (!spMailBox.getProperty("sUsername").toString().isEmpty() && !spMailBox.getProperty("sPassword").toString().isEmpty()) {
        // Grab the query object
        QSqlQuery qsqMailBox = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailBoxByUsernameAndPassword").toString());
        // Try to execute the query
        if (!qsqMailBox.exec()) {
            // Set the error
            qvmReturn.insert("sError", qsqMailBox.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailBox.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mailbox was found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailBox.next()) {
            // Populate the structure
            spMailBox = SpMailBox(qsqMailBox.record());
        }
        // Set the mailbox object
        qvmReturn.insert("oMailBox", spMailBox.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Check for a password
    if (!spMailBox.getProperty("sUsername").toString().isEmpty()) {
        // Load the query object
        QSqlQuery qsqMailBox = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailBoxByUsername").toString());
        // Try to execute the query
        if (!qsqMailBox.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailBox.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailBox.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mailbox was found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailBox.next()) {
            // Populate the structure
            spMailBox = SpMailBox(qsqMailBox.record());
        }
        // Set the mailbox object
        qvmReturn.insert("oMailBox", spMailBox.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Set the error
    qvmReturn.insert("sError",   "No iMailBoxId, sUsername or sPassword was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadMailBoxes(SpMailBox spMailBox) {
    // Set a return list placeholder
    QVariantList qvlReturn;
    // Set a return map placeholder
    QVariantMap qvmReturn;
    // Check for a domain id
    if (spMailBox.getProperty("iDomainId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqMailBoxes = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailBoxesByDomainId").toString());
        // Try to execute the query
        if (!qsqMailBoxes.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailBoxes.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailBoxes.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mailboxes were found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailBoxes.next()) {
            // Populate the structure
            spMailBox = SpMailBox(qsqMailBoxes.record());
            // Append the mailbox
            qvlReturn.append(spMailBox.toMap());
        }
        // Set the map data
        qvmReturn.insert("aMailBoxes", qvlReturn);
        // Set the success status
        qvmReturn.insert("bSuccess",   true);
        // Return the map
        return qvmReturn;
    }
    // Check for an account id
    if (spMailBox.getProperty("iAccountId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqMailBoxes = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailBoxesByAccountId").toString());
        // Try to execute the query
        if (!qsqMailBoxes.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailBoxes.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailBoxes.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mailboxes were found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailBoxes.next()) {
            // Populate the structure
            spMailBox = SpMailBox(qsqMailBoxes.record());
            // Append the mailbox to the list
            qvlReturn.append(spMailBox.toMap());
        }
        // Set the map
        qvmReturn.insert("aMailBoxes", qvlReturn);
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Set the error
    qvmReturn.insert("sError",   "No iDomainId or iAccountId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadMailDomain(SpMailDomain spMailDomain) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for a mail domain id
    if (spMailDomain.getProperty("iMailDomainId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqMailDomain = spMailDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailDomainByMailDomainId").toString());
        // Try to execute the query
        if (!qsqMailDomain.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailDomain.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailDomain.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mail domain was found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailDomain.next()) {
            // Populate the structure
            spMailDomain = SpMailDomain(qsqMailDomain.record());
        }
        // Set the domain object
        qvmReturn.insert("oMailDomain", spMailDomain.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess",    true);
        // Return the map
        return qvmReturn;
    }
    // Check for a domain id
    if (spMailDomain.getProperty("iDomainId").toInt() > 0) {
        // Load the query object
        QSqlQuery qsqMailDomain = spMailDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailDomainByDomainId").toString());
        // Try to execute the query
        if (!qsqMailDomain.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailDomain.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailDomain.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mail domain was found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailDomain.next()) {
            // Populate the structure
            spMailDomain = SpMailDomain(qsqMailDomain.record());
        }
        // Set the mail domain object
        qvmReturn.insert("oMailDomain", spMailDomain.toMap());
        // Set the success status
        qvmReturn.insert("bSuccess",    true);
        // Return the map
        return qvmReturn;
    }
    // Set the error
    qvmReturn.insert("sError",   "No iMailDomainId or iDomainId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::LoadMailDomains(SpMailDomain spMailDomain) {
    // Set the return list placeholder
    QVariantList qvlReturn;
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for an account id
    if (spMailDomain.getProperty("iAccountId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqMailDomains = spMailDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.selectMailDomainsByAccountId").toString());
        // Try to execute the query
        if (!qsqMailDomains.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailDomains.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (!qsqMailDomains.size()) {
            // Set the error
            qvmReturn.insert("sError",   "No mail domains were found.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Loop through the results
        while (qsqMailDomains.next()) {
            // Populate the structure
            spMailDomain = SpMailDomain(qsqMailDomains.record());
            // Append the mail domain
            qvlReturn.append(spMailDomain.toMap());
        }
        // Set the map
        qvmReturn.insert("aMailDomains", qvlReturn);
        // Set the success status
        qvmReturn.insert("bSuccess",     true);
        // Return the map
        return qvmReturn;
    }
    // Set the error
    qvmReturn.insert("sError",   "No iAccountId was found.");
    // Set the success status
    qvmReturn.insert("bSuccess", false);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::SaveAccount(SpAccount spAccount) {
    // Setup the return map
    QVariantMap qvmReturn;
    // Check for an ID
    if (spAccount.getProperty("iAccountId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.checkForExistingUsername").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqAccount.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Check for results
        if (qsqAccount.size()) {
            // Set the error
            qvmReturn.insert("sError",   "An account with that username already exists in the system.");
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // Reset the query
        qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.insertAccount").toString());
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
        qslArguments.append(QString("-p ").append(spAccount.getProperty("sPassword").toString())); // User's Password
        qslArguments.append(spAccount.getProperty("sUsername").toString());
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
    QSqlQuery qsqAccount = spAccount.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.insertAccount").toString());
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
    qslArguments.append(QString("-d ").append("/home/").append(spAccount.getProperty("sUsername").toString())); // Home Directory
    qslArguments.append("-m");                                                                                  // Create Home Directory
    qslArguments.append(QString("-p ").append(spAccount.getProperty("sPassword").toString()));                  // User's Password
    qslArguments.append("-s /bin/bash");                                                                        // Login Shell
    qslArguments.append("-U");                                                                                  // Create User's Group
    qslArguments.append(spAccount.getProperty("sUsername").toString());                                         // User's Login Name
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

QVariantMap ServerPanel::SaveDnsRecord(SpDnsRecord spDnsRecord) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for a record id
    if (spDnsRecord.getProperty("iRecordId").toInt() > 0) {
        // Grab the SQL query
        QSqlQuery qsqDnsRecord = spDnsRecord.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.updateDnsRecord").toString());
        // Try to execute the query
        if (!qsqDnsRecord.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // All is well, set the message
        qvmReturn.insert("sMessage", "The DNS record has been successfully updated.");
        // Set the success status
        qvmReturn.insert("bSuccess", true);
    }
    // We are adding a new record, so grab the query
    QSqlQuery qsqDnsRecord = spDnsRecord.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.checkForExistingDnsRecord").toString());
    // Try to execute the query
    if (!qsqDnsRecord.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Grab the insert query
    qsqDnsRecord = spDnsRecord.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.insertDnsRecord").toString());
    // Try to execute the query
    if (!qsqDnsRecord.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqDnsRecord.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "The DNS record has been successfully created.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::SaveDomain(SpDomain spDomain) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for a domain id
    if (spDomain.getProperty("iDomainId").toInt() > 0) {
        // Grab the query
        QSqlQuery qsqDomain = spDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.updateDomain").toString());
        // Try to execute the query
        if (!qsqDomain.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqDomain.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // All is well, set the message
        qvmReturn.insert("sMessage", "The domain has been successfully updated.");
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Grab the query
    QSqlQuery qsqDomain = spDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.checkForExistingDomain").toString());
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
        // Set the error
        qvmReturn.insert("sError",   "The domain already exists in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Reset the query
    qsqDomain = spDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.insertDomain").toString());
    // Try to execute the query
    if (!qsqDomain.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "The domain was successfully added to the system.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::SaveMailBox(SpMailBox spMailBox) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for a mailbox id
    if (spMailBox.getProperty("iMailBoxId").toInt() > 0) {
        // Grab the query object
        QSqlQuery qsqMailBox = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.updateMailBox").toString());
        // Try to execute the statement
        if (!qsqMailBox.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailBox.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
           return  qvmReturn;
        }
        // All is well, set the message
        qvmReturn.insert("sMessage", "The mailbox has been successfully updated.");
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // Grab the query
    QSqlQuery qsqMailBox = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.checkForExistingMailBox").toString());
    // Try to execute the query
    if (!qsqMailBox.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqMailBox.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqMailBox.size()) {
        // Set the error
        qvmReturn.insert("sError",   "The mailbox already exists in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Grab the query
    qsqMailBox = spMailBox.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.insertMailBox").toString());
    // Try to execute the query
    if (!qsqMailBox.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqMailBox.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "The mailbox has been successfully added to the system.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}

QVariantMap ServerPanel::SaveMailDomain(SpMailDomain spMailDomain) {
    // Set the return map placeholder
    QVariantMap qvmReturn;
    // Check for a mail domain id
    if (spMailDomain.getProperty("iMailDomainId").toInt() > 0) {
        // Grab the query
        QSqlQuery qsqMailDomain = spMailDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.updateMailDomain").toString());
        // Try to execute the query
        if (!qsqMailDomain.exec()) {
            // Set the error
            qvmReturn.insert("sError",   qsqMailDomain.lastError().text());
            // Set the success status
            qvmReturn.insert("bSuccess", false);
            // Return the map
            return qvmReturn;
        }
        // All is well, set the message
        qvmReturn.insert("sMessage", "The mail domain has been successfully updated.");
        // Set the success status
        qvmReturn.insert("bSuccess", true);
        // Return the map
        return qvmReturn;
    }
    // We're adding a new mail domain so grab the query
    QSqlQuery qsqMailDomain = spMailDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.checkForExistingMailDomain").toString());
    // Try to execute the query
    if (!qsqMailDomain.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqMailDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Check for results
    if (qsqMailDomain.size()) {
        // Set the error
        qvmReturn.insert("sError",   "The mail domain is already in the system.");
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // Reset the query
    qsqMailDomain = spMailDomain.toQuery(this->mDbc, ServerPanelConfig::Get("sqlQueries.insertMailDomain").toString());
    // Try to execute the query
    if (!qsqMailDomain.exec()) {
        // Set the error
        qvmReturn.insert("sError",   qsqMailDomain.lastError().text());
        // Set the success status
        qvmReturn.insert("bSuccess", false);
        // Return the map
        return qvmReturn;
    }
    // All is well, set the message
    qvmReturn.insert("sMessage", "The mail domain has been successfully added to the system.");
    // Set the success status
    qvmReturn.insert("bSuccess", true);
    // Return the map
    return qvmReturn;
}
