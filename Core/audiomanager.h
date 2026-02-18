#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <QObject>


/**
 * @brief Управляет устройствами ввода и вывода
 *  Singleton
 */
class AudioManager
{
public:
    static AudioManager& instance();
    void setDeviceByName(const QString& name, bool isInput);

    /** @brief Возвращает закешированный список имен доступных устройств аудио ввода.
    */
    QStringList inputDevices() const;

    /** @brief Возвращает закешированный список имен доступных устройств аудио вывода.
    */
    QStringList outputDevices() const;

    /** @brief Принудительно обновляет списки доступных устройств в через PJSIP.
    */
    void refreshDeviceLists();

private:
    explicit AudioManager();
    QStringList m_inputs;
    QStringList m_outputs;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};

#endif // AUDIOMANAGER_H
