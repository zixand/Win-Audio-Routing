#include "graphicEngine.h"


GraphicEngine::GraphicEngine() {

	this->window_flags = 0;
	this->window_size = std::make_pair(1280, 720);
	this->window = NULL;
	this->gl_context = nullptr;
	this->executionState = false;
	this->started = false;
		
}

GraphicEngine::~GraphicEngine() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

void GraphicEngine::setExecutionState(bool state) {
	this->executionState = state;
}

bool GraphicEngine::getExecutionState() {
	return this->executionState;
}

bool GraphicEngine::isStarted() {
	return this->started;
}

void GraphicEngine::graphicLoop() {
	this->setExecutionState(true);
	while (this->isStarted() && this->getExecutionState()) {
		while (SDL_PollEvent(&this->event)) {

			ImGui_ImplSDL3_ProcessEvent(&this->event);

			if (this->event.type == SDL_EVENT_QUIT) {
				this->setExecutionState(false);
			}

		}

		if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
		{
			SDL_Delay(10);
			continue;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		for (auto widget : this->widgets) {
			widget.second->render();
		}
		
		ImGui::Render();
		glViewport(0, 0, (int)this->io.DisplaySize.x, (int)this->io.DisplaySize.y);
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(this->window);
	}

	this->setExecutionState(false);
}

bool GraphicEngine::init() {

	if (!this->isStarted()) {

		//sdl setup
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
		{
			printf("Error: SDL_Init(): %s\n", SDL_GetError());
			return false;
		}

		if(!this->setSDL_Attributes())
			return false;

		SDL_GL_MakeCurrent(window, gl_context);
		SDL_GL_SetSwapInterval(1); // Enable vsync
		SDL_ShowWindow(window);

		//Imgui setup
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		this->io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		this->io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		this->io = ImGui::GetIO();
		// Setup Platform/Renderer backends
		ImGui_ImplSDL3_InitForOpenGL(this->window, this->gl_context);
		ImGui_ImplOpenGL3_Init();

		this->controller = new ControllerAudioRouting(this);

		this->started = true;
		return true;
	}

}

bool GraphicEngine::setSDL_Attributes() {
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100 (WebGL 1.0)
	const char* glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
	// GL ES 3.0 + GLSL 300 es (WebGL 2.0)
	const char* glsl_version = "#version 300 es";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	this->window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
	this->window = SDL_CreateWindow("Windows Audio Routing", this->window_size.first, this->window_size.second, window_flags);
	if (this->window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	this->gl_context = SDL_GL_CreateContext(window);
	if (this->gl_context == nullptr)
	{
		printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
		return -1;
	}

	return true;

}

 unsigned long long int GraphicEngine::addWidget(WidgetControl *widget) {
	
	unsigned long long int id = std::rand();
	
	while (this->widgets.find(id) != this->widgets.end()) {
		id = std::rand();
	}
	this->widgets.insert(std::pair<int, WidgetControl*>(id, widget));
	
	return id;

}
void GraphicEngine::removeWidget(unsigned long long int id) {
	
	if (this->widgets.find(id) != this->widgets.end()) {
		this->widgets.erase(id);
	}

}

