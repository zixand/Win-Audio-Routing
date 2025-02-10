#include "AudioSession.h"


AudioSession::AudioSession(IAudioSessionControl* pAudioSession, AudioDevice* audioDev) {

	this->pAudioSession = pAudioSession;
	this->device = audioDev;

}

AudioSession::~AudioSession() {


}

IAudioSessionControl* AudioSession::getAudioSessionControl() {
	return this->pAudioSession;
}

AudioDevice* AudioSession::getDevice() {
	return this->device;
}	

int AudioSession::getProcessPid() {
	
    HRESULT hr = NULL;
    AudioSessionState sessionState;
    IAudioSessionControl2* pSessionControl2 = NULL;
    DWORD pid = -1;

    hr = this->pAudioSession->GetState(&sessionState);
    if (FAILED(hr))
    {
        printf("Unable to retrieve device collection: %x\n", hr);
        return -1;
    }

    if (sessionState == AudioSessionStateActive) {

        hr = this->pAudioSession->QueryInterface(IID_PPV_ARGS(&pSessionControl2));
        if (FAILED(hr))
        {
            printf("Unable to retrieve device collection: %x\n", hr);
            return -1;
        }

        hr = pSessionControl2->GetProcessId(&pid);
        if (FAILED(hr))
        {
            printf("Unable to retrieve device collection: %x\n", hr);
            return -1;
        }
    }

	return pid;
}