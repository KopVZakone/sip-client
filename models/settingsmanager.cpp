#include "settingsmanager.h"
#include <QtSql/QSqlQuery>
#include "audiomanager.h"
SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent}
{ }

SettingsManager &SettingsManager::instance()
{
    static SettingsManager inst;
    return inst;
}

QStringList SettingsManager::inputDevices() const
{
    return AudioManager::instance().inputDevices();
}

QStringList SettingsManager::outputDevices() const
{
    return AudioManager::instance().outputDevices();
}

void SettingsManager::setInputDeviceByName(QString deviceName)
{
    AudioManager::instance().setDeviceByName(deviceName, true);
    setVal("input_name", deviceName);
}

void SettingsManager::setOutputDeviceByName(QString deviceName)
{
    AudioManager::instance().setDeviceByName(deviceName, false);
    setVal("output_name", deviceName);
}

void SettingsManager::refreshDeviceLists()
{
    AudioManager::instance().refreshDeviceLists();
}

void SettingsManager::setVal(QString key, QVariant value)
{
    QSqlQuery q;
    q.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (?, ?)");
    q.addBindValue(key);
    q.addBindValue(value.toString());
    q.exec();
}

QVariant SettingsManager::getVal(QString key, QVariant defaultValue) const
{
    QSqlQuery q;
    q.prepare("SELECT value FROM settings WHERE key = ?");
    q.addBindValue(key);
    if (q.exec() && q.next())
        return q.value(0);
    return defaultValue;
}
