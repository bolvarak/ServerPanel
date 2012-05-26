///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

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
    // Initialize the cipher
    QCA::Initializer          cCrypto    = QCA::Initializer();
    // Generate a key
    QCA::SymmetricKey         sKey       = QCA::SymmetricKey(2048);
    // Create the initialization vector
    QCA::InitializationVector vCrypto    = QCA::InitializationVector(2048);
    // Generate a cipher
    QCA::Cipher               cCipher    = QCA::Cipher(QString("aes256"), QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Encode, sKey, vCrypto);
    // Create the data array
    QCA::SecureArray          cData      = this->mUserInterface->txtPassword->text().toAscii();
    // Encrypt the data
    QCA::SecureArray          cEncrypted = cCipher.process(cData);
    // Create a map to store the request in
    QVariantMap mapJson;
    // Add the username
    mapJson.insert("sUsername", this->mUserInterface->txtUsername->text());
    // Add the password
    mapJson.insert("sPassword", cEncrypted.toByteArray());
    // Now serialize the JSON
    QByteArray sJson = QtJson::Json::serialize(mapJson);
    // Show a message box with the XML
    QMessageBox::information(this, "JSON", sJson);
}
