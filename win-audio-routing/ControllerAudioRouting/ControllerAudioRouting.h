#ifndef CONTROLLERAUDIOROUTING_H
#define CONTROLLERAUDIOROUTING_H

#include "pch.h"
#include "ModelAudioRouting.h"
#include "NodeEditorWidget.h"

class GraphicEngine;

class ControllerAudioRouting {

public:
	ControllerAudioRouting(GraphicEngine *engine);
	~ControllerAudioRouting();

private:
	GraphicEngine* engine;
	ModelAudioRouting* model;

	void init();


};

#endif // CONTROLLERAUDIOROUTING_H
