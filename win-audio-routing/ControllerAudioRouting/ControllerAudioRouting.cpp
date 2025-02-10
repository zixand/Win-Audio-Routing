#include "ControllerAudioRouting.h"
#include "graphicEngine.h"

ControllerAudioRouting::ControllerAudioRouting(GraphicEngine *engine) {
	this->engine = engine;
	this->model = new ModelAudioRouting();
	this->init();
}

ControllerAudioRouting::~ControllerAudioRouting() {
	
	this->engine = nullptr;
	delete this->model;

}

void ControllerAudioRouting::init() {

	unsigned long long int id = this->engine->addWidget(new NodeEditorWidget());

}