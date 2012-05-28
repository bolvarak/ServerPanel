///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define RPC_LISTEN_PORT 1597

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "ServerPanelService.h"
#include <QtGui/QApplication>

///////////////////////////////////////////////////////////////////////////////
/// Main() ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int main (int iArguments, char** aArguments) {
    // Setup the application
    QApplication qanApplication(iArguments, aArguments);
    // Instantiate the server
    ServerPanelService::Instance();
    // Return the applications execution status
    qanApplication.exec();
}
