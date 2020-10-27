#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Application.h"
#include <stdexcept>
#include <string>

Application::Application() {
	initializeSDL();
}

Application::~Application() {
	SDL_DestroyWindow(window);
	SDL_FreeSurface(screenSurface);
	SDL_Quit;
	window = nullptr;
	screenSurface = nullptr;
}

void Application::initializeSDL() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error("SDL could not init.");
	}
	// use opengl 2.1
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CreateWindow("SDL2 Project", SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 
								SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(window == nullptr) {
		throw std::runtime_error("could not create window");
	}

	// create gl context
	
	gContext = SDL_GL_CreateContext(window);
	if (gContext == nullptr) {
		std::string errorString = SDL_GetError();
		errorString = "opengl context could not be created: " + errorString;
		throw std::runtime_error(errorString);
	}

	if(SDL_GL_SetSwapInterval(1) < 0) {
		throw std::runtime_error("unable to set vsync");
	}
	initGL();

}

void Application::run() {
	bool quit = false;
	while(!quit) {
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_q:
					gViewPortMode++;
					if(gViewPortMode > VIEWPORT_MODE_RADAR) {
						gViewPortMode = VIEWPORT_MODE_FULL;
					}
					break;
				case SDLK_e:
					if(gProjectionScale == 1.0f) {
						// zoom out
						gProjectionScale = 2.0f;
					} else if(gProjectionScale == 2.0f) {
						//zoom in
						gProjectionScale = 0.5f;
					} else if(gProjectionScale == 0.5f) {
						// default
						gProjectionScale = 1.0f;
					}
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0.0, WIDTH * gProjectionScale, HEIGHT * gProjectionScale,
							0.0f, 1.0f, -1.0f);
					break;
				default:
					break;
				}
			}
		}
		render();
		SDL_GL_SwapWindow(window);
	}
}

void Application::initGL() {
	GLenum error = GL_NO_ERROR;
	
	// set the viewport
	glViewport(0.0f, 0.0f, WIDTH, HEIGHT);

	//set up projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0f, WIDTH, HEIGHT, 0.0f, 1.0f, -1.0f);

	error = glGetError();
	if(error != GL_NO_ERROR) {
		throw std::runtime_error("error initializing projection matrix");
	}

	// setup model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	error = glGetError();
	if(error != GL_NO_ERROR) {
		throw std::runtime_error("error initializing modelview");
	}
	// init clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error("failed to set clear color.");
	}

}

void Application::update() {
	// no per frame update needed at this time
}

void Application::render() {
	// clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// reset modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// move to center of the screen
	glTranslatef(WIDTH / 2.0f, HEIGHT / 2.0f, 0.0f);

	switch(gViewPortMode) {
	case VIEWPORT_MODE_FULL:
		glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(-WIDTH / 2.0f, -HEIGHT / 2.0f);
			glVertex2f(WIDTH / 2.0f, -HEIGHT / 2.0f);
			glVertex2f(WIDTH / 2.0f, HEIGHT / 2.0f);
			glVertex2f(-WIDTH / 2.0f, HEIGHT / 2.0f);
		glEnd();
		break;
	case VIEWPORT_MODE_HALF_CENTER:
		glViewport(WIDTH / 4.0f, HEIGHT / 4.0f, WIDTH / 2.0f, HEIGHT / 2.0f);
		glBegin(GL_QUADS);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex2f(-WIDTH / 2.0f, -HEIGHT / 2.0f);
			glVertex2f(WIDTH / 2.0f, -HEIGHT / 2.0f);
			glVertex2f(WIDTH / 2.0f, HEIGHT / 2.0f);
			glVertex2f(-WIDTH / 2.0f, HEIGHT / 2.0f);
		glEnd();
		break;
	case VIEWPORT_MODE_HALF_TOP:
		glViewport(WIDTH / 4.0f, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT / 2.0f);
		glBegin( GL_QUADS );
            glColor3f( 0.f, 0.f, 1.f );
            glVertex2f( -WIDTH / 2.f, -HEIGHT / 2.f );
            glVertex2f(  WIDTH / 2.f, -HEIGHT / 2.f );
            glVertex2f(  WIDTH / 2.f,  HEIGHT / 2.f );
            glVertex2f( -WIDTH / 2.f,  HEIGHT / 2.f );
        glEnd();
		break;
	case VIEWPORT_MODE_QUAD:
		//Bottom left red quad
        glViewport( 0.f, 0.f, WIDTH / 2.f, HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f );
            glVertex2f( -WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f,  HEIGHT / 4.f );
            glVertex2f( -WIDTH / 4.f,  HEIGHT / 4.f );
        glEnd();

        //Bottom right green quad
        glViewport( WIDTH / 2.f, 0.f, WIDTH / 2.f, HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 0.f );
            glVertex2f( -WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f,  HEIGHT / 4.f );
            glVertex2f( -WIDTH / 4.f,  HEIGHT / 4.f );
        glEnd();

        //Top left blue quad
        glViewport( 0.f, HEIGHT / 2.f, WIDTH / 2.f, HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 0.f, 0.f, 1.f );
            glVertex2f( -WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f,  HEIGHT / 4.f );
            glVertex2f( -WIDTH / 4.f,  HEIGHT / 4.f );
        glEnd();

        //Top right yellow quad
        glViewport( WIDTH / 2.f, HEIGHT / 2.f, WIDTH / 2.f, HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 1.f, 0.f );
            glVertex2f( -WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f, -HEIGHT / 4.f );
            glVertex2f(  WIDTH / 4.f,  HEIGHT / 4.f );
            glVertex2f( -WIDTH / 4.f,  HEIGHT / 4.f );
        glEnd();
		break;
	case VIEWPORT_MODE_RADAR:
		// full sized viewport
		glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex2f(-WIDTH / 8.0f, -HEIGHT / 8.0f);
			glVertex2f(WIDTH / 8.0f, -HEIGHT / 8.0f);
			glVertex2f(WIDTH / 8.0f, HEIGHT / 8.0f);
			glVertex2f(-WIDTH / 8.0f, HEIGHT / 8.0f);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(-WIDTH / 16.0f, -HEIGHT / 16.0f);
			glVertex2f(WIDTH / 16.0f, -HEIGHT / 16.0f);
			glVertex2f(WIDTH / 16.0f, HEIGHT /	16.0f);
			glVertex2f(-WIDTH / 16.0f, HEIGHT / 16.0f);
		glEnd();

		// radar
		glViewport(WIDTH / 2.0f, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT / 2.0f);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);                                            
			glVertex2f(-WIDTH / 8.0f, -HEIGHT / 8.0f);                              
			glVertex2f(WIDTH / 8.0f, -HEIGHT / 8.0f);                               
			glVertex2f(WIDTH / 8.0f, HEIGHT / 8.0f);                                
			glVertex2f(-WIDTH / 8.0f, HEIGHT / 8.0f);                               
			glColor3f(0.0f, 0.0f, 0.0f);                                            
			glVertex2f(-WIDTH / 16.0f, -HEIGHT / 16.0f);                            
			glVertex2f(WIDTH / 16.0f, -HEIGHT / 16.0f);                             
			glVertex2f(WIDTH / 16.0f, HEIGHT /  16.0f);                             
			glVertex2f(-WIDTH / 16.0f, HEIGHT / 16.0f);
		glEnd();
		break;
	default:
		break;
	}
}
