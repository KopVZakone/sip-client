#ifndef CHATLISTMODEL_H
#define CHATLISTMODEL_H

#include <qsqlquerymodel.h>
#include <QObject>

class ChatListModel: public QSqlQueryModel
{
    Q_OBJECT
public:
    ChatListModel(QObject *parent = nullptr);
    enum ChatRoles {
        RemoteNumberRole = Qt::UserRole + 1,
        LastMessageRole,
        LastTimestampRole,
        UnreadCountRole
    };
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
public slots:
    void refresh();
};

#endif // CHATLISTMODEL_H
