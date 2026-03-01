#ifndef SIPACCOUNT_H
#define SIPACCOUNT_H

#include <QObject>
#include <pjsua2.hpp>
class SipAccount : public QObject, public pj::Account
{
    Q_OBJECT
public:
    SipAccount(int id);
    virtual void onRegState(pj::OnRegStateParam &prm) override;
  signals:
    void registrationStatusChanged(int id, QString status, QString error);

  private:
    int m_id;
};

#endif // SIPACCOUNT_H
