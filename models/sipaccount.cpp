#include "sipaccount.h"


SipAccount::SipAccount(int id) : m_id(id)
{ }

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
