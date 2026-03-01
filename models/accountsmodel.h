#ifndef ACCOUNTSMODEL_H
#define ACCOUNTSMODEL_H

#include <QObject>
#include <QSqlTableModel>

class AccountsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1, DisplayNameRole, UsernameRole, PasswordRole,
        DomainRole, PortRole, ProtocolRole, RegStatusRole, LastErrorRole
    };

    explicit AccountsModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const;

    // Получение записи по первичному индексу
    QVariantMap getDataById(int id) const;
    // Названия полей для js объекта
    QHash<int, QByteArray> roleNames() const;

    QVariantMap getAccountById(int id) const;
    /**
     * @brief Обновление или создание нового аккаунта если id == -1,
     * по умолчанию не регистрируется
     */
    void saveAccount(int id, const QString &displayName, const QString &username,const QString &password,
                                 const QString &domain, int port, const QString &protocol);
    void removeAccount(int id);

    void updateStatus(int id, QString status, QString error);
private:
    //статусы регистрации
    QHash<int, QString> m_regStatuses;
    //ошибки регистрации
    QHash<int, QString> m_errors;
    // Названия колонок бд
    QMap<int, QString> m_roleColName;
};

#endif // ACCOUNTSMODEL_H
