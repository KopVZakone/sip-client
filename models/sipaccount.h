#ifndef SIPACCOUNT_H
#define SIPACCOUNT_H

#include <QObject>
#include <pjsua2.hpp>

class SipCall;


class SipAccount : public QObject, public pj::Account
{
    Q_OBJECT
public:
    SipAccount(int id, QString username, QString domain, pjsua_transport_id transportId);
    /**
     * @brief Callback по изменению статуса регистрации.
     * Посылает сигнал registrationStatusChanged
     */
    void onRegState(pj::OnRegStateParam &prm) override;
    /**
     * @brief Callback по началу регистрации.
     * Посылает сигнал registrationStatusChanged(id, "registering")
     */
    void onRegStarted(pj::OnRegStartedParam &prm) override;
    /**
     * @brief Callback по получению входящего звонка.
     *  Посылает сигнал incomingCall
     */
    void onIncomingCall(pj::OnIncomingCallParam &prm) override;
    QString getUsername() const;
    QString getDomain() const;
    int getAccountId() const;
    pjsua_transport_id getTransportId() const;
signals:
    void registrationStatusChanged(int id, QString status, QString error="");
private:
    int m_id;
    QString m_username;
    QString m_domain;
    pjsua_transport_id m_transportId;
};

#endif // SIPACCOUNT_H
