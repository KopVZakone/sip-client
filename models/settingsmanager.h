#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QVariant>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>
#include <qqmlintegration.h>
class SettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList inputDevices READ inputDevices NOTIFY devicesChanged)
    Q_PROPERTY(QStringList outputDevices READ outputDevices NOTIFY devicesChanged)
public:
    static SettingsManager& instance();

    QStringList inputDevices() const;
    QStringList outputDevices() const;
    Q_INVOKABLE void setInputDeviceByName(QString deviceName);
    Q_INVOKABLE void setOutputDeviceByName(QString deviceName);
    Q_INVOKABLE void refreshDeviceLists();
    Q_INVOKABLE QVariant getVal(QString key, QVariant defaultValue = QVariant()) const;

signals:
    void devicesChanged();
private:
    explicit SettingsManager(QObject *parent = nullptr);
    void setVal(QString key, QVariant value);

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
};

#endif // SETTINGSMANAGER_H
