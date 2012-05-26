///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include "ServerPanelRpc.h"
#include "ServerPanelMainWindow.h"
#include "ServerPanelAuthenticatedWindow.h"

///////////////////////////////////////////////////////////////////////////////
/// Main() ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int main(int iArguments, char *aArguments[]) {
    // Create a new application
    QApplication appMain(iArguments, aArguments);
    // Show the login window
    ServerPanelMainWindow::Instance()->show();
    // Show the authenticated window
    // ServerPanelAuthenticatedWindow::Instance()->show();
    // We're done.
    return appMain.exec();
}
