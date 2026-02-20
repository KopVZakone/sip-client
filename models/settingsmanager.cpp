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
    setVal(KeyInputDevice, deviceName);
}

void SettingsManager::setOutputDeviceByName(QString deviceName)
{
    AudioManager::instance().setDeviceByName(deviceName, false);
    setVal(KeyOutputDevice, deviceName);
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

int SettingsManager::inputVolume() const
{
    return getVal(KeyInputVolume, 100).toInt();
}

int SettingsManager::outputVolume() const
{
    return getVal(KeyOutputVolume, 100).toInt();
}

bool SettingsManager::inputMuted() const
{
    return getVal(KeyInputMuted, false).toBool();
}

bool SettingsManager::outputMuted() const
{
    return getVal(KeyOutputMuted, false).toBool();
}

void SettingsManager::setInputVolume(int level)
{
    AudioManager::instance().setInputVolume(level);
    setVal(KeyInputVolume, level);
    emit inputVolumeChanged();
}

void SettingsManager::setOutputVolume(int level)
{
    AudioManager::instance().setOutputVolume(level);
    setVal(KeyOutputVolume, level);
    emit outputVolumeChanged();
}
void SettingsManager::setInputMuted(bool muted)
{
    AudioManager::instance().setInputMuted(muted);
    setVal(KeyInputMuted, muted);
    emit inputMutedChanged();
}

void SettingsManager::setOutputMuted(bool muted)
{
    AudioManager::instance().setOutputMuted(muted);
    setVal(KeyOutputMuted, muted);
    emit outputMutedChanged();
}
