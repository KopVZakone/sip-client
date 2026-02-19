#include "audiomanager.h"
#include <pjsua2.hpp>
#include <QDebug>
AudioManager &AudioManager::instance()
{
    static AudioManager inst;
    return inst;
}

void AudioManager::setDeviceByName(const QString &name, bool isInput)
{
    auto& mgr = pj::Endpoint::instance().audDevManager();
    for (auto info : mgr.enumDev2()) {
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

    for (auto info : mgr.enumDev2()) {
        if (info.inputCount > 0)
            m_inputs << QString::fromUtf8(info.name.c_str());
        if (info.outputCount > 0)
            m_outputs << QString::fromUtf8(info.name.c_str());
    }
}

void AudioManager::setInputMuted(bool muted)
{
    m_input_muted = muted;
    applyInputVolume(muted ? 0 : m_input_volume);
}

void AudioManager::setOutputMuted(bool muted)
{
    m_output_muted = muted;
    applyOutputVolume(muted ? 0 : m_output_volume);
}

void AudioManager::setInputVolume(unsigned level)
{
    m_input_volume = level;
    if (!m_input_muted) {
        applyInputVolume(level);
    }
}

void AudioManager::setOutputVolume(unsigned level)
{
    m_output_volume = level;
    if (!m_output_muted) {
        applyOutputVolume(level);
    }
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
