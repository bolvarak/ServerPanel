///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <ServerPanelClientMainWindow.h>
#include <ui_ServerPanelClientMainWindow.h>

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientMainWindow* ServerPanelClientMainWindow::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientMainWindow* ServerPanelClientMainWindow::Instance(QWidget* cParent) {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelClientMainWindow(cParent);
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientMainWindow::ServerPanelClientMainWindow(QWidget* cParent) : QMainWindow(cParent), mUserInterface(new Ui::ServerPanelClientMainWindow) {
    // Setup the user interface instance
    this->mUserInterface->setupUi(this);
    // Set the default display settings
    this->mUserInterface->btnAccountSettings->setText(ServerPanel::Instance()->GetAccount().getProperty("sUsername").toString());
    // Setup the graphics
    this->SetupGraphics();
    // Setup the connectors
    this->connect(this->mUserInterface->btnSignOut, SIGNAL(clicked()), this, SLOT(SignOutButtonClicked()));
    QMainWindow::connect(this->mUserInterface->btnGetDnsRecordCount, SIGNAL(clicked()), this, SLOT(GetDnsRecordsButtonClicked()));
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientMainWindow::~ServerPanelClientMainWindow() {
    // Delete the user interface
    delete this->mUserInterface;
    // Delete the instance
    delete this->mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Methods ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelClientMainWindow::SetupGraphics() {

}

///////////////////////////////////////////////////////////////////////////////
/// Protected Slots //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelClientMainWindow::GetDnsRecordsButtonClicked() {
    // Create a domain structure
    SpDnsRecord spDnsRecord;
    // Set the account ID
    spDnsRecord.saveProperty("iAccountId", ServerPanel::Instance()->GetAccount().getProperty("iAccountId").toInt());
    QVariantList qvlDnsRecords = ServerPanel::Instance()->LoadDnsRecords(spDnsRecord);
    ServerPanel::Instance()->DispatchMessageBox(qvlDnsRecords.at(0).toMap()["sHostName"].toString(), Notification);

}

/**
 * @paragraph This handles the logout routine
 * @brief ServerPanelClientMainWindow::SignOutButtonClicked
 * @param QString sLink
 * @return void
 */
void ServerPanelClientMainWindow::SignOutButtonClicked() {
    // Close this window
    this->close();
    // Show the login screen
    ServerPanelClientLoginWindow::Instance()->show();
}
