#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include <qsqltablemodel.h>
#include <QObject>

class MessagesModel: public QSqlTableModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        RemoteNumberRole,
        MessageTextRole,
        IsOutgoingRole,
        TimestampRole
    };
    explicit MessagesModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void saveMessage(const QString& remoteNumber, const QString&text, bool isOutgoing, bool isRead);
    void markVisibleMessagesAsRead(const QString& remoteNumber);
};

#endif // MESSAGESMODEL_H
