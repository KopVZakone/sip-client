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

    /** @brief Принудительно обновляет списки доступных устройств через PJSIP.
    */
    void refreshDeviceLists();

    /** @brief Включает/выключает звук микрофона, не меняет значения громкости
     */
    void setInputMuted(bool muted);

    /** @brief Включает/выключает звук динамиков, не меняет значения громкости
     */
    void setOutputMuted(bool muted);

    /**
     * @brief Устанавливает громкость микрофона, не включает/выключает его.
     * Значение сохраняется для любого выбранного микрофона
     * @param level процент громкости от максимального уровня от 0 до 100
     */
    void setInputVolume(unsigned level);

    /**
     * @brief Устанавливает громкость динамиков, не включает/выключает их.
     * Значение сохраняется для любых выбранных динамиков
     * @param level процент громкости от максимального уровня от 0 до 100
     */
    void setOutputVolume(unsigned level);
private:
    explicit AudioManager();
    QStringList m_inputs;
    QStringList m_outputs;
    unsigned m_input_volume;
    unsigned m_output_volume;
    bool m_input_muted;
    bool m_output_muted;
    void applyInputVolume(unsigned level);
    void applyOutputVolume(unsigned level);
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};

#endif // AUDIOMANAGER_H
