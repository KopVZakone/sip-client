#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <QObject>


/**
 * @brief Управляет устройствами ввода и вывода
 *  Singleton
 */
class AudioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList inputDevices READ inputDevices NOTIFY devicesChanged)
    Q_PROPERTY(QStringList outputDevices READ outputDevices NOTIFY devicesChanged)
    Q_PROPERTY(QString outputDevice READ outputDevice WRITE setOutputDeviceByName NOTIFY devicesChanged)
    Q_PROPERTY(QString inputDevice READ inputDevice WRITE setInputDeviceByName NOTIFY devicesChanged)
    Q_PROPERTY(int inputVolume READ inputVolume WRITE setInputVolume NOTIFY inputVolumeChanged)
    Q_PROPERTY(int outputVolume READ outputVolume WRITE setOutputVolume NOTIFY outputVolumeChanged)
    Q_PROPERTY(bool inputMuted READ inputMuted WRITE setInputMuted NOTIFY inputMutedChanged)
    Q_PROPERTY(bool outputMuted READ outputMuted WRITE setOutputMuted NOTIFY outputMutedChanged)
public:
    static AudioManager& instance();
    void applySettings();

    Q_INVOKABLE void setInputDeviceByName(QString deviceName);
    Q_INVOKABLE void setOutputDeviceByName(QString deviceName);
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


    int inputVolume() const;
    int outputVolume() const;
    bool inputMuted() const;
    bool outputMuted() const;
    QString outputDevice() const;
    QString inputDevice() const;
signals:
    void devicesChanged();

    void inputVolumeChanged();
    void outputVolumeChanged();
    void inputMutedChanged();
    void outputMutedChanged();
private:
    explicit AudioManager();
    QStringList m_inputs;
    QStringList m_outputs;
    unsigned m_input_volume;
    unsigned m_output_volume;
    bool m_input_muted;
    bool m_output_muted;

    void setDeviceByName(const QString& name, bool isInput);
    void applyInputVolume(unsigned level);
    void applyOutputVolume(unsigned level);
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};

#endif // AUDIOMANAGER_H
