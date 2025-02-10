#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include "pch.h"

typedef enum {
	inputEndpoint = 0,
	outputEndpoint = 1
}endpointType_e;

class AudioDevice {
public:
	AudioDevice(IMMDevice*, endpointType_e);
	~AudioDevice();
	std::string getFriendlyName();
	IMMDevice* getEndpoit();
	wchar_t* getSessionIdentifier();

private:
	IMMDevice* endpoint = NULL;
	endpointType_e type;
};



#endif // AUDIO_DEVICE_H
