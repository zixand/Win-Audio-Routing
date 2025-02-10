#include "AudioManager.h"


AudioManager::AudioManager() {

	this->updateEndpoint();

}

AudioManager::~AudioManager() {

}


bool AudioManager::createSession(AudioDevice* device) {
	
	HRESULT hr = NULL;
	WAVEFORMATEX* pwfx = NULL;
	IAudioClient* pAudioClient = NULL;

	
	hr = device->getEndpoit()->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		printf("Error: Activate(): %d\n", hr);
		return false;
	}

	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		std::cerr << "Errore durante il recupero del formato di mixaggio." << std::endl;
		pAudioClient->Release();
		return false;
	}

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, pwfx, NULL);
	if (FAILED(hr)) {
		std::cerr << "Errore durante l'inizializzazione dell'audio client." << std::endl;
		pAudioClient->Release();
		return false;
	}



	hr = pAudioClient->Start();
	if (FAILED(hr)) {
		std::cerr << "Errore durante l'avvio dell'audio client." << std::endl;
		pAudioClient->Release();
		return false;
	}

	updateEndpoint();

	return true;
}


void AudioManager::updateEndpoint() {


	HRESULT hr = CoInitialize(NULL);
	IMMDevice* pDevice = NULL;
	IAudioClient* pAudioClient = NULL;
	IMMDeviceEnumerator* pEnumerator = NULL;
	
	
	if (FAILED(hr)) {
		std::cerr << "Errore durante l'inizializzazione di COM." << std::endl;
		return;
	}
	
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&this->pEnumerator	);
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
	
#ifdef _DEBUG
	for (auto session : this->session) {
		std::cout << "Session: " << session->getDevice()->getFriendlyName() << std::endl;
		std::cout << "PID: " << session->getProcessPid() << std::endl;
		std::cout << "State " << session->getSessionState() << std::endl;
		std::cout << "SessionId " << session->getSessionIdentifier() << std::endl << std::endl;
	}
#endif


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


std::map<int, std::string> AudioManager::getActiveProcess() {
	
	std::map<int, std::string> tempProcMap;
	wchar_t processName[MAX_PATH];
	HANDLE hProcess = NULL;
	HMODULE hMod;
	DWORD cbNeeded;

	for (auto proc : this->session) {

		if (proc->getSessionState() == AudioSessionStateActive) { //in futuro controllare che proc->getDevice()->getFriendlyName(); sia una interface vuota o una specificata
		
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc->getProcessPid());

			if (hProcess != NULL) {

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
					GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(char));
				}

				size_t converted = 0;
				size_t len = wcslen(processName) + 1;
				char* cstr = new char[len];
				wcstombs_s(&converted, cstr, len, processName, _TRUNCATE);
				std::string str(cstr);
				delete[] cstr;

				tempProcMap.insert(std::make_pair(proc->getProcessPid(), str));

				// Rilascia l'handle al processo
				CloseHandle(hProcess);
		
			}
		}
	}

	return tempProcMap;;

}

