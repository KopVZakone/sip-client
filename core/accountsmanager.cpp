#include "accountsmanager.h"
#include <pjsua2.hpp>
#include <QDebug>
AccountsManager::AccountsManager(QObject *parent)
    : QObject{parent}
{
    m_model = new AccountsModel(this);
}

AccountsManager &AccountsManager::instance()
{
    static AccountsManager inst;
    return inst;
}

void AccountsManager::saveAccount(int id, const QString &displayName, const QString &username, const QString &password, const QString &domain, int port, const QString &protocol)
{
    m_model->saveAccount(id, displayName, username, password, domain, port, protocol);
}

void AccountsManager::removeAccount(int id)
{
    m_model->removeAccount(id);
}
void AccountsManager::registerAccount(int id) {
    if(m_account)
    {
        unregisterAccount(m_account->getId());
    }
    // Получение данных из бд
    auto accountData {m_model->getAccountById(id)};
    const QString username {accountData["username"].toString()};
    const QString password = accountData["password"].toString();
    const QString domain = accountData["domain"].toString();
    [[maybe_unused]]const int port = accountData["port"].toInt();
    [[maybe_unused]]const QString protocol {accountData["protocol"].toString()};

    //Регистрация
    pj::AccountConfig acc_cfg;
    QString idUri = QString("sip:%1@%2").arg(username, domain);
    acc_cfg.idUri = idUri.toStdString();
    QString regUri = QString("sip:%1").arg(domain);
    acc_cfg.regConfig.registrarUri = regUri.toStdString();
    pj::AuthCredInfo cred("digest", "*", username.toStdString(), 0, password.toStdString());
    // TODO: добавить создание транспорта
    acc_cfg.sipConfig.authCreds.push_back(cred);

    try
    {
        m_account = std::make_unique<pj::Account>();
        m_account->create(acc_cfg);
    }
    catch(pj::Error& err)
    {
        qCritical() << "PJSIP Ошибка на старте:" << QString::fromStdString(err.info());
        m_account.reset();
    }
}

void AccountsManager::unregisterAccount(int id)
{
    if(m_account && m_account->getId() == id)
    {
        m_account.reset();
    }
}

AccountsManager::~AccountsManager()
{
    if(m_account)
    {
        unregisterAccount(m_account->getId());
    }

}

void AccountsManager::updateStatus(int id, QString status, QString error)
{
    m_model->updateStatus(id, status, error);
}


