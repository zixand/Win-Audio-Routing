#ifndef AUDIO_SESSION_H
#define AUDIO_SESSION_H

#include "pch.h"
#include "AudioDevice.h"

class AudioSession {
public:
	AudioSession(IAudioSessionControl*, AudioDevice*);
	~AudioSession();
	IAudioSessionControl* getAudioSessionControl();
	int getProcessPid();
	AudioDevice* getDevice();


private:

	IAudioSessionControl* pAudioSession = nullptr;
	AudioDevice* device = nullptr;

};

#endif // AUDIO_SESSION_H