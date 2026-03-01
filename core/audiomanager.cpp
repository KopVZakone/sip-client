#include "audiomanager.h"
#include "settingsmanager.h"
#include <pjsua2.hpp>
#include <QDebug>
AudioManager &AudioManager::instance()
{
    static AudioManager inst;
    return inst;
}

void AudioManager::applySettings()
{
    auto& settings = SettingsManager::instance();
    setInputDeviceByName(settings.getValue<QString>(SettingsManager::KeyInputDevice, ""));
    setOutputDeviceByName(settings.getValue<QString>(SettingsManager::KeyOutputDevice, ""));
    setInputMuted(settings.getValue(SettingsManager::KeyInputMuted, false));
    setInputVolume(settings.getValue(SettingsManager::KeyInputVolume, 100));
    setOutputMuted(settings.getValue(SettingsManager::KeyOutputMuted, false));
    setOutputVolume(settings.getValue(SettingsManager::KeyOutputVolume, 100));
}

void AudioManager::setDeviceByName(const QString &name, bool isInput)
{
    auto& mgr = pj::Endpoint::instance().audDevManager();
    for (const auto& info : mgr.enumDev2()) {
        if (QString::fromUtf8(info.name.c_str()) == name) {
            try {
                if (isInput)
                    mgr.setCaptureDev(info.id);
                else
                    mgr.setPlaybackDev(info.id);
            } catch (pj::Error& e) {
                throw e;
            }
            break;
        }
    }
}

void AudioManager::setInputDeviceByName(QString deviceName)
{
    setDeviceByName(deviceName, true);
    SettingsManager::instance().setVal(SettingsManager::KeyInputDevice, deviceName);
    emit devicesChanged();
}

void AudioManager::setOutputDeviceByName(QString deviceName)
{
    setDeviceByName(deviceName, false);
    SettingsManager::instance().setVal(SettingsManager::KeyOutputDevice, deviceName);
    emit devicesChanged();
}

QStringList AudioManager::inputDevices() const
{
    return m_inputs;
}

QStringList AudioManager::outputDevices() const
{
    return m_outputs;
}

void AudioManager::refreshDeviceLists()
{
    pjmedia_aud_dev_refresh();
    m_inputs.clear();
    m_outputs.clear();
    auto& mgr = pj::Endpoint::instance().audDevManager();

    for (const auto& info : mgr.enumDev2()) {
        if (info.inputCount > 0)
            m_inputs << QString::fromUtf8(info.name.c_str());
        if (info.outputCount > 0)
            m_outputs << QString::fromUtf8(info.name.c_str());
    }
    emit devicesChanged();
}

void AudioManager::setInputMuted(bool muted)
{
    m_input_muted = muted;
    applyInputVolume(muted ? 0 : m_input_volume);
    SettingsManager::instance().setVal(SettingsManager::KeyInputMuted, muted);
    emit inputMutedChanged();
}

void AudioManager::setOutputMuted(bool muted)
{
    m_output_muted = muted;
    applyOutputVolume(muted ? 0 : m_output_volume);
        SettingsManager::instance().setVal(SettingsManager::KeyOutputMuted, muted);
    emit outputMutedChanged();
}

void AudioManager::setInputVolume(unsigned level)
{
    m_input_volume = level;
    if (!m_input_muted) {
        applyInputVolume(level);
    }
    SettingsManager::instance().setVal(SettingsManager::KeyInputVolume, level);
    emit inputVolumeChanged();
}

void AudioManager::setOutputVolume(unsigned level)
{
    m_output_volume = level;
    if (!m_output_muted) {
        applyOutputVolume(level);
    }
    SettingsManager::instance().setVal(SettingsManager::KeyOutputVolume, level);
    emit outputVolumeChanged();
}

int AudioManager::inputVolume() const
{
    return m_input_volume;
}

int AudioManager::outputVolume() const
{
    return m_output_volume;
}

bool AudioManager::inputMuted() const
{
    return m_input_muted;
}

bool AudioManager::outputMuted() const
{
    return m_output_muted;
}

QString AudioManager::outputDevice() const
{
    auto& mgr = pj::Endpoint::instance().audDevManager();
    int outputDevId = mgr.getPlaybackDev();

    auto outputInfo = mgr.getDevInfo(outputDevId);
    return QString::fromUtf8(outputInfo.name.c_str());
}

QString AudioManager::inputDevice() const
{
    auto& mgr = pj::Endpoint::instance().audDevManager();
    int inputDevId = mgr.getCaptureDev();

    auto inputInfo = mgr.getDevInfo(inputDevId);
    return QString::fromUtf8(inputInfo.name.c_str());
}
AudioManager::AudioManager() {
    refreshDeviceLists();
}


void AudioManager::applyInputVolume(unsigned int level)
{
    try {
        pj::Endpoint::instance().audDevManager().setInputVolume(level);
    } catch (pj::Error &err) {
        qCritical() << "Ошибка регулировки микрофона:" << QString::fromStdString(err.info());
    }
}

void AudioManager::applyOutputVolume(unsigned int level)
{
    try {
        pj::Endpoint::instance().audDevManager().setOutputVolume(level);
    } catch (pj::Error &err) {
        qCritical() << "Ошибка регулировки динамиков:" << QString::fromStdString(err.info());
    }
}
