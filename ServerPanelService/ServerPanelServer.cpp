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

ServerPanelServer::ServerPanelServer(QObject* cParent) : QThread(cParent) {}

///////////////////////////////////////////////////////////////////////////////
/// Public Methods ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelServer::run() {
    // Create the socket
    QTcpSocket* qtsSocket = new QTcpSocket(this);
    // Setup the client read event handler
    connect(qtsSocket, SIGNAL(readyRead()), this, SLOT(HandleClient()));
    // Setup the client disconnect handler
    connect(qtsSocket, SIGNAL(disconnected()), this, SLOT(DisconnectClient()));
    // Setup the completed event handler
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    // Try to set the socket descriptor
    if (!qtsSocket->setSocketDescriptor(this->mSocket)) {
        // Throw an error
        emit this->Error(qtsSocket->error());
        // We're done
        return;
    }
    // Close the socket
    qtsSocket->close();
}

///////////////////////////////////////////////////////////////////////////////
/// Signals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
/// Slots ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelServer::DisconnectClient() {
    // Open the socket
    QTcpSocket* qtsSocket = (QTcpSocket*) sender();
    // Schedule the socket for deletion
    qtsSocket->deleteLater();
}

void ServerPanelServer::HandleClient() {
    // Open the socket
    QTcpSocket* qtsSocket = (QTcpSocket*) sender();
    // Check to see if we can read from the client
    if (qtsSocket->canReadLine()) {
        // Grab the line
        QString     sLine      = qtsSocket->readLine();
        // Grab the headers
        QStringList qslHeaders = sLine.split(QRegExp("[ \r\n][ \t\n]*"));
        // Loop through the headers and log them
        for (int iHeader = 0; iHeader < (int) qslHeaders.size(); iHeader++) {
            // Log the header
            qDebug(qslHeaders[iHeader].toLatin1());
        }
        // Create a buffer
        QByteArray qbaBuffer;
        // Send the response code
        qbaBuffer.append("HTTP/1.1 200 OK\r\n");
        qbaBuffer.append("Content-Type: application/json; charset=\"utf-8\"\r\n");
        // Newline
         qbaBuffer.append("\r\n");
        // Send some JSON
        qbaBuffer.append("{\n");
        qbaBuffer.append("\t\"sServerStatus\":\"Good!\",\n");
        qbaBuffer.append("\t\"sServerTime\":\"");
        qbaBuffer.append(QDateTime::currentDateTime().toString().toLatin1());
        qbaBuffer.append("\"\n}\n");
        // Write the data
        qtsSocket->write(qbaBuffer);
        // Close the socket
        qtsSocket->close();
    }
    // Close the socket
    qtsSocket->disconnectFromHost();
    // Check the state of the socket
    if (qtsSocket->state() != QTcpSocket::UnconnectedState) {
        // Wait for the client to disconnect
        qtsSocket->waitForDisconnected();
    } else {
        // Delete the socket
        delete qtsSocket;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Setters //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelServer::SetSocketDescriptor(int iSocket) {
    // Set the socket descriptor into the system
    this->mSocket = iSocket;
}
