///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <QBuffer>
#include <QXmlStreamWriter>
#include <QtGui/QMessageBox>
#include "ServerPanelMainWindow.h"
#include "ui_ServerPanelMainWindow.h"

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelMainWindow* ServerPanelMainWindow::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelMainWindow* ServerPanelMainWindow::Instance() {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelMainWindow();
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelMainWindow::ServerPanelMainWindow(QWidget *cParent) : QWidget(cParent), mUserInterface(new Ui::ServerPanelMainWindow) {
    // Setup the user interface object
    this->mUserInterface->setupUi(this);
    // Setup a Sign-In button click handler
    connect(this->mUserInterface->btnSignIn, SIGNAL(clicked()), this, SLOT(TrySignIn()));
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelMainWindow::~ServerPanelMainWindow() {
    // Delete the user interface object
    delete this->mUserInterface;
}

///////////////////////////////////////////////////////////////////////////////
/// Event Handlers ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelMainWindow::TrySignIn () {
    // Create an error place holder
    QString sErrorText;
    // Check for valid data
    if (this->mUserInterface->txtUsername->text().isEmpty()) {
        // Add to the error string
        sErrorText += "<font color=\"red\">Missing username.</font>\n";
    }
    if (this->mUserInterface->txtPassword->text().isEmpty()) {
        // Add to the error string
        sErrorText += "<font color=\"red\">Missing password.</font>\n";
    }
    // Do we have errors
    if (!sErrorText.isEmpty()) {
        // Show the message box
        QMessageBox::information(this, "Error!", sErrorText);
    }
    // Set a string to write the XML to
    QString sXml;
    // Setup the XmlStreamWriter
    QXmlStreamWriter xswData(&sXml);
    // Turn auto formatting on
    xswData.setAutoFormatting(true);
    // Start the document
    xswData.writeStartDocument();
    // Start the data element
    xswData.writeStartElement("oData");
    // Add the Username open tag
    xswData.writeStartElement("sUsername");
    // Add the username
    xswData.writeCharacters( this->mUserInterface->txtUsername->text());
    // Close the username element
    xswData.writeEndElement();
    // Add the password open tag
    xswData.writeStartElement("sPassword");
    // Add the password
    xswData.writeCharacters(this->mUserInterface->txtPassword->text());
    // Close the password element
    xswData.writeEndElement();
    // Close the data element
    xswData.writeEndElement();
    // Close the document
    xswData.writeEndDocument();
    // Show a message box with the XML
    QMessageBox::information(this, "XML", sXml);
}
