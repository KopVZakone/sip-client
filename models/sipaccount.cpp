#include "sipaccount.h"
#include "sipcall.h"
#include "callmanager.h"


SipAccount::SipAccount(int id, QString username, QString domain, pjsua_transport_id transportId)
    : pj::Account{}, m_id{id}, m_username{username}, m_domain{domain}, m_transportId {transportId}
{

}

void SipAccount::onRegState(pj::OnRegStateParam &prm)
{
    pj::AccountInfo info = getInfo();

    QString statusText{};
    QString errorText{""};
    if (info.regIsActive) {
        statusText = "registered";
    } else {
        switch (prm.code) {
        case 0:   statusText = "registering"; break;
        case 401:
        case 403: statusText = "error"; errorText = QString::fromStdString(prm.reason); break;
        case 408: statusText = "error"; errorText = "timeout"; break;
        default:  statusText = "offline"; break;
        }
    }

    emit registrationStatusChanged(m_id, statusText, errorText);
}

void SipAccount::onRegStarted(pj::OnRegStartedParam &prm)
{
    emit registrationStatusChanged(m_id, "registering");
}

void SipAccount::onIncomingCall(pj::OnIncomingCallParam &prm)
{
    auto currentCall = new SipCall(*this, prm.callId);
    CallManager::instance().registerIncomingCall(currentCall, *this);
}

QString SipAccount::getUsername() const
{
    return m_username;
}

QString SipAccount::getDomain() const
{
    return m_domain;
}

int SipAccount::getAccountId() const
{
    return m_id;
}

pjsua_transport_id SipAccount::getTransportId() const
{
    return m_transportId;
}
