#include "sipcall.h"
#include "callmanager.h"
SipCall::SipCall(SipAccount &acc, int call_id) : pj::Call(acc, call_id)
{

}

void SipCall::onCallState(pj::OnCallStateParam &prm)
{
    pj::CallInfo ci = getInfo();
    if (ci.state == PJSIP_INV_STATE_CONNECTING || ci.state == PJSIP_INV_STATE_CONFIRMED) {
        CallManager::instance().updateCallStatus(this, CallManager::Active);
    }

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        // очистка после отключения
        CallManager::instance().clearCall(this);
        delete this;
    }
}
void SipCall::onCallMediaState(pj::OnCallMediaStateParam &prm)
{
    pj::CallInfo ci = getInfo();

    for (unsigned i = 0; i < ci.media.size(); ++i) {
        if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && ci.media[i].status == PJSUA_CALL_MEDIA_ACTIVE) {
            pj::AudioMedia aud_med = getAudioMedia(i);

            pj::Endpoint::instance().audDevManager().getCaptureDevMedia().startTransmit(aud_med);

            aud_med.startTransmit(pj::Endpoint::instance().audDevManager().getPlaybackDevMedia());
        }
    }
}
