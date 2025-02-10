#include "AudioDevice.h"


AudioDevice::AudioDevice(IMMDevice* endpoint, endpointType_e type) {
	
	this->endpoint = endpoint;
	this->type = type;

}	

AudioDevice::~AudioDevice() {

	this->endpoint = nullptr;
}	

std::string AudioDevice::getFriendlyName() {

	HRESULT hr = NULL;
	LPWSTR name = NULL;
	wchar_t deviceName[128];

	IPropertyStore* propertyStore;
	hr = this->endpoint->OpenPropertyStore(STGM_READ, &propertyStore);
	if (FAILED(hr))
	{
		printf("Unable to open device property store: %x\n", hr);
		return NULL;
	}

	PROPVARIANT friendlyName;
	PropVariantInit(&friendlyName);
	hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
	propertyStore->Release();
	propertyStore = nullptr;
	if (FAILED(hr))
	{
		printf("Unable to retrieve friendly name for device : %x\n", hr);
		return NULL;
	}

	hr = StringCbPrintf(deviceName, sizeof(deviceName), L"%s", friendlyName.vt != VT_LPWSTR ? L"Unknown" : friendlyName.pwszVal);
	if (FAILED(hr))
	{
		printf("Unable to format friendly name for device : %x\n", hr);
		return NULL;
	}

	size_t converted = 0;
	size_t len = wcslen(deviceName) + 1;
	char* cstr = new char[len];
	wcstombs_s(&converted, cstr, len, deviceName, _TRUNCATE);
	std::string str(cstr);
	delete[] cstr;
	return str;

}

IMMDevice* AudioDevice::getEndpoit() {
	return this->endpoint;
}

