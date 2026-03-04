#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QObject>
#include <QSqlTableModel>
class HistoryModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        FromNumberRole,
        ToNumberRole,
        StatusRole,
        DurationRole,
        TimestampRole
    };
    explicit HistoryModel(QObject *parent = nullptr);
    Q_INVOKABLE void clearHistory();


    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    int insertCallRecord(QString from, QString to, QString timestamp);
    void updateDuration(int id, int duration);
    void finalizeStatus(int id, QString finalStatus);
};

#endif // HISTORYMODEL_H
