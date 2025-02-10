//#include "AudioProcess.h"
//#include "AudioSession.h"
//
//AudioProcess::AudioProcess(AudioDevice* endpoint) {
//
//	this->pipeMap.insert(std::make_pair(endpoint->getSessionIdentifier(), false));
//	this->processEndpoint = *endpoint;
//
//}
//
//AudioProcess::~AudioProcess() {
//}
//
//void AudioProcess::updatePipeMap() {
//}
//
//void AudioProcess::getBufferProcess(AudioSession session) {
//	
//	AUDIOCLIENT_ACTIVATION_PARAMS audioclientActivationParams = {};
//	audioclientActivationParams.ActivationType = AUDIOCLIENT_ACTIVATION_TYPE_PROCESS_LOOPBACK;
//	audioclientActivationParams.ProcessLoopbackParams.ProcessLoopbackMode = PROCESS_LOOPBACK_MODE_INCLUDE_TARGET_PROCESS_TREE;
//	audioclientActivationParams.ProcessLoopbackParams.TargetProcessId = session.getProcessPid();
//
//	PROPVARIANT activateParams = {};
//	activateParams.vt = VT_BLOB;
//	activateParams.blob.cbSize = sizeof(audioclientActivationParams);
//	activateParams.blob.pBlobData = (BYTE*)&audioclientActivationParams;
//
//	//wil::com_ptr_nothrow<IActivateAudioInterfaceAsyncOperation> asyncOp;
//
//	///ActivateAudioInterfaceAsync(this->processEndpoint.getSessionIdentifier(), __uuidof(IAudioClient), &activateParams, this->completionHandler, &asyncOp));
//
//}
//
//void AudioProcess::copyProcessBuffer() {
//}
//
//
//DWORD WINAPI thread_handler(LPVOID lpParameter) {
//
//	//USBConnection* usbConnection = (USBConnection*)lpParameter;
//
//	
//	return 0;
//
//}