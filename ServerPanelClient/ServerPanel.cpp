///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <ServerPanel.h>

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel* ServerPanel::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel* ServerPanel::Instance(QObject* cParent) {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanel(cParent);
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel::ServerPanel(QObject* cParent) : QObject(cParent), mOk(true) {
    // Load the configuration
    this->mConfig = new QSettings(":/ServerPanel.ini", QSettings::IniFormat);
    // Setup the databae
    this->mDbc    = QSqlDatabase::addDatabase("QSQLITE");
    // Set the database
    this->mDbc.setDatabaseName("/home/tbrown/Documents/ServerPanel/ServerPanelClient/ServerPanelClient.sp");
    // Try to open the database
    if (!this->mDbc.open()) {
        // Dispatch a message
        this->DispatchMessageBox(this->mDbc.lastError().text(), Error);
    }
    // Create the client
    this->mClient = new QTcpSocket(this);
    // Setup the connectors
    connect(this->mClient, SIGNAL(connected()),                         this, SLOT(TransferData()));
    connect(this->mClient, SIGNAL(readyRead()),                         this, SLOT(ReadResponse()));
    connect(this->mClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketError(QAbstractSocket::SocketError)));
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanel::~ServerPanel() {
    // Delete the configuration
    delete this->mConfig;
    // Close the database connection
    this->mDbc.close();
    // Close the client
    this->mClient->close();
    // Delete the instance
    delete this->mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Public Methods ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method makes the call to the server to authenticate a user
 * @brief ServerPanel::AuthenticateUser
 * @param SpLocalAccount slaAccount
 * @return bool
 */
bool ServerPanel::AuthenticateUser(SpAccount spAccount) {
    // Try to make the call to the server
    if (this->MakeRequest("LoadAccount", spAccount.toMap())) {
        // Check for success
        if (!this->mResponse["bSuccess"].toBool()) {
            // We're done
            return false;
        }
    } else {
        // We're done
        return false;
    }
    // Set the account
    this->mCurrentRemoteAccount = SpAccount(this->mResponse["oAccount"].toMap());
    // We're done
    return true;
}

/**
 * @paragraph This method decrypts a system encrypted hash
 * @brief ServerPanel::DecryptEntity
 * @param QByteArray qbaKey
 * @param QByteArray qbaVector
 * @param QByteArray qbaHash
 * @return QString
 */
QVariantMap ServerPanel::DecryptEntity(QByteArray qbaKey, QByteArray qbaVector, QByteArray qbaHash) {
    // Initialize the cryptographer
    QCA::Initializer qiInitializer = QCA::Initializer();
    // Create the cipher
    QCA::Cipher qcrCipher          = QCA::Cipher("aes128", QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Decode, QCA::SymmetricKey(qbaKey), QCA::InitializationVector(qbaVector));
    // Decode the hash
    QCA::SecureArray qsaPlain      = qcrCipher.process(QCA::SecureArray(qbaHash));
    // Setup the return map
    QVariantMap qvmReturn;
    // Check the decryption status
    if (qcrCipher.ok()) {
        // Set the plain text entity
        qvmReturn.insert("sPlainText", QString(qsaPlain.data()));
        // Set the decryption status
        qvmReturn.insert("bSuccess", true);
    } else {
        // Set the decryption status
        qvmReturn.insert("bSuccess", false);
    }
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method deletes a local account and server from the local database
 * @brief ServerPanel::DeleteLocalServer
 * @param QString sAddress
 * @return bool
 */
bool ServerPanel::DeleteLocalServer(QString sAddress) {
    // Setup the account structure
    SpLocalAccount slaAccount;
    // Add the server address
    slaAccount.saveProperty("sAddress", sAddress);
    // Setup the server structure
    SpLocalServer slsServer;
    // Add the server address
    slsServer.saveProperty("sAddress", sAddress);
    // Grab the account QSqlQuery object
    QSqlQuery qsqAccount = slaAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/deleteLocalAccount").toString());
    // Grab the server QSqlQuery object
    QSqlQuery qsqServer  = slsServer.toQuery(this->mDbc, this->mConfig->value("sqlQueries/deleteLocalServer").toString());
    // Try to delete the account
    if (!qsqAccount.exec()) {
        // Dispatch a message
        this->DispatchMessageBox(qsqAccount.lastError().text(), Error);
        // We're done
        return false;
    }
    // Try to delete the server
    if (!qsqServer.exec()) {
        // Dispatch a message
        this->DispatchMessageBox(qsqAccount.lastError().text(), Error);
        // We're done
        return false;
    }
    // We're done
    return true;
}

/**
 * @paragraph This method dispatches a QMessageBox to the user
 * @brief ServerPanel::DispatchMessageBox
 * @param QString sMessage
 * @param ServerPanelMessage spmType
 * @return int
 */
int ServerPanel::DispatchMessageBox(QString sMessage, ServerPanelMessage spmType) {
    // Create the message box
    QMessageBox qmbMessage;
    // Set the informative text
    qmbMessage.setInformativeText(sMessage);
    // Determinet he message type
    switch(spmType) {
        // Errors
        case Error :
            // Set the icon
            qmbMessage.setIcon(QMessageBox::Critical);
            // Set the text
            qmbMessage.setText("Error!");
        // We're done
        break;
        // Warnings
        case Warning :
            // Set the icon
            qmbMessage.setIcon(QMessageBox::Warning);
            // Set the text
            qmbMessage.setText("Warning!");
        // We're done
        break;
        // Notices
        case Notification :
            // Set the icon
            qmbMessage.setIcon(QMessageBox::Information);
            // Set the text
            qmbMessage.setText("Notice!");
        // We're done
        break;
        // Success Messages
        case Success :
            // Set the icon
            qmbMessage.setIcon(QMessageBox::NoIcon);
            // Set the text
            qmbMessage.setText("Success!");
        // We're done
        break;
    }
    // Execute the message box
    return qmbMessage.exec();
}

/**
 * @paragraph This method encrypts a data string and returns a map of the key, hash and vector
 * @brief ServerPanel::EncryptEntity
 * @param QString sData
 * @return QVariantMap
 */
QVariantMap ServerPanel::EncryptEntity(QString sData) {
    // Initialize the cryptographer
    QCA::Initializer qiInitialization           = QCA::Initializer();
    // Generate the key
    QCA::SymmetricKey qskKey                    = QCA::SymmetricKey(2048);
    // Generate the vector
    QCA::InitializationVector qivInitialization = QCA::InitializationVector(2048);
    // Create the cipher
    QCA::Cipher qcrCipher                       = QCA::Cipher("aes128", QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Encode, qskKey, qivInitialization);
    // Make sure AES128 is supported
    if (!QCA::isSupported("aes128-cbc-pkcs7")) {
        qDebug("AES128 CBC PKCS7 not supported - please check if qca-ossl plugin installed correctly !");
    }
    // Encrypt the data
    QCA::SecureArray qsaHash = qcrCipher.process(QCA::SecureArray(sData.toAscii()));
    // Setup the map to resturn
    QVariantMap qvmReturn;
    // Add the key
    qvmReturn.insert("qbaKey",    qskKey.toByteArray());
    // Add the vector
    qvmReturn.insert("qbaVector", qivInitialization.toByteArray());
    // Add the hash
    qvmReturn.insert("qbaHash",   qsaHash.toByteArray());
    // Return the map
    return qvmReturn;
}

/**
 * @paragraph This method returns the okay status after a request is made to the server
 * @brief ServerPanel::IsOk
 * @return bool
 */
bool ServerPanel::IsOk() {
    // Return the ok status
    return this->mOk;
}

/**
 * @paragraph This method makes the call to the server to load DNS records
 * @brief ServerPanel::LoadDnsRecords
 * @param SpDnsRecord spDnsRecord
 * @return QVariantList
 */
QVariantList ServerPanel::LoadDnsRecords(SpDnsRecord spDnsRecord) {
    // Try to make the call to the server
    if (this->MakeRequest("LoadDnsRecords", spDnsRecord.toMap())) {
        // Check for success
        if (this->mResponse["bSuccess"].toBool()) {
            // We're done
            return this->mResponse["aDnsRecords"].toList();
        }
    }
    // We're done
    return QVariantList();
}

/**
 * @paragraph This method makes the call to the server to load domains
 * @brief ServerPanel::LoadDomains
 * @param SpDomain spDomain
 * @return QVariantList
 */
QVariantList ServerPanel::LoadDomains(SpDomain spDomain) {
    // Try to make the call to the server
    if (this->MakeRequest("LoadDomains", spDomain.toMap())) {
        // Check for success
        if (this->mResponse["bSuccess"].toBool()) {
            // We're done
            return this->mResponse["aDomains"].toList();
        }
    }
    // We're done
    return QVariantList();
}

/**
 * @paragraph This method loads an account saved in the local database
 * @brief ServerPanel::LoadLocalAccount
 * @param SpLocalAccount slaAccount
 * @return bool
 */
bool ServerPanel::LoadLocalAccount(SpLocalAccount slaAccount) {
    // Check for a username and password
    if (!slaAccount.getProperty("sUsername").toString().isEmpty() && !slaAccount.getProperty("sPassword").toString().isEmpty()) {
        // Grab the QSqlQuery object
        QSqlQuery qsqLocalAccount = slaAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/selectLocalAccountByUsernameAndPassword").toString());
        // Try to execute the query
        if (!qsqLocalAccount.exec()) {
            // Dispatch a message
            this->DispatchMessageBox(qsqLocalAccount.lastError().text(), Error);
            // We're done
            return false;
        }
        // Check for results
        if (!qsqLocalAccount.size()) {
            // We're done
            return false;
        }
        // Loop through the results
        while (qsqLocalAccount.next()) {
            // Set the account object
            slaAccount = SpLocalAccount(qsqLocalAccount.record());
        }
        // Set the account into the system
        this->mCurrentAccount = slaAccount;
        // We're done
        return true;
    }
    // Check for a server address
    if (!slaAccount.getProperty("sAddress").toString().isEmpty()) {
        // Grab the QSqlQuery object
        QSqlQuery qsqAccount = slaAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/selectLocalServerAccount").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Dispatch a message
            this->DispatchMessageBox(qsqAccount.lastError().text(), Error);
            // We're done
            return false;
        }
        // Check for results
        if (!qsqAccount.size()) {
            // We're done
            return false;
        }
        // Loop through the results
        while (qsqAccount.next()) {
            // Populate the structure
            slaAccount = SpLocalAccount(qsqAccount.record());
        }
        // Set the current account
        this->mCurrentAccount = slaAccount;
        // We're done
        return true;
    }
    // Dispatch a message
    this->DispatchMessageBox("No account identifier found.", Error);
    // We're done
    return false;
}

/**
 * @paragraph This method loads a saved server from the local database
 * @brief ServerPanel::LoadLocalServer
 * @param SpLocalServer slsServer
 * @return bool
 */
bool ServerPanel::LoadLocalServer(SpLocalServer slsServer) {
    // Are we loading by id
    if (slsServer.getProperty("iServerId").toInt() > 0) {
        // Grab the QSqlQuery object
        QSqlQuery qsqLocalServer = slsServer.toQuery(this->mDbc, this->mConfig->value("sqlQueries/selectLocalServerByServerId").toString());
        // Try to execute the query
        if (!qsqLocalServer.exec()) {
            // Dispatch a message
            this->DispatchMessageBox(qsqLocalServer.lastError().text(), Error);
            // We're done
            return false;
        }
        // Check for results
        if (!qsqLocalServer.size()) {
            // We're done
            return false;
        }
        // Loop through the results
        while (qsqLocalServer.next()) {
            // Set the server object
            slsServer = SpLocalServer(qsqLocalServer.record());
        }
        // Set the server into the system
        this->mCurrentServer = slsServer;
        // We're done
        return true;
    }
    // Are we loading by server address
    if (!slsServer.getProperty("sAddress").toString().isEmpty()) {
        // Grab the QSqlQuery object
        QSqlQuery qsqLocalServer = slsServer.toQuery(this->mDbc, this->mConfig->value("sqlQueries/selectLocalServerByServerAddress").toString());
        // Try to execute the query
        if (!qsqLocalServer.exec()) {
            // We're done
            return false;
        }
        // Check for results
        if (!qsqLocalServer.size()) {
            // We're done
            return false;
        }
        // Loop through the results
        while (qsqLocalServer.next()) {
            // Populate the structure
            slsServer = SpLocalServer(qsqLocalServer.record());
        }
        // Set the server into the system
        this->mCurrentServer = slsServer;
        // We're done
        return true;
    }
    // Dispatch a message
    this->DispatchMessageBox("No server identifier was found.  Please check for a valid server ID or server address and try again.", Error);
    // We're done
    return false;
}

/**
 * @paragraph This method is responsible for saving and updating local saved accounts
 * @brief ServerPanel::SaveLocalAccount
 * @param SpLocalAccount slaAccount
 * @return bool
 */
bool ServerPanel::SaveLocalAccount(SpLocalAccount slaAccount) {
    // Check for an account id
    if (slaAccount.getProperty("iAccountId").toInt() > 0) {
        // Grab the QSqlQuery object
        QSqlQuery qsqAccount = slaAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/updateLocalAccount").toString());
        // Try to execute the query
        if (!qsqAccount.exec()) {
            // Dispatch a message
            this->DispatchMessageBox(qsqAccount.lastError().text(), Error);
            // We're done
            return false;
        }
        // Set the account into the system
        this->mCurrentAccount = slaAccount;
        // We're done
        return true;
    }
    // Grab the QSqlQuery object
    QSqlQuery qsqAccount = slaAccount.toQuery(this->mDbc, this->mConfig->value("sqlQueries/insertLocalAccount").toString());
    // Try to execute the query
    if (!qsqAccount.exec()) {
        // Dispatch a message
        this->DispatchMessageBox(qsqAccount.lastError().text(), Error);
        // We're done
        return false;
    }
    // Set the new account id
    slaAccount.saveProperty("iAccountId", qsqAccount.lastInsertId().toInt());
    // Set the account into the system
    this->mCurrentAccount = slaAccount;
    // We're done
    return true;
}

/**
 * @paragraph This method is responsible for saving and updating local saved servers
 * @brief ServerPanel::SaveLocalServer
 * @param SpLocalServer slsServer
 * @return bool
 */
bool ServerPanel::SaveLocalServer(SpLocalServer slsServer) {
    // Check for a server id
    if (slsServer.getProperty("iServerId").toInt() > 0) {
        // Grab the QSqlQuery object
        QSqlQuery qsqServer = slsServer.toQuery(this->mDbc, this->mConfig->value("sqlQueries/updateLocalServer").toString());
        // Try to execute the query
        if (!qsqServer.exec()) {
            // Dispatch a message
            this->DispatchMessageBox(qsqServer.lastError().text(), Error);
            // We're done
            return false;
        }
        // Set the server into the system
        this->mCurrentServer = slsServer;
        // We're done
        return true;
    }
    // Grab the QSqlQuery object
    QSqlQuery qsqServer = slsServer.toQuery(this->mDbc, this->mConfig->value("sqlQueries/insertLocalServer").toString());
    // Try to execute the query
    if (!qsqServer.exec()) {
        // Dispatch a message
        this->DispatchMessageBox(qsqServer.lastError().text(), Error);
        // We're done
        return false;
    }
    // Set the new server id
    slsServer.saveProperty("iServerId", qsqServer.lastInsertId().toInt());
    // Set the server into the system
    this->mCurrentServer = slsServer;
    // We're done
    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Methods ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method handles the connection to the server
 * @brief ServerPanel::MakeRequest
 * @param QString sMethod
 * @param QVariantMap qvmRequestData
 * @return bool
 */
bool ServerPanel::MakeRequest(QString sMethod, QVariantMap qvmRequestData) {
    // Reset the block size
    this->mBlockSize = 0;
    // Append the method to the map
    qvmRequestData.insert("sMethod", sMethod);
    // Set the request map
    this->mRequest = qvmRequestData;
    // Make sure we have a server address
    if (this->mCurrentServer.getProperty("sAddress").toString().isEmpty()) {
        // Dispatch a message
        this->DispatchMessageBox("No server address provided.", Error);
        // We're done
        return false;
    }
    // Make sure we have a port
    if (this->mCurrentServer.getProperty("iPort").toInt() < 1) {
        // Dispatch a message
        this->DispatchMessageBox("No server port provided.", Error);
        // We're done
        return false;
    }
    // Abort the current connection
    this->mClient->abort();
    // Check for open connections
    if (!this->mClient->isOpen()) {
        // Create a new connection
        this->mClient->connectToHost(this->mCurrentServer.getProperty("sAddress").toString(), this->mCurrentServer.getProperty("iPort").toInt());
    }
    // We're done
    return true;
}

/**
 * @paragraph This method processes the JSON response from the server
 * @brief ServerPanel::ProcessResponse()
 * @return void
 */
void ServerPanel::ProcessResponse() {
    qDebug() << this->mJsonResponse;
    // Set a conversion boolean
    bool bDeserialized;
    // Decode the response
    this->mResponse = QtJson::Json::parse(this->mJsonResponse, bDeserialized).toMap();
    // Make sure the JSON was deserialized
    if (!bDeserialized) {
        // Dispatch the message
        this->DispatchMessageBox("Could not decode the server response.", Error);
    }
    // Check for an error
    if (!this->mResponse["bSuccess"].toBool()) {
        // Set the error into the system
        this->mError = this->mResponse["sError"].toString();
        // Dispatch the message
        this->DispatchMessageBox(this->mError, Error);
    }
    // Set the okay status
    this->mOk = this->mResponse["bSuccess"].toBool();
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Slots //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method reads the socket response
 * @brief ServerPanel::ReadResponse
 * @return void
 */
void ServerPanel::ReadResponse() {
    // Create the data stream
    QDataStream qdsServerPanel(this->mClient);
    // Check the block size
    if (this->mBlockSize == 0) {
        // Make sure we have a valid amount of daya
        if (this->mClient->bytesAvailable() < (int) sizeof(quint16)) {
            // We're done
            return;
        }
        // Read the data
        qdsServerPanel >> this->mBlockSize;
    }
    // See if we have read all of the data
    if (this->mClient->bytesAvailable() < this->mBlockSize) {
        // We're done
        return;
    }
    // Set a response placeholder
    QString sJson;
    // Read the stream
    qdsServerPanel >> sJson;
    // Check for data
    if (sJson == this->mJsonResponse) {
        // Rerun the read
        QTimer::singleShot(0, this, SLOT(ReadResponse()));
        // We're done
        return;
    }
    // Set the response
    this->mJsonResponse = sJson;
    // Process the response
    this->ProcessResponse();
}

/**
 * @paragraph This method hadnles the dispatching socket errors
 * @brief ServerPanel::SocketError
 * @param QAbstractSocket::SocketError qseError
 * @return void
 */
void ServerPanel::SocketError(QAbstractSocket::SocketError qseError) {
    switch (qseError) {
        // Remote Host Closed
        case QAbstractSocket::RemoteHostClosedError:
            // Dispatch the message
            this->DispatchMessageBox("The remote ServerPanel instance closed unexpectedly.", Error);
        // We're Done
        break;
        // Host Not Found
        case QAbstractSocket::HostNotFoundError:
            // Dispatch the message
        this->DispatchMessageBox("The remote ServerPanel instance could not be found.", Error);
        // We're done
        break;
        // Connection Refused
        case QAbstractSocket::ConnectionRefusedError:
            // Dispatch the message
            this->DispatchMessageBox("The remote ServerPanel instance refused the connection.", Error);
        // We're done
        break;
        // Everything else
        default:
            // Dispatch the message
            this->DispatchMessageBox(this->mClient->errorString(), Error);
    }
}

/**
 * @paragraph This slot method sends data to the server
 * @brief ServerPanel::TransferData
 * @return void
 */
void ServerPanel::TransferData() {
    // Convert the map to JSON
    QByteArray qbaRequest = QtJson::Json::serialize(this->mRequest);
    // Create an empty byte array
    QByteArray qbaOutput;
    // Create a data stream
    QDataStream qdsResponse(&qbaOutput, QIODevice::WriteOnly);
    // Send a 0 response
    qdsResponse << (quint16) 0;
    // Send the JSON response
    qdsResponse << QString(qbaRequest);
    // Reset the response
    qdsResponse.device()->seek(0);
    // Send the block size
    qdsResponse << (quint16) (qbaOutput.size() - sizeof(quint16));
    // Write the response
    this->mClient->write(qbaOutput);
}

///////////////////////////////////////////////////////////////////////////////
/// Getters //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method returns the current remote account
 * @brief ServerPanel::GetAccount
 * @return SpAccount
 */
SpAccount ServerPanel::GetAccount() {
    // Return the current remote account
    return this->mCurrentRemoteAccount;
}

/**
 * @paragraph This method returns the currently authenticated user
 * @brief ServerPanel::GetCurrentAccount
 * @return SpLocalAccount
 */
SpLocalAccount ServerPanel::GetCurrentAccount() {
    // Return the current account
    return this->mCurrentAccount;
}

/**
 * @paragraph This method returns the current server
 * @brief ServerPanel::GetCurrentServer
 * @return SpLocalServer
 */
SpLocalServer ServerPanel::GetCurrentServer() {
    // Return the current server
    return this->mCurrentServer;
}

/**
 * @paragraph This method returns the current error in the system
 * @brief ServerPanel::GetError
 * @return QString
 */
QString ServerPanel::GetError() {
    // Return the error
    return this->mError;
}

/**
 * @paragraph This method returns all of the saved servers in the local database
 * @brief ServerPanel::GetLocalServers
 * @return QVariantList
 */
QVariantList ServerPanel::GetLocalServers() {
    // Create a new QSqlQuery object
    QSqlQuery qsqServers(this->mDbc);
    // Create a return QVariantList
    QVariantList qvlServers;
    // Prepare the query
    qsqServers.prepare(this->mConfig->value("sqlQueries/selectAllLocalServers").toString());
    // Try to execute the query
    if (!qsqServers.exec()) {
        // Dispatch a message
        this->DispatchMessageBox(qsqServers.lastError().text(), Error);
        // Return the list
        return qvlServers;
    }
    // Loop through the results
    while (qsqServers.next()) {
        // Create the new structure
        SpLocalServer slsServer(qsqServers.record());
        // Append the server to the list
        qvlServers.append(slsServer.toMap());
    }
    // We're done
    return qvlServers;
}

/**
 * @paragraph This method returns the last response from the server
 * @brief ServerPanel::GetResponse
 * @return QVariantMap
 */
QVariantMap ServerPanel::GetResponse() {
    // Return the last response
    return this->mResponse;
}
