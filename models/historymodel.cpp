#include "historymodel.h"
#include "qsqlquery.h"
#include "qsqlrecord.h"


HistoryModel::HistoryModel(QObject *parent) : QSqlTableModel{parent}
{
    setTable("history");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setSort(fieldIndex("timestamp"), Qt::DescendingOrder);
    select();
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    if (role < Qt::UserRole) {
        return QSqlTableModel::data(index, role);
    }
    int columnIdx = role - Qt::UserRole - 1;
    QModelIndex modelIndex = this->index(index.row(), columnIdx);
    return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> HistoryModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {FromNumberRole, "fromNumber"},
        {ToNumberRole, "toNumber"},
        {StatusRole, "status"},
        {DurationRole, "duration"},
        {TimestampRole, "timestamp"}
    };
}

int HistoryModel::insertCallRecord(QString from, QString to, QString timestamp)
{
    QSqlRecord rec = record();
    rec.setValue("from_number", from);
    rec.setValue("to_number", to);
    rec.setValue("status", "В процессе...");
    rec.setValue("duration", 0);
    rec.setValue("timestamp", timestamp);
    if (insertRecord(-1, rec)) {
        submitAll();
        select();
        return query().lastInsertId().toInt();
    }
    return -1;
}

void HistoryModel::updateDuration(int id, int duration)
{
    auto matches = match(index(0, fieldIndex("id")), Qt::DisplayRole, id, 1, Qt::MatchExactly);
    if (!matches.isEmpty()) {
        QModelIndex idx = matches.first();
        int row = idx.row();
        setData(index(row, fieldIndex("duration")), duration);
        emit dataChanged(idx, idx, {HistoryModel::DurationRole});
    }
}

void HistoryModel::finalizeStatus(int id, QString finalStatus)
{
    auto matches = match(index(0, fieldIndex("id")), Qt::DisplayRole, id, 1, Qt::MatchExactly);
    if (!matches.isEmpty()) {
        QModelIndex idx = matches.first();
        int row = idx.row();
        setData(index(row, fieldIndex("status")), finalStatus);
        emit dataChanged(idx, idx, {HistoryModel::StatusRole});
    }
}

void HistoryModel::clearHistory()
{
    QSqlQuery q;
    if (q.exec("DELETE FROM history")) {
        submitAll();
        select();
    }
}
