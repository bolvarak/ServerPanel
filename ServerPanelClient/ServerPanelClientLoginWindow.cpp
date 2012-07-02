///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <ServerPanelClientLoginWindow.h>

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ServerPanelClientLoginWindow* ServerPanelClientLoginWindow::mInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Singleton ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This is our singleton constructor, it ensures that the same instance of this window is always utilized
 * @brief ServerPanelClientLoginWindow::Instance()
 * @param QWidget cParent
 * @return ServerPanelClientLoginWindow
 */
ServerPanelClientLoginWindow* ServerPanelClientLoginWindow::Instance(QWidget* cParent) {
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

/**
 * @paragraph This is our base constructor, it makes all the method calls to setup the GUI
 * @brief ServerPanelClientWindow::ServerPanelClientLoginWindow()
 * @param QWidget cParent
 * @return void
 */
ServerPanelClientLoginWindow::ServerPanelClientLoginWindow(QWidget* cParent) : QDialog(cParent), mGrid(new QGridLayout) {
    // Set the window title
    this->setWindowTitle("ServerPanel - Login");
    // Set the window icon
    this->setWindowIcon(QIcon(":/icons/apps/gdmsetup.png"));
    // Setup the UI
    this->SetupUserInterface();
    // Set the layout
    this->setLayout(this->mGrid);
    // Show the window
    this->show();
}

///////////////////////////////////////////////////////////////////////////////
/// Destructor ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This is our base destructor, it deletes all of the setup objects from memory if need be.
 * @brief ServerPanelClientLoginWindow::~ServerPanelClientLoginWindow()
 * @return void
 */
ServerPanelClientLoginWindow::~ServerPanelClientLoginWindow() {
    // Delete the instance
    delete this->mInstance;
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Methods ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method reloads the lists of servers anytime something changes
 * @brief ServerPanelClientLoginWindow::ReloadServerList()
 * @return void
 */
void ServerPanelClientLoginWindow::ReloadServerList() {
    // Clear the server list
    this->mQlwServerList->clear();
    // Grab the servers from the local database
    QVariantList qvlLocalServers = ServerPanel::Instance()->GetLocalServers();
    // Loop through the servers
    foreach (QVariant qvServer, qvlLocalServers) {
        // Set the server name
        this->mQlwServerList->addItem(qvServer.toMap()["sAddress"].toString());
    }
}

/**
 * @paragraph This method sets up the user interface and draws all of the controls
 * @brief ServerPanelClientLoginWindow::SetupUserInterface()
 * @return void
 */
void ServerPanelClientLoginWindow::SetupUserInterface() {
    /**
     * QPushButton
     */
    // Create the add button
    this->mBtnAddServer                     = new QPushButton("");
    // Add the icon
    this->mBtnAddServer->setIcon(QIcon(":/icons/actions/add.png"));
    // Set the icon size
    this->mBtnAddServer->setIconSize(QSize(20, 20));
    // Set the size
    this->mBtnAddServer->setBaseSize   (25, 25);
    this->mBtnAddServer->setMaximumSize(25, 25);
    this->mBtnAddServer->setMinimumSize(25, 25);
    // Create the remove button
    this->mBtnRemoveServer                  = new QPushButton("");
    // Add the icon
    this->mBtnRemoveServer->setIcon(QIcon(":/icons/actions/remove.png"));
    // Set the icon size
    this->mBtnRemoveServer->setIconSize(QSize(20, 20));
    // Set the button size
    this->mBtnRemoveServer->setBaseSize(25, 25);
    this->mBtnRemoveServer->setMaximumSize(25, 25);
    this->mBtnRemoveServer->setMinimumSize(25, 25);
    // Create the server list button boxes
    QDialogButtonBox* qdbbServerList        = new QDialogButtonBox();
    // Add the buttons
    qdbbServerList->addButton(this->mBtnAddServer,    QDialogButtonBox::ActionRole);
    qdbbServerList->addButton(this->mBtnRemoveServer, QDialogButtonBox::ActionRole);
    // Add the elements to the grid
    this->mGrid->addWidget(qdbbServerList, 1, 0, 1, 1, Qt::AlignLeft);
    // Create a dialog button box for the server information
    QDialogButtonBox* qdbbServerInformation = new QDialogButtonBox();
    // Create the buttons for the server information
    this->mBtnSaveServer                    = new QPushButton(tr("Save"));    // Save
    this->mBtnCancel                        = new QPushButton(tr("Cancel"));  // Cancel
    this->mBtnSignIn                        = new QPushButton(tr("Sign-In")); // Sign-In
    // Set the button icon
    this->mBtnSaveServer->setIcon(QIcon(":/icons/devices/gnome-dev-harddisk.png")); // Save
    this->mBtnCancel->setIcon(QIcon(":/icons/actions/dialog-cancel.png"));          // Cancel
    this->mBtnSignIn->setIcon(QIcon(":/icons/apps/gnome-session-switch.png"));      // Sign-In
    // Set the button size
    this->mBtnSaveServer->setBaseSize   (100, 25); // Save
    this->mBtnSaveServer->setMaximumSize(100, 25); // Save
    this->mBtnSaveServer->setMinimumSize(100, 25); // Save
    this->mBtnCancel->setBaseSize       (100, 25); // Cancel
    this->mBtnCancel->setMaximumSize    (100, 25); // Cancel
    this->mBtnCancel->setMinimumSize    (100, 25); // Cancel
    this->mBtnSignIn->setBaseSize       (100, 25); // Sign-In
    this->mBtnSignIn->setMaximumSize    (100, 25); // Sign-In
    this->mBtnSignIn->setMinimumSize    (100, 25); // Sign-In
    // Set the shortcut
    this->mBtnSignIn->setShortcut(QKeySequence("Return"));
    // Add the buttons
    qdbbServerInformation->addButton(this->mBtnSaveServer, QDialogButtonBox::ActionRole); // Save
    qdbbServerInformation->addButton(this->mBtnCancel,     QDialogButtonBox::ActionRole); // Cancel
    qdbbServerInformation->addButton(this->mBtnSignIn,     QDialogButtonBox::ActionRole); // Sign-In
    /**
     * QLineEdit
     */
    // Create a widget for the server information
    QWidget* qwServerInformation            = new QWidget();
    // Create a grid for the server information
    QGridLayout* qglServerInformation       = new QGridLayout();
    // Create the server name/address box
    this->mTxtServerAddress             = new QLineEdit();
    // Create the server name/address label
    QLabel* qlblServerAddress               = new QLabel(tr("Server Address"));
    // Set the size
    this->mTxtServerAddress->setBaseSize   (250, 25);
    this->mTxtServerAddress->setMaximumSize(250, 25);
    this->mTxtServerAddress->setMinimumSize(250, 25);
    // Assign the line edit with the label
    qlblServerAddress->setBuddy(this->mTxtServerAddress);
    // Create the server port box
    this->mTxtServerPort                    = new QLineEdit(tr("1597"));
    // Create a server port label
    QLabel* qlblServerPort                  = new QLabel(tr("Port"));
    // Set the size
    this->mTxtServerPort->setBaseSize   (50, 25);
    this->mTxtServerPort->setMaximumSize(50, 25);
    this->mTxtServerPort->setMinimumSize(50, 25);
    // Assign the line edit with the label
    qlblServerPort->setBuddy(this->mTxtServerPort);
    // Create the username box
    this->mTxtUsername                      = new QLineEdit();
    // Create the username box label
    QLabel* qlblUsername                    = new QLabel(tr("Username"));
    // Set the size
    this->mTxtUsername->setBaseSize   (310, 25);
    this->mTxtUsername->setMaximumSize(310, 25);
    this->mTxtUsername->setMinimumSize(310, 25);
    // Assign the line edit with the label
    qlblUsername->setBuddy(this->mTxtUsername);
    // Create the password box
    this->mTxtPassword                      = new QLineEdit();
    // Create the password box label
    QLabel* qlblPassword                    = new QLabel(tr("Password"));
    // Set the size
    this->mTxtPassword->setBaseSize   (310, 25);
    this->mTxtPassword->setMaximumSize(310, 25);
    this->mTxtPassword->setMinimumSize(310, 25);
    // Set the echo mode
    this->mTxtPassword->setEchoMode(QLineEdit::Password);;
    // Assign the line edit with the label
    qlblPassword->setBuddy(this->mTxtPassword);
    // Add the labels to the grid
    qglServerInformation->addWidget(qlblServerAddress,       0, 0, 1, 1);
    qglServerInformation->addWidget(qlblServerPort,          0, 1, 1, 1);
    qglServerInformation->addWidget(qlblUsername,            2, 0, 1, 2);
    qglServerInformation->addWidget(qlblPassword,            4, 0, 1, 2);
    // Add the textboxes to the grid
    qglServerInformation->addWidget(this->mTxtServerAddress, 1, 0, 1, 1);
    qglServerInformation->addWidget(this->mTxtServerPort,    1, 1, 1, 1);
    qglServerInformation->addWidget(this->mTxtUsername,      3, 0, 1, 2);
    qglServerInformation->addWidget(this->mTxtPassword,      5, 0, 1, 2);
    // Add the button set to the main grid
    this->mGrid->addWidget(qdbbServerInformation, 1, 1, 1, 1, Qt::AlignLeft);
    // Add the grid to the widget
    qwServerInformation->setLayout(qglServerInformation);
    // Add the text boxes to the grid
    this->mGrid->addWidget(qwServerInformation,   0, 1, 1, 1, Qt::AlignTop);
    /**
     * QListWidget
     */
    // Create the list widget
    this->mQlwServerList                    = new QListWidget();
    // Set the list widget size
    this->mQlwServerList->setBaseSize   (150, 150);
    this->mQlwServerList->setMaximumSize(150, 150);
    this->mQlwServerList->setMinimumSize(150, 150);
    // Grab the servers from the local database
    QVariantList qvlLocalServers            = ServerPanel::Instance()->GetLocalServers();
    // Loop through the servers
    foreach (QVariant qvServer, qvlLocalServers) {
        // Set the server name
        this->mQlwServerList->addItem(qvServer.toMap()["sAddress"].toString());
    }
    // Add the widget to the grid
    this->mGrid->addWidget(this->mQlwServerList, 0, 0);
    /**
     * Connectors
     */
    this->connect(this->mBtnAddServer,    SIGNAL(clicked()),                       this, SLOT(AddServerButtonClicked()));
    this->connect(this->mBtnCancel,       SIGNAL(clicked()),                       this, SLOT(CancelButtonClicked()));
    this->connect(this->mBtnRemoveServer, SIGNAL(clicked()),                       this, SLOT(RemoveServerButtonClicked()));
    this->connect(this->mBtnSaveServer,   SIGNAL(clicked()),                       this, SLOT(SaveServerButtonClicked()));
    this->connect(this->mBtnSignIn,       SIGNAL(clicked()),                       this, SLOT(SignInButtonClicked()));
    this->connect(this->mQlwServerList,   SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(ServerSelected(QListWidgetItem*)));
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Slots //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method handles the clicking of the "+" button
 * @brief ServerPanelClientLoginWindow::AddServerButtonClicked()
 * @return void
 */
void ServerPanelClientLoginWindow::AddServerButtonClicked() {
    // Add a new item to the list
    this->mQlwServerList->addItem("New Server");
    // Clear the address field
    this->mTxtServerAddress->clear();
    // Clear the username field
    this->mTxtUsername->clear();
    // Clear the password field
    this->mTxtPassword->clear();
}

/**
 * @paragraph This method handles the clicking of the "Cancel" button
 * @brief ServerPanelClientLoginWindow::CancelButtonClicked()
 * @return void
 */
void ServerPanelClientLoginWindow::CancelButtonClicked() {
    // Close the application
    this->close();
}

/**
 * @paragraph This method handles the clicking of the "-" button
 * @brief ServerPanelClientLoginWindow::RemoveServerButtonClicked()
 * @return void
 */
void ServerPanelClientLoginWindow::RemoveServerButtonClicked() {
    // Try to delete the server
    if (ServerPanel::Instance()->DeleteLocalServer(this->mQlwServerList->currentItem()->text())) {
        // Reload the list
        this->ReloadServerList();
        // We're done
        return;
    }
    // Dispatch the message
    ServerPanel::Instance()->DispatchMessageBox("Unable to delete the server.", Error);
    // We're done
    return;
}

/**
 * @paragraph This method handles the clicking of the "Save" button
 * @brief ServerPanelClientLoginWindow::SaveServerButtonClicked
 * @return void
 */
void ServerPanelClientLoginWindow::SaveServerButtonClicked() {
    // Create the server structure
    SpLocalServer slsServer;
    // Set the server name
    slsServer.saveProperty("sName",    this->mTxtServerAddress->text());
    // Set the server address
    slsServer.saveProperty("sAddress", this->mTxtServerAddress->text());
    // Set the port
    slsServer.saveProperty("iPort",    this->mTxtServerPort->text().toInt());
    // Try to save the server
    if (ServerPanel::Instance()->SaveLocalServer(slsServer)) {
        // Setup the account
        SpLocalAccount slaAccount;
        // Set the username
        slaAccount.saveProperty("sUsername", this->mTxtUsername->text());
        // Set the password
        slaAccount.saveProperty("sPassword", this->mTxtPassword->text());
        // Set the server id
        slaAccount.saveProperty("iServerId", ServerPanel::Instance()->GetCurrentServer().getProperty("iServerId").toInt());
        // Try to save the account
        if (ServerPanel::Instance()->SaveLocalAccount(slaAccount)) {
            // Reload the server list
            this->ReloadServerList();
            // We're done
            return;
        }
        // Dispatch the message
        ServerPanel::Instance()->DispatchMessageBox("Unable to save the account.", Error);
        // We're done
        return;
    }
    // Dispatch the message
    ServerPanel::Instance()->DispatchMessageBox("Unable to save the server.", Error);
    // We're done
    return;
}

/**
 * @paragraph This method handles the activation of an item in the server list
 * @brief ServerPanelClientLoginWindow::ServerSelected()
 * @param QListWidgetItem qlwiSelected
 * @return void
 */
void ServerPanelClientLoginWindow::ServerSelected(QListWidgetItem* qlwiSelected){
    // Create a new account structure
    SpLocalAccount slaAccount;
    // Set the server address
    slaAccount.saveProperty("sAddress", qlwiSelected->text());
    // Try to load the account
    if (ServerPanel::Instance()->LoadLocalAccount(slaAccount)) {
        // Set the host address
        this->mTxtServerAddress->setText(qlwiSelected->text());
        // Set the port
        this->mTxtServerPort->setText("1597");
        // Set the username
        this->mTxtUsername->setText(ServerPanel::Instance()->GetCurrentAccount().getProperty("sUsername").toString());
        // Set the password
        this->mTxtPassword->setText(ServerPanel::Instance()->GetCurrentAccount().getProperty("sPassword").toString());
        // We're done
        return;
    }
    // Dispatch the message
    ServerPanel::Instance()->DispatchMessageBox("Unable to load server accound details.", Error);
    // We're done
    return;
}

/**
 * @paragraph This method handles the clicking of the "Sign-In"
 * @brief ServerPanelClientLoginWindow::SignInButtonClicked()
 * @return void
 */
void ServerPanelClientLoginWindow::SignInButtonClicked() {
    // Setup the account
    SpAccount spAccount;
    // Set the username
    spAccount.saveProperty("sUsername", ServerPanel::Instance()->GetCurrentAccount().getProperty("sUsername").toString());
    // Set the password
    spAccount.saveProperty("sPassword", ServerPanel::Instance()->GetCurrentAccount().getProperty("sPassword").toString());
    // Setup the server
    SpLocalServer slsServer;
    // Set the address
    slsServer.saveProperty("sAddress", this->mTxtServerAddress->text());
    // Setup the local account
    SpLocalAccount slaAccount;
    // Set the address
    slaAccount.saveProperty("sAddress", this->mTxtServerAddress->text());
    // Load the server
    if (!ServerPanel::Instance()->LoadLocalServer(slsServer)) {
        // Dispatch the message
        ServerPanel::Instance()->DispatchMessageBox("Unable to load server details.", Error);
        // We're done
        return;
    }
    // Load the local account
    if (!ServerPanel::Instance()->LoadLocalAccount(slaAccount)) {
        // Dispatch the message
        ServerPanel::Instance()->DispatchMessageBox("Unable to load local account details.", Error);
        // We're done
        return;
    }
    // Try to authenticate the user
    if (ServerPanel::Instance()->AuthenticateUser(spAccount)) {
        // Save the remote account
        ServerPanel::Instance()->SetRemoteAccount(ServerPanel::Instance()->GetResponse()["oAccount"].toMap());
        // Close the window
        this->hide();
        // Open the dashboard
        ServerPanelClientMainWindow::Instance()->show();
    }
    // We're done
    return;
}
