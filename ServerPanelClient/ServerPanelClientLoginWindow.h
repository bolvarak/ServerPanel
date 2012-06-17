#ifndef SERVERPANELCLIENTLOGINWINDOW_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELCLIENTLOGINWINDOW_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QListWidget>
#include <QMainWindow>
#include <QtCrypto/QtCrypto>
#include <ServerPanel.h>
#include <ServerPanelClientMainWindow.h>

///////////////////////////////////////////////////////////////////////////////
/// Namespace ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

namespace Ui {
    // Set the window class into the namespace
    class ServerPanelClientLoginWindow;
}

///////////////////////////////////////////////////////////////////////////////
/// ServerPanelClientLoginWindow Class Definition ////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanelClientLoginWindow : public QMainWindow {
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
    // Singleton Instance
    static ServerPanelClientLoginWindow* mInstance;
    // User Interface Instance
    Ui::ServerPanelClientLoginWindow*    mUserInterface;
// Protected slots
protected slots:
    void     AddServerButtonClick                  ();
    void     CancelButtonClick                     ();
    void     LoadLocalServerData                   (QListWidgetItem* qlwiServer);
    void     LoginButtonClick                      ();
    void     RemoveServerButtonClick               ();
    void     SaveButtonClick                       ();
};

#endif // SERVERPANELCLIENTLOGINWINDOW_H
