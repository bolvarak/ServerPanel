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
    // Set the grid size
    this->mGrid->geometry().setSize(QSize(500, 500));
    // Setup the lists
    this->SetupLists();
    // Setup text boxes
    this->SetupLineEdits();
    // Setup buttons
    this->SetupButtons();
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
 * @paragraph This method sets up all of the buttons needed for this window
 * @brief ServerPanelClientLoginWindow::SetupButtons()
 * @return void
 */
void ServerPanelClientLoginWindow::SetupButtons() {
    // Create the add button
    QPushButton* qpbAddServer = new QPushButton("");
    // Add the icon
    qpbAddServer->setIcon(QIcon(":/icons/actions/add.png"));
    // Set the icon size
    qpbAddServer->setIconSize(QSize(20, 20));
    // Set the size
    qpbAddServer->setBaseSize(25, 25);
    qpbAddServer->setMaximumSize(25, 25);
    qpbAddServer->setMinimumSize(25, 25);
    // Create the remove button
    QPushButton* qpbRemoveServer = new QPushButton("");
    // Add the icon
    qpbRemoveServer->setIcon(QIcon(":/icons/actions/remove.png"));
    // Set the icon size
    qpbRemoveServer->setIconSize(QSize(20, 20));
    // Set the button size
    qpbRemoveServer->setBaseSize(25, 25);
    qpbRemoveServer->setMaximumSize(25, 25);
    qpbRemoveServer->setMinimumSize(25, 25);
    // Create the server list button boxes
    QDialogButtonBox* qdbbServerList = new QDialogButtonBox();
    // Add the buttons
    qdbbServerList->addButton(qpbAddServer,    QDialogButtonBox::ActionRole);
    qdbbServerList->addButton(qpbRemoveServer, QDialogButtonBox::ActionRole);
    // Add the elements to the grid
    this->mGrid->addWidget(qdbbServerList, 310, 0, 1, 1, Qt::AlignRight);
}

/**
 * @paragraph This method sets up all of the QLineEdit boxes this window needs
 * @brief ServerPanelClientLoginWindow::SetupLineEdits()
 * @return void
 */
void ServerPanelClientLoginWindow::SetupLineEdits() {
    // Create a widget for the server information
    QWidget* qwServerInformation      = new QWidget();
    // Create a grid for the server information
    QGridLayout* qglServerInformation = new QGridLayout();
    // Create the server name/address box
    QLineEdit* qleServerAddress       = new QLineEdit();
    // Create the server name/address label
    QLabel* qlblServerAddress         = new QLabel(tr("Server Address"));
    // Set the size
    qleServerAddress->setBaseSize   (250, 25);
    qleServerAddress->setMaximumSize(250, 25);
    qleServerAddress->setMinimumSize(250, 25);
    // Assign the line edit with the label
    qlblServerAddress->setBuddy(qleServerAddress);
    // Create the server port box
    QLineEdit* qleServerPort          = new QLineEdit(tr("1597"));
    // Create a server port label
    QLabel* qlblServerPort            = new QLabel(tr("Port"));
    // Set the size
    qleServerPort->setBaseSize   (50, 25);
    qleServerPort->setMaximumSize(50, 25);
    qleServerPort->setMinimumSize(50, 25);
    // Assign the line edit with the label
    qlblServerPort->setBuddy(qleServerPort);
    // Add the labels to the grid
    qglServerInformation->addWidget(qlblServerAddress, 0, 0, 1, 1);
    qglServerInformation->addWidget(qlblServerPort,    0, 1, 1, 1);
    // Add the textboxes to the grid
    qglServerInformation->addWidget(qleServerAddress,  1, 0, 1, 1);
    qglServerInformation->addWidget(qleServerPort,     1, 1, 1, 1);
    // Add the grid to the widget
    qwServerInformation->setLayout(qglServerInformation);
    // Add the text boxes to the grid
    this->mGrid->addWidget(qwServerInformation, 0, 160, 1, 1, Qt::AlignTop);
}

/**
 * @paragraph This method sets up the lists for this window
 * @brief ServerPanelClientLoginWindow::SetupLists()
 * @return void
 */
void ServerPanelClientLoginWindow::SetupLists() {
    // Create the list widget
    QListWidget* qlwServers = new QListWidget();
    // Set the list widget size
    qlwServers->setBaseSize   (150, 300);
    qlwServers->setMaximumSize(150, 300);
    qlwServers->setMinimumSize(150, 300);
    // Grab the servers from the local database
    QVariantList qvlLocalServers = ServerPanel::Instance()->GetLocalServers();
    // Loop through the servers
    foreach (QVariant qvServer, qvlLocalServers) {
        // Set the server name
        qlwServers->addItem(qvServer.toMap()["sAddress"].toString());
    }
    // Add the widget to the grid
    this->mGrid->addWidget(qlwServers, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
/// Protected Slots //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


