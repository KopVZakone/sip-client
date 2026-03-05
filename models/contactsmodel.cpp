#include "contactsmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
ContactsModel::ContactsModel(QObject *parent) : QSqlTableModel(parent)
{
    setTable("contacts");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};

    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }

    int columnIdx { role - Qt::UserRole - 1 };
    QModelIndex modelIndex = this->index(index.row(), columnIdx);
    return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> ContactsModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {NameRole, "name"},
        {PhoneRole, "phone"},
        {InfoRole, "info"}
    };
}

void ContactsModel::saveContact(int id, const QString &name, const QString &phone, const QString &info)
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

    rec.setValue("name", name);
    rec.setValue("phone", phone);
    rec.setValue("info", info);

    if (row == -1) {
        if (!insertRecord(-1, rec)) {
            qCritical() << "Ошибка добавления контакта:" << lastError().text();
            return;
        }
    } else {
        if (!setRecord(row, rec)) {
            qCritical() << "Ошибка обновления контакта:" << lastError().text();
            return;
        }
    }

    submitAll();
}

void ContactsModel::removeContact(int id)
{
    auto matches = match(index(0, fieldIndex("id")), Qt::DisplayRole, id, 1, Qt::MatchExactly);
    if (!matches.isEmpty()) {
        int row = matches.first().row();
        if (removeRow(row)) {
            submitAll();
        } else {
            qCritical() << "Ошибка удаления ряда:" << lastError().text();
        }
    }
}
