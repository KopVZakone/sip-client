#include "callmanager.h"

CallManager &CallManager::instance()
{
    static CallManager inst;
    return inst;
}

QString CallManager::remoteCallerNumber()
{
    std::lock_guard<std::mutex> lock(m_callMutex);
    if(!m_currentCall)
        return "";
    pj::CallInfo ci = m_currentCall->getInfo();
    return QString::fromStdString(ci.remoteUri);
}

CallManager::CallState CallManager::callState()
{
    return m_callState;
}

int CallManager::callDuration()
{
    std::lock_guard<std::mutex> lock(m_callMutex);
    if(!m_currentCall)
        return 0;
    return m_currentCall->getInfo().connectDuration.sec;
}

void CallManager::acceptIncomingCall()
{
    if (auto call = getSafeCall())
    {
        pj::CallOpParam prm;
        prm.statusCode = PJSIP_SC_OK;
        try {
            call->answer(prm);
        } catch (pj::Error &err)
        {

        }
    }
}

void CallManager::declineIncomingCall()
{

}

void CallManager::pauseCall()
{

}

void CallManager::hangupCall()
{

}

void CallManager::makeCall(QString uri)
{

}

void CallManager::abortOutgoingCall()
{

}
void CallManager::registerIncomingCall(SipCall *call)
{
    bool isBusy = false;
    {
        // Взятие мьютекса для проверки наличия текущего звонка
        // ответ на звонок подается после проверки для исключения блокировки
        std::lock_guard<std::mutex> lock(m_callMutex);
        if(m_currentCall) {
            // Уже есть другой звонок
            isBusy = true;
        } else {
            m_currentCall = call;
            m_callState = Incoming;
        }
    }

    if(isBusy)
    {
        pj::CallOpParam opPrm;
        opPrm.statusCode = PJSIP_SC_BUSY_HERE;
        call->answer(opPrm);
    }
    else
    {
        // Текущих звонков нет
        pj::CallOpParam opPrm;
        opPrm.statusCode = PJSIP_SC_RINGING;
        try {
            call->answer(opPrm);
            emit callStateChanged();
            emit remoteCallerChanged();
        } catch (pj::Error &err) {
            qCritical() << err.status;
            clearCall(call);
        }
    }
}

CallManager::CallManager()
    : QObject{nullptr}, m_currentCall{nullptr}, m_callState(Idle), m_callMutex{}
{}

SipCall *CallManager::getSafeCall()
{
    // Барьер памяти дл получения актуального значения ссылки
    std::lock_guard<std::mutex> lock(m_callMutex);
    return m_currentCall;
}

void CallManager::clearCall(SipCall *call)
{
    std::lock_guard<std::mutex> lock(m_callMutex);
    if (m_currentCall == call) {
        m_currentCall = nullptr;
        m_callState = Idle;
        emit callStateChanged();
    }
}
