#ifndef AUDIO_PROCESS_H
#define AUDIO_PROCESS_H
//
//#include "pch.h"
//
//class AudioSession;
//class AudioDevice;
//
//
//class AudioProcess {
//
//public:	
//	AudioProcess(AudioDevice*);
//	~AudioProcess();
//	void updatePipeMap(); //passiamo l'intera mappa o solo l'aggiunta?
//	void getBufferProcess(AudioSession);
//	void copyProcessBuffer();
//	void addPipe(AudioSession);
//	void removePipe(AudioSession);
//	void startDataStream();
//	void initPipeMap();
//
//private:
//	
//	std::map<wchar_t*, bool> pipeMap;
//	AudioDevice processEndpoint;
//	std::mutex pipeMapMutex;
//
//	IActivateAudioInterfaceCompletionHandler* completionHandler; //da implementare
//
//};
//
//DWORD WINAPI thread_handler(LPVOID lpParameter);

#endif