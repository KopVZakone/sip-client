#include "callmanager.h"

#include "accountsmanager.h"

CallManager &CallManager::instance()
{
    static CallManager inst;
    return inst;
}

QString CallManager::remoteCallerNumber()
{
    if (auto call = getSafeCall())
    {
        pj::CallInfo ci = m_currentCall->getInfo();
        return QString::fromStdString(ci.remoteUri);
    } else {
        return "";
    }
}

CallManager::CallState CallManager::callState()
{
    return m_callState;
}

int CallManager::callDuration()
{
    if (auto call = getSafeCall())
    {
        return call->getInfo().connectDuration.sec;
    } else {
        return 0;
    }
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
    // TODO: пофиксить отключение собеседника после 10-20 секунд после продолжения разговора
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

void CallManager::makeCall(QString remoteUsername)
{
    SipCall *call{nullptr};
    QString accountUsername {""};
    QString accountDomain {""};
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
            accountUsername = account->getUsername();
            accountDomain = account->getDomain();
        }
    }

    QString uri = QString("sip:%1@%2").arg(remoteUsername, accountDomain);
    // Звонок с настройками по умолчанию
    pj::CallOpParam prm(true);
    try {
        // Сохранение в бд
        auto timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        auto historyId = m_model->insertCallRecord(accountUsername, uri, timestamp);
        // Установка id для сохранения статуса и длительности в бд по завершению
        call->setHistoryId(historyId);


        // Только после сохранения начать звонок
        call->makeCall(uri.toStdString(), prm);



    }
    catch (pj::Error &err) {
        qCritical() << "Ошибка инициации звонка: " << QString::fromStdString(err.info());

        clearCall(call);

        // Ручное удаление звонка т.к. callback не будет вызван
        delete call;
    }
    emit callStateChanged();
    emit remoteCallerChanged();
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
void CallManager::registerIncomingCall(SipCall *call, SipAccount &callee)
{
    bool isBusy {false};
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
    auto from {QString::fromStdString(call->getInfo().remoteUri)};
    auto timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    int historyId {m_model->insertCallRecord(from,callee.getUsername(), timestamp)};
    call->setHistoryId(historyId);
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
    if (getSafeCall() == call) {
        if (m_callState != state) {
            m_callState = state;
        }
    }
    if(state == Active){
        emit callDurationChanged();
        QMetaObject::invokeMethod(&m_durationTimer, "start", Qt::QueuedConnection);
    }
    emit callStateChanged();
}

HistoryModel *CallManager::model()
{
    return m_model.get();
}

HistoryProxyModel *CallManager::proxy()
{
    return m_proxy.get();
}

void CallManager::updateDuration()
{
    emit callDurationChanged();
}

CallManager::CallManager()
    : QObject{nullptr}, m_currentCall{nullptr},
    m_callState{Idle}, m_callMutex{}, m_durationTimer{},
    m_model{std::make_unique<HistoryModel>()},
    m_proxy{std::make_unique<HistoryProxyModel>(m_model.get())}
{
    m_durationTimer.setInterval(500);
    connect(&m_durationTimer, &QTimer::timeout, this, &CallManager::updateDuration);
}

SipCall *CallManager::getSafeCall()
{
    // Барьер памяти для получения актуального значения указателя
    std::lock_guard<std::mutex> lock(m_callMutex);
    return m_currentCall;
}

void CallManager::clearCall(SipCall *call)
{
    bool cleared {false};
    {
        std::lock_guard<std::mutex> lock(m_callMutex);
        if (m_currentCall == call) {
            m_currentCall = nullptr;
            cleared = true;
        }
    }
    if(cleared){
        m_callState = Idle;
        emit callStateChanged();
        QMetaObject::invokeMethod(&m_durationTimer, "stop", Qt::QueuedConnection);
    }
}
