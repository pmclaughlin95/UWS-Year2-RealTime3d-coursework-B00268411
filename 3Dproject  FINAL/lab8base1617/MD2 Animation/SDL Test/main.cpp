#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "Renderer.h"
#include "world.h"
using namespace std;
#define DEG_TO_RADIAN 0.017453293

SDL_Window * createWindow(SDL_GLContext &context) {
	SDL_Window * window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
		rt3d::exitFatalError("Unable to initialize SDL");

	

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); 

													  
	window = SDL_CreateWindow("RT3D Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)							
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); 
	SDL_GL_SetSwapInterval(1);				
	return window;
}

int main(int argc, char *argv[])
{
	
	SDL_Window * hWindow;					
	SDL_GLContext glContext;				
	hWindow = createWindow(glContext);		

											
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (GLEW_OK != error) {					
		std::cout << "glewInit failed, aborting." << endl;
		exit(1);
	}
	cout << glGetString(GL_VERSION) << endl;



	//Our world object
	world* ourWorld = new world();

	ourWorld->running = true;					
					
	while (ourWorld->running) {						// the game loop

		ourWorld->gameLoop(hWindow);
		
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(hWindow);
	SDL_Quit();
	return 0;


	
}