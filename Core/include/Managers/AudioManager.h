#ifndef AUDIO_MGR_H

#include <fmod.h>
#include <string>

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	void update();

	FMOD::System* getSystem();

private:
	FMOD::System* fmodSystem = nullptr;
};

// Convert this kind of structure to ECS Components
//struct AudioComponent {
//    std::string filePath;
//    FMOD::Sound* sound = nullptr;
//    FMOD::Channel* channel = nullptr;
//    bool isPlaying = false;
//
//    // 3D sound attributes
//    FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
//    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
//};
//
//struct ListenerComponent {
//    FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
//    FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
//    FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
//};


#endif