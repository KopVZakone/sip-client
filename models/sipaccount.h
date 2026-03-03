#ifndef SIPACCOUNT_H
#define SIPACCOUNT_H

#include <QObject>
#include <pjsua2.hpp>

class SipCall;


class SipAccount : public QObject, public pj::Account
{
    Q_OBJECT
public:
    SipAccount(int id);
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
    int getAccountId();
signals:
    void registrationStatusChanged(int id, QString status, QString error="");
private:
    int m_id;
};

#endif // SIPACCOUNT_H
