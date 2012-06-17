#ifndef SERVERPANEL_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANEL_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QtCrypto>
#include <QSettings>
#include <QtSql>
#include <QtNetwork/QtNetwork>
#include <QMessageBox>
#include <ServerPanelStructures.h>

///////////////////////////////////////////////////////////////////////////////
/// Enumerations /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

enum ServerPanelMessage {
    Error        = 0,
    Warning      = 1,
    Notification = 2,
    Success      = 3
};

///////////////////////////////////////////////////////////////////////////////
/// ServerPanel Class Definition /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ServerPanel : public QObject {
    // Ensure this class is an instance of QObject
    Q_OBJECT
// Public
public:
    // Singleton
    static ServerPanel* Instance(QObject* cParent = 0);
    // Constructor
    explicit ServerPanel(QObject* cParent = 0);
    // Destructor
    ~ServerPanel();
    // Methods
    bool           AuthenticateUser  (SpAccount spAccount);
    QVariantMap    DecryptEntity     (QByteArray qbaKey, QByteArray qbaVector, QByteArray qbaHash);
    bool           DeleteLocalServer (QString sAddress);
    int            DispatchMessageBox(QString sMessage, ServerPanelMessage spmType);
    QVariantMap    EncryptEntity     (QString sData);
    bool           IsOk              ();
    bool           LoadLocalAccount  (SpLocalAccount slaAccount);
    bool           LoadLocalServer   (SpLocalServer  slsServer);
    bool           SaveLocalAccount  (SpLocalAccount slaAccount);
    bool           SaveLocalServer   (SpLocalServer  slsServer);
    // Getters
    SpLocalAccount GetCurrentAccount ();
    SpLocalServer  GetCurrentServer  ();
    QVariantList   GetLocalServers   ();
    QString        GetError          ();
    QVariantMap    GetResponse       ();
// Protected
protected:
    // Properties
    QTcpSocket          mClient;
    QSettings*          mConfig;
    SpLocalAccount      mCurrentAccount;
    SpLocalServer       mCurrentServer;
    QSqlDatabase        mDbc;
    QString             mError;
    static ServerPanel* mInstance;
    QVariantList        mLocalServers;
    bool                mOk;
    QVariantMap         mRequest;
    QVariantMap         mResponse;
    // Methods
    bool           MakeRequest       (QString sMethod, QVariantMap qvmRequestData);
// Protected Slots
protected slots:
    void           SocketError       (QAbstractSocket::SocketError qseError);
    void           TransferData      ();
};

#endif // SERVERPANEL_H
