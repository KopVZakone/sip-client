#include "sipcall.h"
#include "callmanager.h"
SipCall::SipCall(SipAccount &acc, int call_id) : pj::Call(acc, call_id)
{

}

void SipCall::onCallState(pj::OnCallStateParam &prm)
{
    pj::CallInfo ci = getInfo();

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        // очистка после отключения
        CallManager::instance().clearCall(this);
        delete this;
    }
}
