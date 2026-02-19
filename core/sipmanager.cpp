#include "sipmanager.h"
#include <QDebug>
SipManager &SipManager::instance()
{
    static SipManager inst;
    return inst;
}

void SipManager::start()
{
    try {
        m_endpoint->libCreate();

        pj::EpConfig ep_cfg;
        ep_cfg.uaConfig.threadCnt = 0;
        m_endpoint->libInit(ep_cfg);

        m_endpoint->libStart();

    } catch (pj::Error &err) {
        qCritical() << "PJSIP Ошибка на старте:" << QString::fromStdString(err.info());
    }
}

void SipManager::stop()
{
    if(m_endpoint)
    {
        try {
            if (m_endpoint->libGetState() >= PJSUA_STATE_INIT) {
                m_endpoint->libDestroy();
            }
            m_endpoint.reset();
            qDebug() << "PJSIP успешно остановлен";
        } catch (pj::Error &err) {
            qDebug() << "Ошибка при деструкции PJSIP:" << QString::fromStdString(err.info());
        }
    }

}

SipManager::SipManager() {
    m_endpoint = std::make_unique<pj::Endpoint>();
}

SipManager::~SipManager()
{
    stop();
}
