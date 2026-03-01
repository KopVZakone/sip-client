#ifndef ACCOUNTSMANAGER_H
#define ACCOUNTSMANAGER_H

#include "accountsmodel.h"
#include <QObject>
#include <pjsua2.hpp>
#include <memory>
class AccountsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AccountsModel* model READ model CONSTANT)
public:
    static AccountsManager& instance();
    Q_INVOKABLE void saveAccount(int id, const QString &displayName, const QString &username,const QString &password,
                     const QString &domain, int port, const QString &protocol);
    Q_INVOKABLE void removeAccount(int id);
    AccountsModel* model() const { return m_model; }
    Q_INVOKABLE void registerAccount(int id);
    Q_INVOKABLE void unregisterAccount(int id);
    ~AccountsManager();
public slots:
    // Слот для обновления статуса регистрации в ui после регистрации
    void updateStatus(int id, QString status, QString error = "");
private:
    std::unique_ptr<pj::Account> m_account;
    AccountsModel *m_model;
    explicit AccountsManager(QObject *parent = nullptr);
};

#endif // ACCOUNTSMANAGER_H
