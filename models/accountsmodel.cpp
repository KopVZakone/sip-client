#include "accountsmodel.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
AccountsModel::AccountsModel(QObject *parent)
    : QSqlTableModel{parent}
{
    m_roleColName = {
        {IdRole,          "id"},
        {DisplayNameRole, "display_name"},
        {UsernameRole,    "username"},
        {PasswordRole,    "password"},
        {DomainRole,      "domain"},
        {PortRole,        "port"},
        {ProtocolRole,    "protocol"},
    };
    setTable("accounts");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int row = index.row();

    int idColumn = fieldIndex("id");
    // id для получения статуса
    int accId = QSqlTableModel::data(this->index(row, idColumn), Qt::DisplayRole).toInt();

    // Данные, хранящиеся во время работы приложения, не в бд
    if (role == RegStatusRole)
        return m_regStatuses.value(accId, "offline");
    if (role == LastErrorRole)
        return m_errors.value(accId, "");

    // Данные из бд
    if (role == IdRole)
        return accId;
    if (m_roleColName.contains(role)) {
        int col = fieldIndex(m_roleColName[role]);
        return QSqlTableModel::data(this->index(row, col), Qt::DisplayRole);
    }

    return QSqlTableModel::data(index, role);
}

QHash<int, QByteArray> AccountsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole]           = "id";
    roles[DisplayNameRole]  = "displayName";
    roles[UsernameRole]     = "username";
    roles[PasswordRole]     = "password";
    roles[DomainRole]       = "domain";
    roles[PortRole]         = "port";
    roles[ProtocolRole]     = "protocol";
    roles[RegStatusRole]    = "regStatus";
    roles[LastErrorRole]    = "lastError";
    return roles;
}

void AccountsModel::saveAccount(int id, const QString &displayName, const QString &username,const QString &password,
                                const QString &domain, int port, const QString &protocol)
{
    int row = -1;


    if (id != -1) {
        // Поиск записи в бд
        auto matches = match(index(0, fieldIndex("id")), Qt::DisplayRole, id, 1, Qt::MatchExactly);
        if (!matches.isEmpty()) {
            row = matches.first().row();
        }
    }


    QSqlRecord rec = (row != -1) ? record(row) : record();

    // Запись полей
    rec.setValue(m_roleColName[DisplayNameRole], displayName);
    rec.setValue(m_roleColName[UsernameRole], username);
    rec.setValue(m_roleColName[PasswordRole], password);
    rec.setValue(m_roleColName[DomainRole], domain);
    rec.setValue(m_roleColName[PortRole], port);
    rec.setValue(m_roleColName[ProtocolRole], protocol);

    if (row == -1) {
        // Создание новой записи
        if (!insertRecord(-1, rec)){
            qCritical() << "Ошибка вставки аккаунта в бд: " << query().lastError();
            return;
        }

        id = query().lastInsertId().toInt();
    } else {
        // Изменение существующей
        if (!setRecord(row, rec)){
            qCritical() << "Ошибка обновления аккаунта в бд: " << query().lastError();
            return;
        }
    }
    //Ручное подтверждение операции
    submitAll();

    emit accountChanged(id, username, password, domain, port, protocol);
}

void AccountsModel::removeAccount(int id)
{
    int row = -1;

    auto matches = match(index(0, fieldIndex("id")), Qt::DisplayRole, id, 1, Qt::MatchExactly);
    if (!matches.isEmpty()) {
        row = matches.first().row();
    }
    //Удаление данных из бд
    if (row != -1 && removeRow(row)) {

        //Удаление данных, хранящихся во время работы приложения
        m_regStatuses.remove(id);
        m_errors.remove(id);

        //Ручное подтверждение удаления
        submitAll();

        emit accountRemoved(id);
    }
}

void AccountsModel::updateStatus(int id, QString status, QString error)
{
    m_regStatuses[id] = status;
    m_errors[id] = error;
    auto matches = match(index(0, fieldIndex("id")), Qt::DisplayRole, id, 1, Qt::MatchExactly);
    if (!matches.isEmpty()) {
        QModelIndex idx = matches.first();
        emit dataChanged(idx, idx, {RegStatusRole, LastErrorRole});
    }
}
