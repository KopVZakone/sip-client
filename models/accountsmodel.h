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

    // Названия полей для js объекта
    QHash<int, QByteArray> roleNames() const;

    /**
     * @brief Обновление или создание нового аккаунта если id == -1,
     * по умолчанию не регистрируется
     * при успехе вызывает сигнал accountChanged
     */
    Q_INVOKABLE void saveAccount(int id, const QString &displayName, const QString &username,const QString &password,
                                 const QString &domain, int port, const QString &protocol);
    /**
     * @brief Удаление аккаунта из бд
     *  при успехе вызывает сигнал accountRemoved
     */
    Q_INVOKABLE void removeAccount(int id);

public slots:
    // Слот для обновления статуса регистрации в ui после регистрации
    void updateStatus(int id, QString status, QString error = "");
signals:
    // Сигналы для перерегистрации и удаления аккаунта

    void accountChanged(int id, QString username, QString password, QString domain, int port, QString protocol);
    void accountRemoved(int id);
private:
    //статусы регистрации
    QHash<int, QString> m_regStatuses;
    //ошибки регистрации
    QHash<int, QString> m_errors;
    // Названия колонок бд
    QMap<int, QString> m_roleColName;
};

#endif // ACCOUNTSMODEL_H
