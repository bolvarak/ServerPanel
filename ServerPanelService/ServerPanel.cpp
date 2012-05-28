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
    // Setup the databae
    this->mDbc = QSqlDatabase::addDatabase("QPSQL");
    // Set the host
    this->mDbc.setHostName(SERVERPANEL_DATABASE_HOST);
    // Set the port
    this->mDbc.setPort(SERVERPANEL_DATABASE_PORT);
    // Set the database
    this->mDbc.setDatabaseName(SERVERPANEL_DATABASE_NAME);
    // Set the username
    this->mDbc.setUserName(SERVERPANEL_DATABASE_USER);
    // Set the password
    this->mDbc.setPassword(SERVERPANEL_DATABASE_PASS);
    // Try to open the database
    if (!this->mDbc.open()) {
        // Notify the socket
        std::cout << this->mDbc.lastError().text().toStdString() << std::endl;
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
QVariantMap ServerPanel::AuthenticateUser(QString sUsername, QString sPassword) {
    // Set the return map
    QVariantMap qvmReturn;
    // Setup the query
    QSqlQuery qsqAccount(this->mDbc);
    // Prepare the query
    qsqAccount.prepare("SELECT * FROM accounts WHERE \"sUsername\" = ? AND \"sPassword\" = ?;");
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
            qvmReturn.insert("iAccountId",    qsqAccount.value(qsrAccount.indexOf("iAccountId")).toInt());       // Account ID
            qvmReturn.insert("sUsername",     qsqAccount.value(qsrAccount.indexOf("sUsername")).toString());     // Username
            qvmReturn.insert("sFirstName",    qsqAccount.value(qsrAccount.indexOf("sFirstName")).toString());    // First Name
            qvmReturn.insert("sLastName",     qsqAccount.value(qsrAccount.indexOf("sLastName")).toString());     // Last Name
            qvmReturn.insert("sEmailAddress", qsqAccount.value(qsrAccount.indexOf("sEmailAddress")).toString()); // Email Address
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
    // Decode the request
    QVariantMap qvmRequest = this->DecodeRequest(sRequest);
    // Make sure there is a method
    if (qvmRequest.contains(SERVERPANEL_METHOD_NOTATION_KEY)) {
        // Set the method
        QString sMethod = qvmRequest[SERVERPANEL_METHOD_NOTATION_KEY].toString();
        // Determine the method to execute
        if (sMethod.contains("AuthenticateUser", Qt::CaseSensitive)) { // User authentication
            // Run the method, encode the results and return the response
            return this->EncodeResponse(                        // Call to EncodeResponse
                        this->AuthenticateUser(                 // Call to AuthenticateUser
                            qvmRequest["sUsername"].toString(), // Username
                            qvmRequest["sPassword"].toString()  // Password
                        )
                   );                                           // The response is sent
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
        // Encode and return the response
        return this->EncodeResponse(qvmResponse).append("\n");
    }
}
