#ifndef SERVERPANELCLIENTLOGINWINDOW_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELCLIENTLOGINWINDOW_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>
#include <QGridLayout>
#include <QtCrypto>
#include <ServerPanel.h>
#include <ServerPanelClientMainWindow.h>
#include <QJsonView.h>

///////////////////////////////////////////////////////////////////////////////
/// Namespace ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
/// ServerPanelClientLoginWindow Class Definition ////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanelClientLoginWindow : public QDialog {
    // Make sure this is an instance of a QObject
    Q_OBJECT
// Public
public:
    // Singleton
    static   ServerPanelClientLoginWindow* Instance(QWidget* cParent = 0);
    // Constructor
    explicit ServerPanelClientLoginWindow          (QWidget* cParent = 0);
    // Destructor
            ~ServerPanelClientLoginWindow          ();
// Protected
protected:
    // Properties
    static ServerPanelClientLoginWindow* mInstance;
    QGridLayout*                         mGrid;
    // Methods
    void SetupUserInterface                        ();
    // QPushButton
    QPushButton*                         mBtnAddServer;
    QPushButton*                         mBtnCancel;
    QPushButton*                         mBtnRemoveServer;
    QPushButton*                         mBtnSaveServer;
    QPushButton*                         mBtnSignIn;
    // QLineEdit
    QLineEdit*                           mTxtPassword;
    QLineEdit*                           mTxtServerAddress;
    QLineEdit*                           mTxtServerPort;
    QLineEdit*                           mTxtUsername;
    // QListWidget
    QListWidget*                         mQlwServerList;
    // Methods
    void ReloadServerList         ();
// Protected slots
protected slots:
    // Button Clicks
    void AddServerButtonClicked   ();
    void CancelButtonClicked      ();
    void RemoveServerButtonClicked();
    void SaveServerButtonClicked  ();
    void ServerSelected           (QListWidgetItem* qlwiSelected);
    void SignInButtonClicked      ();
};

#endif // SERVERPANELCLIENTLOGINWINDOW_H
