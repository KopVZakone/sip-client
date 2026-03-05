#include "sipcall.h"
#include "callmanager.h"
SipCall::SipCall(SipAccount &acc, int call_id) : pj::Call(acc, call_id), m_historyId{-1}
{

}

void SipCall::onCallState(pj::OnCallStateParam &prm)
{
    pj::CallInfo ci = getInfo();
    if (ci.state == PJSIP_INV_STATE_CONNECTING || ci.state == PJSIP_INV_STATE_CONFIRMED) {
        CallManager::instance().updateCallStatus(this, CallManager::Active);
    }

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        // Очистка после отключения
        CallManager::instance().clearCall(this);
        QString finalStatus {"Error"};
        // Сохранение статуса в бд
        std::optional<int> duration{};
        if (ci.role == PJSIP_ROLE_UAC) {
            // Исходящий
            switch (ci.lastStatusCode){
            case PJSIP_SC_OK:
                duration = getInfo().connectDuration.sec;
                finalStatus = "Завершен";
                break;
            case PJSIP_SC_REQUEST_TERMINATED:
                finalStatus = "Отменен";
                break;
            case PJSIP_SC_BUSY_HERE:
                finalStatus = "Занят";
                break;
            case PJSIP_SC_NOT_FOUND:
                finalStatus = "Не найден";
                break;
            case PJSIP_SC_DECLINE:
                finalStatus = "Отклонен";
                break;
            case PJSIP_SC_TEMPORARILY_UNAVAILABLE:
                finalStatus = "Недоступен";
                break;
            default:
                qDebug() << "Не обработанный код исходящего звонка: " << ci.lastStatusCode;
                break;
            }
        } else {
            // Входящий
            switch (ci.lastStatusCode){
            case PJSIP_SC_OK:
                duration = getInfo().connectDuration.sec;
                finalStatus = "Завершен";
                break;
            case PJSIP_SC_REQUEST_TERMINATED:
            case PJSIP_SC_BUSY_HERE:
                finalStatus = "Пропущен";
                break;
            case PJSIP_SC_DECLINE:
                finalStatus = "Отклонен";
                break;
            default:
                qDebug() << "Не обработанный код входящего звонка: " << ci.lastStatusCode;
                break;
            }
        }
        if(duration.has_value()){
            CallManager::instance().model()->updateDuration(m_historyId, duration.value());
        }
        CallManager::instance().model()->finalizeStatus(m_historyId, finalStatus);
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

void SipCall::setHistoryId(int id)
{
    m_historyId = id;
}
