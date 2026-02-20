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
    Q_PROPERTY(int inputVolume READ inputVolume WRITE setInputVolume NOTIFY inputVolumeChanged)
    Q_PROPERTY(int outputVolume READ outputVolume WRITE setOutputVolume NOTIFY outputVolumeChanged)
    Q_PROPERTY(bool inputMuted READ inputMuted WRITE setInputMuted NOTIFY inputMutedChanged)
    Q_PROPERTY(bool outputMuted READ outputMuted WRITE setOutputMuted NOTIFY outputMutedChanged)
public:
    static SettingsManager& instance();

    QStringList inputDevices() const;
    QStringList outputDevices() const;
    Q_INVOKABLE void setInputDeviceByName(QString deviceName);
    Q_INVOKABLE void setOutputDeviceByName(QString deviceName);
    Q_INVOKABLE void refreshDeviceLists();
    Q_INVOKABLE QVariant getVal(QString key, QVariant defaultValue = QVariant()) const;

    int inputVolume() const;
    int outputVolume() const;
    bool inputMuted() const;
    bool outputMuted() const;

    void setInputVolume(int level);
    void setOutputVolume(int level);
    void setInputMuted(bool muted);
    void setOutputMuted(bool muted);
signals:
    void devicesChanged();

    void inputVolumeChanged();
    void outputVolumeChanged();
    void inputMutedChanged();
    void outputMutedChanged();
private:

    //Константы ключи из бд

    static constexpr const char* KeyInputVolume  = "input_vol";
    static constexpr const char* KeyOutputVolume = "output_vol";
    static constexpr const char* KeyInputMuted   = "input_muted";
    static constexpr const char* KeyOutputMuted  = "output_muted";
    static constexpr const char* KeyInputDevice  = "input_name";
    static constexpr const char* KeyOutputDevice = "output_name";
    explicit SettingsManager(QObject *parent = nullptr);
    void setVal(QString key, QVariant value);

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
};

#endif // SETTINGSMANAGER_H
