///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <ServerPanelClientLoginWindow.h>
#include <ui_ServerPanelClientLoginWindow.h>

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientLoginWindow* ServerPanelClientLoginWindow::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientLoginWindow* ServerPanelClientLoginWindow::Instance(QWidget *cParent) {
    // Check for an existing instance
    if (!mInstance) {
        // Create a new instance
        mInstance = new ServerPanelClientLoginWindow(cParent);
    }
    // Return the instance
    return mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientLoginWindow::ServerPanelClientLoginWindow(QWidget* cParent) : QMainWindow(cParent), mUserInterface(new Ui::ServerPanelClientLoginWindow) {
    // Setup the user interface
    this->mUserInterface->setupUi(this);
    // Setup the connector slots
    connect(this->mUserInterface->btnAddServer,    SIGNAL(clicked()),                       this, SLOT(AddServerButtonClick()));    // Add Server
    connect(this->mUserInterface->btnCancel,       SIGNAL(clicked()),                       this, SLOT(CancelButtonClick()));       // Cancel
    connect(this->mUserInterface->btnLogin,        SIGNAL(clicked()),                       this, SLOT(LoginButtonClick()));        // Sign-In
    connect(this->mUserInterface->btnRemoveServer, SIGNAL(clicked()),                       this, SLOT(RemoveServerButtonClick())); // Remove Server
    connect(this->mUserInterface->btnSave,         SIGNAL(clicked()),                       this, SLOT(SaveButtonClick()));         // Save Server
    connect(this->mUserInterface->listServers,     SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(LoadLocalServerData(QListWidgetItem*)));     // Item Activated
    // Load all of the local servers
    QVariantList qvlLocalServers = ServerPanel::Instance()->GetLocalServers();
    // Make sure the list view is clear
    this->mUserInterface->listServers->clear();
    // Check for servers
    if (qvlLocalServers.size()) {
        // Loop through the servers
        for (int iServer = 0; iServer < qvlLocalServers.size(); ++iServer) {
            // Create a new QListWidgetItem
            QListWidgetItem* qlwiServer = new QListWidgetItem();
            // Set the item data
            qlwiServer->setText(qvlLocalServers[iServer].toMap()["sAddress"].toString());
            // Add the item
            this->mUserInterface->listServers->addItem(qlwiServer);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientLoginWindow::~ServerPanelClientLoginWindow() {
    // Delete the user interface
    delete this->mUserInterface;
    // Delete the instance
    delete this->mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Slots //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void ServerPanelClientLoginWindow::AddServerButtonClick() {

}

/**
 * @paragraph This method causes the application to exit once the cancel button is clicked
 * @brief ServerPanelClientLoginWindow::CancelButtonClick
 * @return void;
 */
void ServerPanelClientLoginWindow::CancelButtonClick() {
    // Close the application
    this->close();
}

/**
 * @paragraph This method loads the account associated with the currently selected server
 * @brief ServerPanelClientLoginWindow::LoadLocalServerData
 * @return void
 */
void ServerPanelClientLoginWindow::LoadLocalServerData(QListWidgetItem* qlwiServer) {
    // Setup the local account structure
    SpLocalAccount slaAccount;
    // Set the address
    slaAccount.saveProperty("sAddress", qlwiServer->text());
    // Try to load the server data
    if (ServerPanel::Instance()->LoadLocalAccount(slaAccount)) {
        // Setup the form
        this->mUserInterface->txtServerAddress->setText(qlwiServer->text());                                                          // Server Address
        this->mUserInterface->txtUsername->setText(ServerPanel::Instance()->GetCurrentAccount().getProperty("sUsername").toString()); // Username
        this->mUserInterface->txtPassword->insert(ServerPanel::Instance()->GetCurrentAccount().getProperty("sPassword").toString()); // Password
    }
}

void ServerPanelClientLoginWindow::LoginButtonClick() {
    // Setup the account structure
    SpAccount spAccount;
    // Set the username
    spAccount.saveProperty("sUsername", this->mUserInterface->txtUsername->text());
    // Set the password
    spAccount.saveProperty("sPassword", this->mUserInterface->txtPassword->text());
    // Setup the server
    SpLocalServer slsServer;
    // Set the server address
    slsServer.saveProperty("sAddress", this->mUserInterface->txtServerAddress->text());
    // Load the server
    ServerPanel::Instance()->LoadLocalServer(slsServer);
    // Try to authenticate the user
    if (ServerPanel::Instance()->AuthenticateUser(spAccount)) {
        // Dispatch a message
        ServerPanel::Instance()->DispatchMessageBox("Account Authenticated!", Success);
    }
}

/**
 * @paragraph This method deletes a server from the local database when the minus button is clicked
 * @brief ServerPanelClientLoginWindow::RemoveServerButtonClick
 */
void ServerPanelClientLoginWindow::RemoveServerButtonClick() {
    // Remove the server
    ServerPanel::Instance()->DeleteLocalServer(this->mUserInterface->listServers->currentItem()->text());
    // Load all of the local servers
    QVariantList qvlLocalServers = ServerPanel::Instance()->GetLocalServers();
    // Make sure the list view is clear
    this->mUserInterface->listServers->clear();
    // Check for servers
    if (qvlLocalServers.size()) {
        // Loop through the servers
        for (int iServer = 0; iServer < qvlLocalServers.size(); ++iServer) {
            // Create a new QListWidgetItem
            QListWidgetItem* qlwiServer = new QListWidgetItem();
            // Set the item data
            qlwiServer->setText(qvlLocalServers[iServer].toMap()["sAddress"].toString());
            // Add the item
            this->mUserInterface->listServers->addItem(qlwiServer);
        }
    }
}

/**
 * @paragraph This method saves a local server and account when the "Save" button is clicked
 * @brief ServerPanelClientLoginWindow::SaveButtonClick
 * @return void
 */
void ServerPanelClientLoginWindow::SaveButtonClick() {
    // Create a local server structure
    SpLocalServer slsLocalServer;
    // Set the server address
    slsLocalServer.saveProperty("sAddress", this->mUserInterface->txtServerAddress->text());
    // Set the server name
    slsLocalServer.saveProperty("sName",    this->mUserInterface->txtServerAddress->text());
    // Check to see if the server exists
    if (ServerPanel::Instance()->LoadLocalServer(slsLocalServer)) {
        // Set the server id
        slsLocalServer.saveProperty("iServerId", ServerPanel::Instance()->GetCurrentServer().getProperty("iServerId").toInt());
    }
    // Save the server
    ServerPanel::Instance()->SaveLocalServer(slsLocalServer);
    // Set the local server id
    slsLocalServer.saveProperty("iServerId", ServerPanel::Instance()->GetCurrentServer().getProperty("iServerId").toInt());
    // Create a local account structure
    SpLocalAccount slaLocalAccount;
    // Set the username
    slaLocalAccount.saveProperty("sUsername", this->mUserInterface->txtUsername->text());
    // Set the password
    slaLocalAccount.saveProperty("sPassword", this->mUserInterface->txtPassword->text().toLatin1());
    // Set the server id
    slaLocalAccount.saveProperty("iServerId", slsLocalServer.getProperty("iServerId").toInt());
    // Check to see if the account exists
    if (ServerPanel::Instance()->LoadLocalAccount(slaLocalAccount)) {
        // Set the account id
        slaLocalAccount.saveProperty("iAccountId", ServerPanel::Instance()->GetCurrentAccount().getProperty("iAccountId").toInt());
    }
    // Save the account
    ServerPanel::Instance()->SaveLocalAccount(slaLocalAccount);
    // Load all of the local servers
    QVariantList qvlLocalServers = ServerPanel::Instance()->GetLocalServers();
    // Make sure the list view is clear
    this->mUserInterface->listServers->clear();
    // Check for servers
    if (qvlLocalServers.size()) {
        // Loop through the servers
        for (int iServer = 0; iServer < qvlLocalServers.size(); ++iServer) {
            // Create a new QListWidgetItem
            QListWidgetItem* qlwiServer = new QListWidgetItem();
            // Set the item data
            qlwiServer->setText(qvlLocalServers[iServer].toMap()["sAddress"].toString());
            // Add the item
            this->mUserInterface->listServers->addItem(qlwiServer);
        }
    }
}
