///////////////////////////////////////////////////////////////////////////////
/// Headers //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <ServerPanelConfig.h>

///////////////////////////////////////////////////////////////////////////////
/// Globals //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

QSettings ServerPanelConfig::mConfig(":/ServerPanel.ini", QSettings::IniFormat);

///////////////////////////////////////////////////////////////////////////////
/// Constructor //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This is our constructor, it does nothing
 * @brief ServerPanelConfig::ServerPanelConfig()
 * @return void
 */
ServerPanelConfig::ServerPanelConfig(QObject* cParent) : QObject(cParent) {}

///////////////////////////////////////////////////////////////////////////////
/// Public Static Methods ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * @paragraph This method grabs a property or section from the configuration file
 * @brief ServerPanelConfig::Get()
 * @param QString sProperty
 * @return QVariant
 */
QVariant ServerPanelConfig::Get(QString sProperty) {
    // Look for a path separator
    if (sProperty.contains(".")) {
        // Return the property
        return mConfig.value(sProperty.replace(".", "/"));
    }
    // Return the property or section
    return mConfig.value(sProperty);
}

/**
 * @paragraph This method sets the value of a property or creates the new property with a value
 * @brief ServerPanelConfig::Set()
 * @param QString sProperty
 * @param QVariant mValue
 * @return bool
 */
bool ServerPanelConfig::Set(QString sProperty, QVariant mValue) {
    // Check for a path separator
    if (sProperty.contains(".")) {
        // Set the property and return
        return mConfig.setProperty(sProperty.replace(".", "/").toLatin1(), mValue);
    } else {
        // Set the property and return
        return mConfig.setProperty(sProperty.toLatin1(), mValue);
    }
}
