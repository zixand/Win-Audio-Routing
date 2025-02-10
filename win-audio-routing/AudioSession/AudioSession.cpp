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

AudioSessionState AudioSession::getSessionState() {

	HRESULT hr = NULL;
	AudioSessionState sessionState;
	hr = this->pAudioSession->GetState(&sessionState);
	if (FAILED(hr))
	{
		printf("Unable to retrieve device collection: %x\n", hr);
		return AudioSessionStateInactive;
	}
	return sessionState;
}

int AudioSession::getProcessPid() {
	
    HRESULT hr = NULL;
    IAudioSessionControl2* pSessionControl2 = NULL;
    DWORD pid = -1;

    hr = this->pAudioSession->QueryInterface(IID_PPV_ARGS(&pSessionControl2));
    if (FAILED(hr)) {
        printf("Unable to retrieve device collection: %x\n", hr);
        return -1;
    }

    hr = pSessionControl2->GetProcessId(&pid);
    if (FAILED(hr)) {
        printf("Unable to retrieve device collection: %x\n", hr);
        return -1;
    }

	return pid;
}

std::string AudioSession::getSessionIdentifier() {
	
	std::string strError = "-1";
	HRESULT hr = NULL;
	IAudioSessionControl2* pSessionControl2 = NULL;
	LPWSTR name = NULL;
	wchar_t sessionName[128];

	hr = this->pAudioSession->QueryInterface(IID_PPV_ARGS(&pSessionControl2));
	if (FAILED(hr)) {
		printf("Unable to retrieve device collection: %x\n", hr);
		return strError;
	}

	hr = pSessionControl2->GetSessionIdentifier(&name);
	if (FAILED(hr)) {
		printf("Unable to retrieve device collection: %x\n", hr);
		return strError;
	}


	size_t converted = 0;
	size_t len = wcslen(name) + 1;
	char* cstr = new char[len];
	wcstombs_s(&converted, cstr, len, name, _TRUNCATE);
	std::string str(cstr);
	delete[] cstr;

	return str;


}