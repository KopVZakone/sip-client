#include "callmanager.h"

#include "accountsmanager.h"

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
            m_callState = Active;
            emit callStateChanged();
        } catch (pj::Error &err)
        {
            qCritical() << "Ошибка принятия:" << QString::fromStdString(err.info());
        }
    }
}

void CallManager::declineIncomingCall()
{
    if (auto call = getSafeCall())
    {
        pj::CallOpParam prm;
        prm.statusCode = PJSIP_SC_DECLINE;
        try {
            call->answer(prm);
            // clearCall(call);
        } catch (pj::Error &err)
        {
          qCritical() << "Ошибка отклонения: " << QString::fromStdString(err.info());
        }
    }
}

void CallManager::pauseCall()
{
    if (auto call = getSafeCall())
    {
        pj::CallOpParam prm;
        try {
            call->setHold(prm);
            m_callState = Paused;
            emit callStateChanged();
        } catch (pj::Error &err)
        {
          qCritical() << "Ошибка постановки на паузу: " << QString::fromStdString(err.info());
        }
    }
}

void CallManager::resumeCall()
{
    if (auto call = getSafeCall()) {
        pj::CallOpParam prm;
        prm.opt.flag = PJSUA_CALL_UNHOLD;
        try {
            call->reinvite(prm);
            m_callState = Active;
            emit callStateChanged();
        } catch (pj::Error &err) {
            qCritical() << "Ошибка снятия с паузы: " << QString::fromStdString(err.info());
        }
    }
}

void CallManager::hangupCall()
{
    if (auto call = getSafeCall()) {
        pj::CallOpParam prm;
        try {
            m_callState = Ended;
            emit callStateChanged();
            call->hangup(prm);
        } catch (pj::Error &err) {
            qCritical() << "Ошибка сброса звонка: " << QString::fromStdString(err.info());
        }
    }
}

void CallManager::makeCall(QString uri)
{
    SipCall *call{nullptr};

    {
        // Взятие мьютекса для проверки наличия текущего звонка
        std::lock_guard<std::mutex> lock(m_callMutex);
        if(m_currentCall) {
            // Уже есть другой звонок
            return;
        } else {
            auto account { AccountsManager::instance().getSelectedAccount() };
            if(!account)
                return;
            call = new SipCall(*account);
            m_currentCall = call;
            m_callState = Dialing;
        }
    }
    // Звонок с настройками по умолчанию
    pj::CallOpParam prm(true);
    try {
        call->makeCall(uri.toStdString(), prm);
        emit callStateChanged();
    }
    catch (pj::Error &err) {
        qCritical() << "Ошибка инициации звонка: " << QString::fromStdString(err.info());

        clearCall(call);

        // Ручное удаление звонка т.к. callback не будет вызван
        delete call;
    }
}

void CallManager::abortDialingCall()
{
    if (auto call = getSafeCall()) {
        pj::CallOpParam prm;
        try {
            call->hangup(prm);
        } catch (pj::Error &err) {
            qCritical() << "Ошибка отмены звонка: " << QString::fromStdString(err.info());
        }
    }
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

void CallManager::updateCallStatus(SipCall *call, CallState state)
{
    std::lock_guard<std::mutex> lock(m_callMutex);
    if (m_currentCall == call) {
        if (m_callState != state) {
            m_callState = state;
            emit callStateChanged();
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
