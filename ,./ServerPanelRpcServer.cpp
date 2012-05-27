///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "ServerPanelRpcServer.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelRpcServer* ServerPanelRpcServer::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelRpcServer* ServerPanelRpcServer::Instance() {
    // Check for an instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelRpcServer();
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelRpcServer::ServerPanelRpcServer(QObject *cParent) : QTcpServer(cParent), bDisabled(false) {
    // Tell the socket which address and port to listen on
    listen(QHostAddress::Any, RPC_PORT);
}

///////////////////////////////////////////////////////////////////////////////
/// Public Methods ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelRpcServer::incomingConnection(int iSocket) {
    // Check to see if the server is disabled
    if (this->bDisabled) {
        // We're done
        return;
    }
    // Create a new asynchronous socket
    QTcpSocket* cSocket = new QTcpSocket(this);
    // Add the event handler to read the client
    connect(cSocket, SIGNAL(readyRead()), this, SLOT(ReadClient()));
    // Add the event handler for disconnecting a client
    connect(cSocket, SIGNAL(disconnected()), this, SLOT(DiscardClient()));
    // Set the socket descriptor
    cSocket->setSocketDescriptor(iSocket);
}

void ServerPanelRpcServer::Pause() {
    // Disable the server
    this->bDisabled = true;
}

void ServerPanelRpcServer::Resume() {
    // Enable the server
    this->bDisabled = false;
}

///////////////////////////////////////////////////////////////////////////////
/// Slots ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelRpcServer::DiscardClient() {
    // Open the socket
    QTcpSocket* cSocket = (QTcpSocket*) sender();
    // Schedule the socket for deletion
    cSocket->deleteLater();
}

void ServerPanelRpcServer::ReadClient() {
    // Check to see if the server is paused
    if (this->bDisabled) {
        // We're done
        return;
    }
    // Open the socket
    QTcpSocket* cSocket = (QTcpSocket*) sender();
    // Can we read the socket
    if (cSocket->canReadLine()) {
        // Grab the headers
        QStringList qslTokens = QString(cSocket->readLine().split(QRegExp("[ \r\n][ \r\n]*")));
        // What reqeust was sent
        if (qslTokens[0] == "GET") {         // GET
            // Create a new text stream for the socket
            QTextStream cSocketStream(cSocket);
            // Automagically detect the use of unicode
            cSocketStream.setAutoDetectUnicode(true);
            // Send a response
            cSocketStream << "HTTP/1.0 200 Ok\r\n";
            cSocketStream << "Content-Type: application/json; charset=\"utf-8\"\r\n";
            cSocketStream << "\r\n";
            cSocketStream << "{\"sServerResponse\":\"Good!\", \"sServerTime\":\"";
            cSocketStream << QDateTime::currentDateTime().toString() << "\"}\n";
        } else if (qslTokens[0] == "POST") { // POST

        } else {                             // Unknown/Unsupported

        }
    }
    // Close the socket
    cSocket->close();
    // Check the state of the socket
    if (cSocket->state() == QTcpSocket::UnconnectedState) {
        // Remove the socket
        delete cSocket;
    }
}
