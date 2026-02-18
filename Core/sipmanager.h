#ifndef SIPMANAGER_H
#define SIPMANAGER_H
#include <pjsua2.hpp>
#include <memory>
class SipManager
{
public:
    static SipManager& instance();
    /**
    * @brief Инициализирует и запускает ядро PJSIP в однопоточном режиме
    * (threadCnt=0)
    * @note Необходимо вызвать перед обращением к функциям библиотеки
    */
    void start();
    /**
    * @brief Освобождает ресурсы библиотеки.
    */
    void stop();

    ~SipManager();
private:
    SipManager();
    std::unique_ptr<pj::Endpoint> m_endpoint;

    SipManager(const SipManager&) = delete;
    SipManager& operator=(const SipManager&) = delete;
};

#endif // SIPMANAGER_H
