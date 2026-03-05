#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QSqlTableModel>
#include <QObject>

class ContactsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum ContactRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        PhoneRole,
        InfoRole
    };

    ContactsModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void saveContact(int id, const QString &name, const QString &phone, const QString &info);
    Q_INVOKABLE void removeContact(int id);
};

#endif // CONTACTSMODEL_H
