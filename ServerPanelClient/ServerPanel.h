#ifndef SERVERPANEL_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANEL_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QtCrypto/QtCrypto>
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
    bool           AuthenticateUser  (SpAccount      spAccount);
    QVariantMap    DecryptEntity     (QByteArray     qbaKey,   QByteArray         qbaVector, QByteArray qbaHash);
    bool           DeleteLocalServer (QString        sAddress);
    int            DispatchMessageBox(QString        sMessage, ServerPanelMessage spmType);
    QVariantMap    EncryptEntity     (QString        sData);
    bool           IsOk              ();
    QVariantList   LoadDnsRecords    (SpDnsRecord    spDnsRecord);
    QVariantList   LoadDomains       (SpDomain       spDomain);
    bool           LoadLocalAccount  (SpLocalAccount slaAccount);
    bool           LoadLocalServer   (SpLocalServer  slsServer);
    bool           SaveLocalAccount  (SpLocalAccount slaAccount);
    bool           SaveLocalServer   (SpLocalServer  slsServer);
    // Getters
    SpLocalAccount GetCurrentAccount ();
    SpLocalServer  GetCurrentServer  ();
    QVariantList   GetLocalServers   ();
    QString        GetError          ();
    SpAccount      GetAccount        ();
    QVariantMap    GetResponse       ();
// Protected
protected:
    // Properties
    quint16             mBlockSize;
    QTcpSocket*         mClient;
    QSettings*          mConfig;
    SpLocalAccount      mCurrentAccount;
    SpLocalServer       mCurrentServer;
    SpAccount           mCurrentRemoteAccount;
    QSqlDatabase        mDbc;
    QString             mError;
    static ServerPanel* mInstance;
    QString             mJsonResponse;
    QVariantList        mLocalServers;
    bool                mOk;
    QVariantMap         mRequest;
    QVariantMap         mResponse;
    // Methods
    bool           MakeRequest       (QString sMethod, QVariantMap qvmRequestData);
    void           ProcessResponse   ();
// Protected Slots
protected slots:
    void           ReadResponse      ();
    void           SocketError       (QAbstractSocket::SocketError qseError);
    void           TransferData      ();
};

#endif // SERVERPANEL_H
