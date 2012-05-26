///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "ServerPanelRpc.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelRpc* ServerPanelRpc::mInstance        = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * This method provides a single point of entrance to
 * this class.  It ensures that only one instance runs
 * at any given time, unless otherwise needed.
 *
 * @brief ServerPanelRpc::Instance
 * @return ServerPanelRpc*
 */
ServerPanelRpc* ServerPanelRpc::Instance() {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelRpc();
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * This is simply our constructor.  It sets up the globals
 *
 * @brief ServerPanelRpc::ServerPanelRpc
 */
ServerPanelRpc::ServerPanelRpc(QObject* cParent) : QObject(cParent) {
    // Setup the network
    this->mNetwork = new QHttp(cParent);
    // Setup the event handler
    connect(this->mNetwork, SIGNAL(done(bool)), this, SLOT(ProcessResponseData(bool)));
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * This is our destructor, it simply removes
 * all of the globals from memory
 *
 * @brief ServerPanelRpc::~ServerPanelRpc
 */
ServerPanelRpc::~ServerPanelRpc() {

}

///////////////////////////////////////////////////////////////////////////////
/// Public Methods ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * This method takes a JSON string and decodes it
 * into a pragmatically usable QVariantMap.
 *
 * @brief ServerPanelRpc::DecodeJson
 * @param QByteArray sJson
 * @return QVariantMap mapJson
 */
QVariantMap ServerPanelRpc::DecodeJson(QByteArray sJson) {
    // Convert the QByteArray to QString
    QString sJsonString(sJson);
    // Define a boolean
    bool bSuccess;
    // Process the JSON
    QVariantMap mapJson = QtJson::Json::parse(sJsonString, bSuccess).toMap();
    // Make sure everything went okay
    if (!bSuccess) {
        // Send a fatal message to the stream
        qFatal("Could not properly deserialize the JSON.  This usually occurs when the provided JSON is not valid.");
        // Terminate
        exit(1);
    }
    // Return the map
    return mapJson;
}

/**
 * This method serializes a QVariantMap into a JSON
 * string in the form of a QByteArray.
 *
 * @brief ServerPanelRpc::EncodeJson
 * @param QVariantMap mapJson
 * @return QByteArray sJson
 */
QByteArray ServerPanelRpc::EncodeJson(QVariantMap mapJson) {
    // Define a boolean
    bool bSuccess;
    // Serialize the JSON
    QByteArray sJson = QtJson::Json::serialize(mapJson, bSuccess);
    // Make sure everything went okay
    if (!bSuccess) {
        // Send a fatal message to the stream
        qFatal("Could not properly serialize the QVariantMap.");
        // Terminate
        exit(1);
    }
    // Return the JSON
    return sJson;
}

/**
 * This method makes a GET request to the provided
 * resource and decodes the response into the system
 *
 * @brief ServerPanelRpc::GetRequestData
 * @param QString sResource
 * @return void
 */
void ServerPanelRpc::GetRequestData(QString sResource) {
    // Create a new URI object
    QUrl cUrl;
    // Set the host
    cUrl.setHost(RPC_ENDPOINT_HOST);
    // Set the path
    cUrl.setPath(sResource);
    // Check the port
    if (RPC_ENDPOINT_PORT == 443) {                                      // HTTPS
        // Set the schema
        cUrl.setScheme("https");
    } else if ((RPC_ENDPOINT_PORT == 21) || (RPC_ENDPOINT_PORT == 20)) { // FTP
        // Set the schema
        cUrl.setScheme("ftp");
    } else {                                                             // HTTP
        // Set the schema
        cUrl.setScheme("http");
    }
    // Set the port
    cUrl.setPort(RPC_ENDPOINT_PORT);
    // Set the host
    this->mNetwork->setHost(RPC_ENDPOINT_HOST);
    // Make the request
    this->mNetwork->get(cUrl.toString());
}

/**
 * This method makes a POST request to the provieded
 * resource and decodes the response into the system
 *
 * @brief ServerPanelRpc::SendRequestData
 * @param QString sResource
 * @param QVariantMap mapJson
 * @return void
 */
void ServerPanelRpc::SendRequestData(QString sResource, QVariantMap mapJson) {
    // Serialize the JSON
    QByteArray sJson = this->EncodeJson(mapJson);
    // Create a new URI object
    QUrl cUrl;
    // Set the host
    cUrl.setHost(RPC_ENDPOINT_HOST);
    // Set the path
    cUrl.setPath(sResource);
    // Check the port
    if (RPC_ENDPOINT_PORT == 443) {                                      // HTTPS
        // Set the schema
        cUrl.setScheme("https");
    } else if ((RPC_ENDPOINT_PORT == 21) || (RPC_ENDPOINT_PORT == 20)) { // FTP
        // Set the schema
        cUrl.setScheme("ftp");
    } else {                                                             // HTTP
        // Set the schema
        cUrl.setScheme("http");
    }
    // Set the port
    cUrl.setPort(RPC_ENDPOINT_PORT);
    // Set the query delimiters
    cUrl.setQueryDelimiters(',', ';');
    // Add a query parameter
    cUrl.addQueryItem("oJson", QString(sJson));
    // Set the network host
    this->mNetwork->setHost(RPC_ENDPOINT_HOST);
    // Make the request
    this->mNetwork->post(cUrl.toString(), cUrl.encodedQuery());
}

///////////////////////////////////////////////////////////////////////////////
/// Slots ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * This is the callback method for the QHttp
 * object.  It receives and decodes the response
 * and sets both the encoded and decoded response
 * into the system
 *
 * @brief ServerPanelRpc::ProcessResponseData
 * @param bool bError
 * @return void
 */
void ServerPanelRpc::ProcessResponseData(bool bError) {
    // Make sure there is no error
    if (!bError) {
        // Log the Resposne
        qDebug(this->mNetwork->readAll());
        // Set the encoded JSON into the system
        this->mEncodedResponse = this->mNetwork->readAll();
        // Set the decoded response into the system
        this->mDecodedResponse = this->DecodeJson(this->mNetwork->readAll());
    } else {
        // Log the error message
        qDebug(this->mNetwork->errorString().toLatin1());
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Getters //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * This method returns the response from the web request
 * in a usable QVariantMap format.
 *
 * @brief ServerPanelRpc::GetDecodedResponse
 * @return QVariantMap mDecodedResponse
 */
QVariantMap ServerPanelRpc::GetDecodedResponse() {
    // Return the decoded JSON response
    return this->mDecodedResponse;
}

/**
 * This method returns the raw JSON response from the
 * web request in QString format.
 *
 * @brief ServerPanelRpc::GetEncodedResponse
 * @return QString mEncodedResponse
 */
QString ServerPanelRpc::GetEncodedResponse() {
    // Return the encoded JSON response
    return QString(this->mEncodedResponse);
}
