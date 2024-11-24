﻿#include "Managers/AudioManager.h"
#include "iostream"
#include <filesystem>

AudioManager::AudioManager()
{
    FMOD::System_Create(&fmodSystem);
    fmodSystem->init(512, FMOD_INIT_3D_RIGHTHANDED, nullptr);
}

AudioManager::~AudioManager()
{
    for (auto& pair : soundCache)
    {
        pair.second->release();
    }
    fmodSystem->release();
}

void AudioManager::Update(float deltaTime)
{
    fmodSystem->update();
    // TODO: Positioning of the listener (Possibly the camera's positioning)
    // fmodSystem->set3DListenerAttributes(0, &listenerPosition, &listenerVelocity, &listenerForward, &listenerUp);
}

void AudioManager::FixedUpdate()
{
    // fmodSystem->update();
}

FMOD::System* AudioManager::GetSystem() {
    return fmodSystem;
}


FMOD::Sound* AudioManager::LoadSound(const std::string& filePath) {
    auto it = soundCache.find(filePath);
    if (it != soundCache.end()) {
        return it->second;
    }
    

    FMOD::Sound* newSound;
    fmodSystem->createSound(filePath.c_str(), FMOD_3D, nullptr, &newSound);
    
    const size_t pos = filePath.find_last_of("/\\");
    const std::string fileName = (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
    soundCache[fileName] = newSound;
    return newSound;
}

FMOD::Sound* AudioManager::GetSound(const std::string& fileName)
{
    return soundCache[fileName];
} 

void AudioManager::SetListenerAttributes(const FMOD_VECTOR& position, const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f }; // TODO: get velocity for doppler
    fmodSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
}

void AudioManager::SetMasterVolume(float volume) {
    fmodSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, 0);
    fmodSystem->setSoftwareChannels(100);
}

void AudioManager::PlaySound(const std::string& soundName) {
	
    auto it = soundCache.find(soundName);
    if (it != soundCache.end()) {
        FMOD::Channel* newChannel = nullptr;
        fmodSystem->playSound(it->second, nullptr, false, &newChannel);
    } else {
        std::cerr << "Sound not found: " << soundName << std::endl;
    }
}