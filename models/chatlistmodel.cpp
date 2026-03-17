#include "chatlistmodel.h"

ChatListModel::ChatListModel(QObject *parent)
    : QSqlQueryModel {parent}
{
    refresh();
}

QHash<int, QByteArray> ChatListModel::roleNames() const
{
    return {
        {RemoteNumberRole, "remote_number"},
        {LastMessageRole, "last_message"},
        {LastTimestampRole, "last_timestamp"},
        {UnreadCountRole, "unread_count"},
    };
}

QVariant ChatListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    if (role < Qt::UserRole) {
        return QSqlQueryModel::data(index, role);
    }
    int columnIdx = role - Qt::UserRole - 1;
    QModelIndex modelIndex = this->index(index.row(), columnIdx);
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

void ChatListModel::refresh()
{
    QString sql {R"(
        SELECT
            remote_number,
            message_text AS last_message,
            MAX(timestamp) AS last_timestamp,
            SUM(CASE WHEN is_read = 0 AND is_outgoing = 0 THEN 1 ELSE 0 END) AS unread_count
        FROM messages
        GROUP BY remote_number
        ORDER BY last_timestamp DESC;
    )"};
    setQuery(sql);
}
