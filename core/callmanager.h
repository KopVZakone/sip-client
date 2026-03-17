#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include "historymodel.h"
#include "sipcall.h"
#include <QObject>
#include <QtQml>
#include <historyproxymodel.h>
#include <mutex>
class CallManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(HistoryModel* model READ model CONSTANT)
    Q_PROPERTY(HistoryProxyModel* proxy READ proxy CONSTANT)
    Q_PROPERTY(QString remoteCallerNumber READ remoteCallerNumber NOTIFY remoteCallerChanged)
    Q_PROPERTY(CallState callState READ callState NOTIFY callStateChanged)
    Q_PROPERTY(int callDuration READ callDuration NOTIFY callDurationChanged)
public:
    enum CallState{
        Idle = 1,
        Incoming,
        Dialing,
        Active,
        Paused,
        Ended,
    };
    Q_ENUM(CallState)
    static CallManager& instance();
    /**
     * @brief Возвращает номер удаленного обонента.
     * @return Если нет звонка, то возвращает "", иначе номер без домена
     */
    QString remoteCallerNumber();
    CallState callState();
    /**
     * @brief Возвращает длительность звонка
     * @return Если нет звонка, то 0, иначе длительность в секундах
     */
    int callDuration();
    HistoryModel *model();
    HistoryProxyModel *proxy();
    /**
     * @brief Принимает входящий звонок.
     * Не меняет состояния
     */
    Q_INVOKABLE void acceptIncomingCall();
    /**
     * @brief Отклоняет входящий звонок.
     * Не меняет состояния
     */
    Q_INVOKABLE void declineIncomingCall();
    /**
     * @brief Ставит звонок на паузу.
     * Меняет состояние на CallManager::Pause
     */
    Q_INVOKABLE void pauseCall();
    /**
     * @brief Снимает звонок с паузы.
     * Меняет состояние на CallManager::Active
     */
    Q_INVOKABLE void resumeCall();
    /**
     * @brief Завершает звонок.
     * Меняет состояние на CallManager::Ended
     */
    Q_INVOKABLE void hangupCall();
    /**
     * @brief Начинает звонок.
     * Меняет состояние на CallManager::Dialing.
     * Сохраняет в бд в историю начало звонка
     */
    Q_INVOKABLE void makeCall(QString remoteUsername);
    Q_INVOKABLE void abortDialingCall();
    /**
     * @brief Удаляет ссылку на звонок
     * Меняет состояние на CallManager::Idle.
     * Ничего не делает, если звонок уже удален.
     * Не вызывает деструктор SipCall
     * @param call звонок для удаления.
     */
    void clearCall(SipCall* call);
    /**
     * @brief Если нет текущего, сохраняет звонок и
     * меняет состояние на CallManager::Incoming.
     * Иначе отвечает на него кодом BUSY_HERE
     * Сохраняет в бд начало звонка либо пропущеный звонок.
     */
    void registerIncomingCall(SipCall *call, SipAccount &callee);
    /**
     * @brief Обновляет статус звонка,
     * если статус CallManager::Active запускает таймер обновления
     * интерфейса для длительности.
     */
    void updateCallStatus(SipCall *call, CallState state);
signals:
    void remoteCallerChanged();
    void callStateChanged();
    void callDurationChanged();
private slots:
    /**
     * @brief По таймеру посылает сигнал callDurationChanged
     */
    void updateDuration();
private:
    explicit CallManager();
    SipCall *m_currentCall;
    QTimer m_durationTimer;
    CallState m_callState;
    std::mutex m_callMutex;
    SipCall *getSafeCall();

    std::unique_ptr<HistoryModel> m_model;
    std::unique_ptr<HistoryProxyModel> m_proxy;
};

#endif // CALLMANAGER_H
