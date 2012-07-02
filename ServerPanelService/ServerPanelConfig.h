#ifndef SERVERPANELCONFIG_H

///////////////////////////////////////////////////////////////////////////////
/// Definitions //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define SERVERPANELCONFIG_H

///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <QObject>
#include <QSettings>

///////////////////////////////////////////////////////////////////////////////
/// ServerPanelConfig Class Definition ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/**
 * @paragraph This class handles all of the talking between ServerPanel and the configuration file
 * @brief ServerPanelConfig()
 */
class ServerPanelConfig : public QObject {
    // Make sure this class is recognized as a QObject
    Q_OBJECT
// Public
public:
    // Constructor
    explicit ServerPanelConfig(QObject* cParent = 0);
    // Methods
    static QVariant Get(QString sProperty);
    static bool     Set(QString sName, QVariant mValue);
// Protected
protected:
    // Properties
    static QSettings mConfig;
};
#endif // SERVERPANELCONFIG_H
