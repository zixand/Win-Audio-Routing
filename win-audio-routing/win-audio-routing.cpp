#include "win-audio-routing.h"
#include "AudioManager.h"

int main() {

	GraphicEngine* engine = new GraphicEngine();
	engine->init();
	engine->graphicLoop();
	delete engine;

	return 0;
}
