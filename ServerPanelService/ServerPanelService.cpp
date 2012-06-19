///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "ServerPanelService.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelService* ServerPanelService::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelService* ServerPanelService::Instance() {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelService();
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelService::ServerPanelService(QObject* cParent) : QObject(cParent) {
    // Instantiate our backbone class
    ServerPanel::Instance();
    // Setup the new connection event handler
    connect(&this->mServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    // Setup the address and port for the socket to listen on
    this->mServer.listen(QHostAddress::Any, SERVERPANELSERVICE_LISTEN_PORT);
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelService::~ServerPanelService() {
    // Close the server connection
    this->mServer.close();
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Methods ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

QByteArray ServerPanelService::HandleRequest(QString sJson) {
    // Create a success boolean
    bool bSuccess;
    // Decode the json
    QVariantMap qvmJson = QtJson::Json::parse(sJson, bSuccess).toMap();
    // Make sure the decode succeeded
    if (!bSuccess) {
        // Create a response map
        QVariantMap qvmResponse;
        // Add the error
        qvmResponse.insert("sError", "Unable to decode the request JSON string.  This usually occurs when the JSON is not valid.");
        // Encode the JSON
        QByteArray sResponseJson = QtJson::Json::serialize(qvmResponse);
        // Return the JSON
        return sResponseJson;
    }
    // Create a response map
    QVariantMap qvmResponse;
    // Add the called method
    qvmResponse.insert("sCalledMethod", qvmJson["sMethod"].toString());
    // Add the time
    qvmResponse.insert("sServerTime", QDateTime::currentDateTime());
    // Encode the response
    QByteArray sResponseJson = QtJson::Json::serialize(qvmResponse);
    // Return the response
    return sResponseJson;
}

///////////////////////////////////////////////////////////////////////////////
/// Slots ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method slot accepts new connections
 * @brief ServerPanel::acceptConnection()
 * @return void
 */
void ServerPanelService::acceptConnection() {
    // Setup the client
    this->mClient = this->mServer.nextPendingConnection();
    // Setup the client readyRead() event handler
    connect(this->mClient, SIGNAL(readyRead()), this, SLOT(startRead()));
}

/**
 * @paragraph This method slot handles the reading and responding of the server
 * @brief ServerPanel::startRead()
 * @return void
 */
void ServerPanelService::startRead() {
    // Create an output block
    QByteArray qbaOutput;
    // Read the client data into the buffer
    QByteArray qbaClientData = this->mClient->readAll();
    // Handle the request
    QByteArray qbaClientResponse = ServerPanel::Instance()->HandleRequest(QString(qbaClientData));
    // Create a data stream
    QDataStream qdsResponse(&qbaOutput, QIODevice::WriteOnly);
    // Set the version
    qdsResponse.setVersion(QDataStream::Qt_4_8);
    // Send a 0 response
    qdsResponse << (quint16) 0;
    // Send the JSON response
    qdsResponse << qbaClientResponse;
    // Reset the response
    qdsResponse.device()->seek(0);
    // Send the block size
    qdsResponse << (quint16) (qbaOutput.size() - sizeof(quint16));
    // Write the response
    this->mClient->write(qbaOutput);
    // Disconnect
    this->mClient->disconnectFromHost();
}
