#ifndef SIPCALL_H
#define SIPCALL_H

#include "sipaccount.h"

#include <QObject>
#include <pjsua2.hpp>
class SipCall : public pj::Call
{
public:
    explicit SipCall(SipAccount &acc, int call_id = PJSUA_INVALID_ID);
    void onCallState(pj::OnCallStateParam &prm) override;
    void onCallMediaState(pj::OnCallMediaStateParam &prm) override;
    void setHistoryId(int id);
private:
    int m_historyId;
};

#endif // SIPCALL_H
