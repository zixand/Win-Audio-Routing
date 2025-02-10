#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include "pch.h"
#include "WidgetControl.h"
#include "ControllerAudioRouting.h"

class GraphicEngine {

public:
	GraphicEngine();
	~GraphicEngine();
	bool init();
	void graphicLoop();
	bool getExecutionState();
	bool isStarted();
	void setExecutionState(bool state);
	unsigned long long int addWidget(WidgetControl *widget);
	void removeWidget(unsigned long long int id);
	

private:
	ImGuiIO io;
	SDL_Window* window;
	uint32_t window_flags;
	std::pair<int, int> window_size;
	SDL_GLContext gl_context;
	SDL_Event event;
	bool executionState;
	bool started;
	std::map< int , WidgetControl*> widgets;
	ControllerAudioRouting* controller;

	bool setSDL_Attributes();

};

#endif // GRAPHICENGINE_H
