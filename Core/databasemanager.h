#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
class DatabaseManager
{
public:
    explicit DatabaseManager(const QString &path);
    ~DatabaseManager();

    bool openDatabase(const QString &path);
signals:
private:
    void createTables();
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
