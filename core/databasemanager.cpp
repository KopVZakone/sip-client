#include "databasemanager.h"
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>

DatabaseManager::DatabaseManager(const QString &path)
{
    openDatabase(path);
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
    QString connectionName = m_db.connectionName();
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connectionName);
}

bool DatabaseManager::openDatabase(const QString &path) {
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        m_db = QSqlDatabase::database();
        return m_db.isOpen();
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qCritical() << "Ошибка:" << m_db.lastError().text();
        return false;
    }
    createTables();
    seedTables();
    return true;
}

void DatabaseManager::createTables() {
    QSqlQuery q;
    // Таблица аккаунтов
    q.exec("CREATE TABLE IF NOT EXISTS accounts ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "display_name TEXT, username TEXT, password TEXT, "
           "domain TEXT, port INTEGER, protocol TEXT)");

    // Таблица контактов
    q.exec("CREATE TABLE IF NOT EXISTS contacts ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "name TEXT, phone TEXT, info TEXT)");

    // Таблица истории
    q.exec("CREATE TABLE IF NOT EXISTS history ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "from_number TEXT, to_number TEXT, "
           "status TEXT, duration INTEGER, "
           "timestamp TEXT)");
    // Таблица настроек
    q.exec("CREATE TABLE IF NOT EXISTS settings ("
           "key TEXT PRIMARY KEY, value TEXT)");
    // Таблица сообщений
    q.exec("CREATE TABLE IF NOT EXISTS messages ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "remote_number TEXT, "
           "message_text TEXT, "
           "is_outgoing INTEGER, "
           "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
           "is_read INTEGER)");
    // Индекс для отображения чата с конкретным собеседником
    q.exec("CREATE INDEX IF NOT EXISTS idx_remote_number ON messages(remote_number)");
}

void DatabaseManager::seedTables()
{

}
