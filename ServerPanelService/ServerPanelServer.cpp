///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "ServerPanelServer.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelServer* ServerPanelServer::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelServer* ServerPanelServer::Instance() {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelServer();
    }
    // Return the Instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelServer::ServerPanelServer(QObject* cParent) : QTcpServer(cParent), mDisabled(false) {
    // Set the address and port to listen on
    listen(QHostAddress::Any, RPC_PORT);
}

///////////////////////////////////////////////////////////////////////////////
/// Public Methods ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelServer::incomingConnection(int iSocket) {
    // Check to see if the server has been paused
    if (this->mDisabled) {
        // We're done
        return;
    }
    // Create the socket
    QTcpSocket* cSocket = new QTcpSocket(this);
    // Setup the event handler to read the client data
    connect(cSocket, SIGNAL(readyRead()), this, SLOT(ReadClient()));
    // Setup the event handler to close the connection
    connect(cSocket, SIGNAL(disconnected()), this, SLOT(DiscardClient()));
    // Set the socket descriptor
    cSocket->setSocketDescriptor(iSocket);
}

void ServerPanelServer::Pause() {
    // Pause the server
    this->mDisabled = true;
}

void ServerPanelServer::Resume() {
    // Resume server operations
    this->mDisabled = false;
}

///////////////////////////////////////////////////////////////////////////////
/// Slots ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelServer::ReadClient() {
    // Check to see if the server has been paused
    if (this->mDisabled) {
        // We're done
        return;
    }
    // Open the socket
    QTcpSocket* cSocket = (QTcpSocket*) sender();
    // Check to see if we have access to the socket
    if (cSocket->readLine()) {
        // Grab the headers
        QStringList qslHeaders = QString(cSocket->readLine().split(QRegExp("[ \r\n][ \r\n]*")));
        // Determine the request method
        if (qslHeaders[0] == "GET") {         // GET
            // Create a stream to write to the socket
            QTextStream cClientStream(cSocket);
            // Tell the stream to autodetect unicode
            cClientStream.autoDetectUnicode(true);
            // Send the response code
            cClientStream << "HTTP/1.1 200 OK\r\n";
            // Send the content type
            cClientStream << "Content-Type: application/json; charset=\"utf-8\"\r\n";
            // Newline
            cClientStream << "\r\n";
            // Send the JSON response
            cClientStream << "{\n\t\"sServerStatus\":\"Good!\", \n";
            cClientStream << "\t\"sServerTime\":\"";
            cClientStream << QDateTime::currentDateTime().toString();
            cClientStream << "\"\n}\n";
        } else if (qslHeaders[0] == "POST") { // POST

        } else {                              // Unknown | Unsupported

        }
    }
    // Close the socket
    cSocket->close();
    // Check the state of the socket
    if (cSocket->state() == QTcpSocket::UnconnectedState) {
        // Delete the socket from memory
        delete cSocket;
    }
}

void ServerPanelServer::DiscardClient() {
    // Open the socket
    QTcpSocket* cSocket = (QTcpSocket*) sender();
    // Schedule the socket for deletion
    cSocket->deleteLater();
}
