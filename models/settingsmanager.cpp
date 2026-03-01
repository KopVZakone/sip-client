#include "settingsmanager.h"
#include <QtSql/QSqlQuery>
SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent}
{ }

SettingsManager &SettingsManager::instance()
{
    static SettingsManager inst;
    return inst;
}


void SettingsManager::setVal(QString key, QVariant value)
{
    QSqlQuery q;
    q.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (?, ?)");
    q.addBindValue(key);
    q.addBindValue(value.toString());
    q.exec();
}
