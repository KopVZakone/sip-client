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
           "name TEXT, phone TEXT UNIQUE, info TEXT)");

    // Таблица истории
    q.exec("CREATE TABLE IF NOT EXISTS history ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "from_number TEXT, to_number TEXT, "
           "status TEXT, duration INTEGER, "
           "timestamp TEXT)");
    q.exec("CREATE TABLE IF NOT EXISTS settings ("
           "key TEXT PRIMARY KEY, value TEXT)");

}

void DatabaseManager::seedTables()
{
    QSqlQuery q;
    // Аккаунты
    q.exec("DELETE FROM accounts");
    q.exec("INSERT INTO accounts (display_name, username, password, domain, port, protocol) VALUES "
           "('Рабочий', '10112312', 'password', 'sip.company.com', 5060, 'UDP'), "
           "('Домашний', '74951234567', 'password', 'sip.provider.net', 5061, 'TLS'), "
           "('Test', 'test', 'password', '192.168.1.50', 5060, 'TCP')");

    // Контакты
    q.exec("DELETE FROM contacts");
    q.prepare("INSERT INTO contacts (name, phone, info) VALUES (?, ?, ?)");
    q.addBindValue("Alice"); q.addBindValue("123-456"); q.addBindValue("Some info");
    q.exec();

    q.addBindValue("Bob"); q.addBindValue("987-654"); q.addBindValue(
                    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum faucibus volutpat gravida. Sed mollis tellus risus,"
                    "sed rhoncus ipsum volutpat eget. Sed a commodo ante, et tincidunt odio."
                    "Quisque elementum quam non mi sollicitudin congue. Vestibulum ante ipsum"
                    "primis in faucibus orci luctus et ultrices posuere cubilia curae;"
                    "Phasellus in semper quam, in viverra risus. Pellentesque scelerisque"
                    "consectetur lectus, eu pretium erat tempus ac. Curabitur augue tellus,"
                    "luctus non posuere finibus, finibus vel libero. Ut lobortis, nibh a accumsan dignissim,"
                    "purus erat condimentum libero, in ultricies nibh nunc hendrerit leo. Aenean iaculis ultrices"
                    "est id vehicula. Etiam sit amet tincidunt ipsum. Vivamus vel ex tincidunt,"
                    "lacinia lacus maximus, tristique orci. Fusce varius risus nec accumsan hendrerit. ");
    q.exec();

    q.addBindValue("Charlie a daosijoidjaoisj xzi cwsdasd"); q.addBindValue("555-111"); q.addBindValue("");
    q.exec();
    q.exec("DELETE FROM history");
    // История
    q.exec("INSERT INTO history (from_number, to_number, timestamp, duration, status) VALUES "
           "('84950000000', '81230453000', '12.05 14:20', '05:12', 'incoming'), "
           "('84000', '12400', '12.05 15:45', '00:00', 'missed'), "
           "('1249504532132312300', '12495045300231231231', '04.04 10:05', '12:30', 'outgoing')");
}
