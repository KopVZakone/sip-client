#include "messagesmodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
MessagesModel::MessagesModel(QObject *parent)
    : QSqlTableModel{parent}
{
    setTable("messages");
    setEditStrategy(QSqlTableModel::OnRowChange);
    select();
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }
    int columnIdx = role - Qt::UserRole - 1;
    QModelIndex modelIndex = this->index(index.row(), columnIdx);
    return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {RemoteNumberRole, "remoteNumber"},
        {MessageTextRole, "text"},
        {IsOutgoingRole, "isOutgoing"},
        {TimestampRole, "timestamp"}
    };
}

void MessagesModel::saveMessage(const QString &remoteNumber, const QString &text, bool isOutgoing, bool isRead)
{
    QSqlRecord newRecord = this->record();

    newRecord.setValue("remote_number", remoteNumber);
    newRecord.setValue("message_text", text);
    newRecord.setValue("is_outgoing", static_cast<int>(isOutgoing));
    newRecord.setValue("is_read", static_cast<int>(isRead));
    newRecord.setValue("timestamp", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (insertRecord(-1, newRecord)) {
        submitAll();
        select();
    }
}

void MessagesModel::markVisibleMessagesAsRead(const QString &remoteNumber)
{
    QSqlQuery q;
    q.prepare("UPDATE messages SET is_read = 1 WHERE remote_number = :num AND is_read = 0");
    q.bindValue(":num", remoteNumber);
    q.exec();

    select();
}
