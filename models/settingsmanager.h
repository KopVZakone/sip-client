#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "qsqlquery.h"
#include <QObject>
#include <QVariant>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>
#include <qqmlintegration.h>
class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager& instance();

    template<typename T>
    T getValue(QString key, T defaultValue) const
    {
        QSqlQuery q;
        q.prepare("SELECT value FROM settings WHERE key = ?");
        q.addBindValue(key);
        if (q.exec() && q.next())
            return q.value(0).value<T>();
        return defaultValue;
    }
    //Константы ключи из бд
    static constexpr const char* KeyInputVolume  = "input_vol";
    static constexpr const char* KeyOutputVolume = "output_vol";
    static constexpr const char* KeyInputMuted   = "input_muted";
    static constexpr const char* KeyOutputMuted  = "output_muted";
    static constexpr const char* KeyInputDevice  = "input_name";
    static constexpr const char* KeyOutputDevice = "output_name";
    static constexpr const char* KeySelectedAccount = "selected_account";
    void setVal(QString key, QVariant value);
private:
    explicit SettingsManager(QObject *parent = nullptr);

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
};

#endif // SETTINGSMANAGER_H
