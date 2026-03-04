#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include "sipcall.h"
#include <QObject>
#include <QtQml>
#include <mutex>
class CallManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
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
    QString remoteCallerNumber();
    CallState callState();
    int callDuration();
    Q_INVOKABLE void acceptIncomingCall();
    Q_INVOKABLE void declineIncomingCall();
    Q_INVOKABLE void pauseCall();
    Q_INVOKABLE void resumeCall();
    Q_INVOKABLE void hangupCall();
    Q_INVOKABLE void makeCall(QString uri);
    Q_INVOKABLE void abortDialingCall();
    /**
     * @brief Удаляет ссылку на звонок и устанавливает статус на Idle.
     * Ничего не делает, если звонок уже удален.
     * Не вызывает деструктор SipCall
     * @param call звонок для удаления.
     */
    void clearCall(SipCall* call);
    void registerIncomingCall(SipCall *call);
    void updateCallStatus(SipCall *call, CallState state);
signals:
    void remoteCallerChanged();
    void callStateChanged();
    void callDurationChanged();
private slots:
    void updateDuration();
private:
    explicit CallManager();
    SipCall *m_currentCall;
    QTimer m_durationTimer;
    CallState m_callState;
    std::mutex m_callMutex;
    SipCall *getSafeCall();
};

#endif // CALLMANAGER_H
