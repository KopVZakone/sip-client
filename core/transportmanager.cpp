#include "transportmanager.h"
#include <QDebug>
TransportManager &TransportManager::instance()
{
    static TransportManager inst;
    return inst;
}

std::optional<pjsua_transport_id> TransportManager::createTransport(const QString &protocol, int port)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    QString key = QString("%1:%2").arg(protocol.toLower()).arg(port);

    if (m_keyToId.contains(key)) {
        pjsua_transport_id existingId = m_keyToId[key];
        m_transports[existingId].refCount++;
        return existingId;
    }

    pj::TransportConfig cfg;
    cfg.port = port;

    pjsip_transport_type_e type = PJSIP_TRANSPORT_UDP;
    if (protocol.toLower() == "tcp") type = PJSIP_TRANSPORT_TCP;
    else if (protocol.toLower() == "tls") type = PJSIP_TRANSPORT_TLS;

    try {
        pjsua_transport_id newId = pj::Endpoint::instance().transportCreate(type, cfg);

        TransportInfo info;
        info.id = newId;
        info.refCount = 1;
        info.key = key;

        m_transports[newId] = info;
        m_keyToId[key] = newId;

        return newId;
    } catch (pj::Error &err) {
        qCritical() << "Ошибка создания транспорта" << QString::fromStdString(err.info());
        return std::nullopt;
    }
}

void TransportManager::releaseTransport(pjsua_transport_id id)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_transports.contains(id)) return;

    m_transports[id].refCount--;

    // if (m_transports[id].refCount <= 0) {
    //     try {
    //         QString key = m_transports[id].key;
    //         m_keyToId.remove(key);
    //         m_transports.remove(id);

    //         qDebug() << "Транспорт" << id << "удален";
    //     } catch (pj::Error &err) {
    //     qCritical() << "Ошибка удаления транспорта" << QString::fromStdString(err.info());
    //     }
    // }
}

TransportManager::TransportManager() {}
