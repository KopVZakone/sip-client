#ifndef ACCOUNTSMANAGER_H
#define ACCOUNTSMANAGER_H

#include "accountsmodel.h"
#include "contactsmodel.h"
#include "sipaccount.h"
#include <QObject>
#include <pjsua2.hpp>
#include <memory>
class AccountsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AccountsModel* model READ model CONSTANT)
    Q_PROPERTY(int selectedAccountIndex READ selectedAccountIndex WRITE selectAccount NOTIFY selectedAccountChanged)
    Q_PROPERTY(QString activeUsername READ activeUsername NOTIFY activeUsernameChanged)
    Q_PROPERTY(ContactsModel* contactsModel READ contactsModel CONSTANT)
public:
    static AccountsManager& instance();
    void applySettings();

    Q_INVOKABLE void saveAccount(int id, const QString &displayName, const QString &username,const QString &password,
                     const QString &domain, int port, const QString &protocol);
    Q_INVOKABLE void removeAccount(int id);
    AccountsModel* model() const { return m_model; }
    ContactsModel* contactsModel() const { return m_contactsModel.get();}
    Q_INVOKABLE void registerAccount(int id);
    Q_INVOKABLE void unregisterAccount(int id);
    int selectedAccountIndex() const;
    QString activeUsername() const;
    void selectAccount(int id);
    SipAccount *getSelectedAccount();
    ~AccountsManager();
public slots:
    // Слот для обновления статуса регистрации в ui после регистрации
    void updateStatus(int id, QString status, QString error = "");
signals:
    void selectedAccountChanged();
    void activeUsernameChanged();
private:
    std::unique_ptr<SipAccount> m_account;
    AccountsModel *m_model;
    std::unique_ptr<ContactsModel> m_contactsModel;
    int m_selectedIndex;

    explicit AccountsManager(QObject *parent = nullptr);
};

#endif // ACCOUNTSMANAGER_H
