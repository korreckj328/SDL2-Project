#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

enum ViewPortMode {
	VIEWPORT_MODE_FULL,
	VIEWPORT_MODE_HALF_CENTER,
	VIEWPORT_MODE_HALF_TOP,
	VIEWPORT_MODE_QUAD,
	VIEWPORT_MODE_RADAR
};

class Application {
public:
	Application();
	~Application();
	void run();
private:
	const int WIDTH = 640;
	const int HEIGHT = 480;
	const int COLOR_MODE_CYAN = 0;
	const int COLOR_MODE_MULTI = 1;

	int gColorMode = COLOR_MODE_CYAN;
	GLfloat gProjectionScale = 1.0f;

	SDL_Window *window = nullptr;
	SDL_Surface *screenSurface = nullptr;
	SDL_Event event;
	void initializeSDL();
	void initGL();
	void update();
	void render();
	SDL_GLContext gContext;
	bool gRenderQuad = true;
	int gViewPortMode = VIEWPORT_MODE_FULL;
};
