#ifndef MODELAUDIOROUTING_H
#define MODELAUDIOROUTING_H

#include "pch.h"
#include "AudioManager.h"

class ModelAudioRouting {	
public:
	ModelAudioRouting();
	~ModelAudioRouting();

private:
	AudioManager* audioManager;
	//ControllerAudioRouting* controller;

};

#endif // MODELAUDIOROUTING_H
