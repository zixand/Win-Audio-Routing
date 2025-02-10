#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "pch.h"
#include "AudioDevice.h"
#include "AudioSession.h"

class AudioManager {

public:
	AudioManager();
	~AudioManager();
	void updateEndpoint();
	bool createSession(AudioDevice* device);
	std::map<int, std::string> getActiveProcess();

private:
	IMMDeviceEnumerator* pEnumerator = NULL;
	std::vector<AudioDevice*> endpoints;
	std::vector<AudioSession*> session;

	bool setInputEndpoints(IMMDeviceEnumerator* collector);
	bool setOutputEndpoints(IMMDeviceEnumerator* collector);
	bool setInterfaceState(std::vector<AudioDevice*> endpoint);
};


#endif // AUDIO_MANAGER_H