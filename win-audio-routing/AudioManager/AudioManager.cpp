#include "AudioManager.h"


AudioManager::AudioManager() {

	this->updateEndpoint();

}

AudioManager::~AudioManager() {

}

void AudioManager::updateEndpoint() {


	HRESULT hr = CoInitialize(NULL);
	
	
	
	if (FAILED(hr)) {
		std::cerr << "Errore durante l'inizializzazione di COM." << std::endl;
		return;
	}
	
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->pEnumerator));
	if (FAILED(hr)) {
		
		printf("Error: CoCreateInstance(): %d\n", hr);
		return;

	}

	if (!this->setInputEndpoints(this->pEnumerator)) {
		printf("Error: setInputEndpoints()\n");
		return;
	}

	if (!this->setOutputEndpoints(this->pEnumerator)) {
		printf("Error: setOutputEndpoints()\n");
		return;
	}

	if (!this->setInterfaceState(this->endpoints)) {
		printf("Error: setInterfaceState()\n");
		return;
	}

	for (auto session : this->session) {
		std::cout << "Session: " << session->getDevice()->getFriendlyName() << std::endl;
		std::cout << "PID: " << session->getProcessPid() << std::endl;
	}

}

bool AudioManager::setInterfaceState(std::vector<AudioDevice*> endpoint) {
	
	HRESULT hr = NULL;
	IAudioSessionManager2* pSessionManager;
	IAudioSessionEnumerator* audioSessionEnumerator;
	int audioSessionCount = 0;

	for (auto device : endpoint) {
		pSessionManager = NULL;
		hr = device->getEndpoit()->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL, (void**)&pSessionManager);
		if (FAILED(hr)) {
			printf("Error: Activate(): %d\n", hr);
			return false;
		}
		audioSessionEnumerator = NULL;
		hr = pSessionManager->GetSessionEnumerator(&audioSessionEnumerator);
		if (FAILED(hr))
		{
			printf("Unable to retrieve device collection: %x\n", hr);
			return NULL;
		}
		hr = audioSessionEnumerator->GetCount(&audioSessionCount);
		if (FAILED(hr))
		{
			printf("Unable to retrieve device collection: %x\n", hr);
			return NULL;
		}
		for (auto index = 0; index < audioSessionCount; index++) {
			IAudioSessionControl* audioSessionControl;
			hr = audioSessionEnumerator->GetSession(index, &audioSessionControl);
			if (FAILED(hr))
			{
				printf("Unable to retrieve device collection: %x\n", hr);
				return false;
			}
			this->session.push_back(new AudioSession(audioSessionControl, device));

			audioSessionControl->Release();
		}


	}
	
	return true;
}

bool AudioManager::setOutputEndpoints(IMMDeviceEnumerator* enumerator) {

	HRESULT hr = NULL;
	UINT deviceCount = 0;
	IMMDeviceCollection* pEndpointCollector = NULL;

	if (!enumerator) {
		return false;
	}

	hr = enumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pEndpointCollector);
	if (FAILED(hr)) {

		printf("Error: EnumAudioEndpoints(): %d\n", hr);
		return false;
	}

	hr = pEndpointCollector->GetCount(&deviceCount);
	if (FAILED(hr))
	{
		printf("Unable to get endPoint count: %x\n", hr);
		return false;
	}

	for (auto index = 0; index < deviceCount; index++) {
		IMMDevice* endpoint = NULL;
		hr = pEndpointCollector->Item(index, &endpoint);
		if (FAILED(hr)) {
			printf("Unable to get endPoint item: %x\n", hr);
			return false;
		}

		this->endpoints.push_back(new AudioDevice(endpoint, endpointType_e::outputEndpoint));

	}

	return true;

}


bool AudioManager::setInputEndpoints(IMMDeviceEnumerator* enumerator) {

	HRESULT hr = NULL;
	UINT deviceCount = 0;
	IMMDeviceCollection* pEndpointCollector = NULL;
	
	if (!enumerator) {
		return false;
	}

	hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pEndpointCollector);
	if (FAILED(hr)) {

		printf("Error: EnumAudioEndpoints(): %d\n", hr);
		return false;
	}

	hr = pEndpointCollector->GetCount(&deviceCount);
	if (FAILED(hr))
	{
		printf("Unable to get endPoint count: %x\n", hr);
		return false;
	}

	for (auto index = 0; index < deviceCount; index++) {
		IMMDevice* endpoint = NULL;
		hr = pEndpointCollector->Item(index, &endpoint);
		if (FAILED(hr)) {
			printf("Unable to get endPoint item: %x\n", hr);
			return false;
		}

		this->endpoints.push_back(new AudioDevice(endpoint, endpointType_e::inputEndpoint));

	}

	return true;

}
