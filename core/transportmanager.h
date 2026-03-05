#ifndef TRANSPORTMANAGER_H
#define TRANSPORTMANAGER_H
#include <pjsua2.hpp>
#include <QString>
#include <mutex>
#include <QMap>
class TransportManager
{
public:
    static TransportManager& instance();
    std::optional<pjsua_transport_id> createTransport(const QString &protocol, int port);

    void releaseTransport(pjsua_transport_id id);
private:

    struct TransportInfo {
        pjsua_transport_id id;
        int refCount = 0;
        QString key;
    };

    std::mutex m_mutex;
    QMap<pjsua_transport_id, TransportInfo> m_transports;
    QMap<QString, pjsua_transport_id> m_keyToId;
    explicit TransportManager();
};

#endif // TRANSPORTMANAGER_H
