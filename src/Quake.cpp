//
// Quake map viewer
//
// Author: Johan Gardhage <johan.gardhage@gmail.com>
//
#include <SDL2/SDL.h>
#include <getopt.h> // getopt_long
#include "Map.h"
#include "World.h"
#include "Camera.h"

enum Parameters
{
	WINDOW = 1,
	FULLWINDOW = 2,
	FULLSCREEN = 4,
	SHOWCURSOR = 8
};

int ParseArguments(int argc, char *argv[], int param)
{
	int parameters = param;
	static struct option long_options[] = {
		{"help",       no_argument, 0, 'h'},
		{"window",     no_argument, 0, 'w'},
		{"fullwindow", no_argument, 0, 0},
		{"fullscreen", no_argument, 0, 'f'},
		{"showcursor", no_argument, 0, 'c'},
		{"nocursor",   no_argument, 0, 0},
		{0,            0,           0, 0}
	};
	bool usage = false;
	int c;
	int option_index = 0;
	while ((c = getopt_long(argc, argv, ":hwfc", long_options, &option_index)) != -1) {
		switch (c) {
		case 0:
			if (strcmp("fullwindow", long_options[option_index].name) == 0) {
				parameters |= FULLWINDOW;
				parameters &= ~(WINDOW | FULLSCREEN);
			} else if (strcmp("nocursor", long_options[option_index].name) == 0) {
				parameters &= ~SHOWCURSOR;
			}
			break;
		case 'h':
			usage = true;
			break;
		case 'w':
			parameters |= WINDOW;
			parameters &= ~(FULLWINDOW | FULLSCREEN);
			break;
		case 'f':
			parameters |= FULLSCREEN;
			parameters &= ~(WINDOW | FULLWINDOW);
			break;
		case 'c':
			parameters |= SHOWCURSOR;
			break;
		case '?':
			usage = true;
			printf("unrecognized option '%s'\n", argv[optind - 1]);
			break;
		default:
			usage = true;
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}
	if (optind < argc) {
		usage = true;
		printf ("non-option ARGV-elements: ");
		while (optind < argc) {
			printf("%s ", argv[optind++]);
		}
		printf("\n");
	}
	if (usage) {
		printf("Usage: %s [OPTION]...\n\n", /***basename*/(argv[0]));
		printf("Options:\n");
		printf(" -h, --help         Display this text and exit\n");
		printf(" -w, --window       Render in a window\n");
		printf("     --fullwindow   Render in a fullscreen window\n");
		printf(" -f, --fullscreen   Render in fullscreen\n");
		printf(" -c, --showcursor   Show mouse cursor\n");
		printf("     --nocursor     Hide mouse cursor\n");
		exit(1);
	}
	return parameters;
}

int SDL_main(int argc, char *argv[])
{
	Map map;
	World world;
	Camera camera;

	// Parse arguments
	int parameters = ParseArguments(argc, argv, FULLSCREEN);

	// Load map
	if (!map.Initialize("assets/start.bsp", "assets/palette.lmp")) {
		printf("[ERROR] Quake::main() Unable to initialize map\n");
		return -1;
	}

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("[ERROR] Quake::main() Unable to initialize SDL\n");
		return -1;
	}

	// Get current display mode
	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
		printf("[ERROR] Quake::main() Unable to get current display mode\n");
		return -1;
	}
	if (parameters & WINDOW) {
		dm.w = 320;
		dm.h = 200;
	}

	// Cursor and mouse mode
	if (parameters & SHOWCURSOR) {
		SDL_ShowCursor(1);
	} else {
		SDL_ShowCursor(0);

		// Set relative mouse mode
		if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0) {
			printf("[ERROR] Quake::main() Unable to set mouse mode\n");
			return -1;
		}
	}

	// Create window
	Uint32 flags = SDL_WINDOW_OPENGL;
	if (parameters & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	SDL_Window *window = SDL_CreateWindow("Quake!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, flags);
	if (!window) {
		printf("[ERROR] Quake::main() Unable to create window\n");
		return -1;
	}

	// Create our opengl context and attach it to our window
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		printf("[ERROR] Quake::main() Unable to create context\n");
		return -1;
	}

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	// Initialize world
	if (!world.Initialize(&map, dm.w, dm.h)) {
		printf("[ERROR] Quake::main() Unable to initialize world\n");
		return -1;
	}

	bool loop = true;
	while (loop) {
		SDL_PumpEvents();

		const Uint8 * keys = SDL_GetKeyboardState(NULL);

		// Handle keyboard events
		if (SDL_QuitRequested()) {
			loop = false;
			break;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_ESCAPE)]) {
			loop = false;
			break;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_q)]) {
			loop = false;
			break;
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_w)]) {
			camera.MoveForward();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_UP)]) {
			camera.MoveForward();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_s)]) {
			camera.MoveBackward();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_DOWN)]) {
			camera.MoveBackward();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_RIGHT)]) {
			camera.TurnRight();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_LEFT)]) {
			camera.TurnLeft();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_d)]) {
			camera.StrafeRight();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_a)]) {
			camera.StrafeLeft();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_PAGEUP)]) {
			camera.PitchUp();
		}
		if (keys[SDL_GetScancodeFromKey(SDLK_PAGEDOWN)]) {
			camera.PitchDown();
		}

		// Handle mouse events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_MOUSEMOTION) {
				camera.Yaw(event.motion.xrel);
				camera.Pitch(event.motion.yrel);
			}
		}

		// Update camera position
		camera.UpdatePosition();

		// Draw the scene
		world.DrawScene(&camera);
		SDL_GL_SwapWindow(window);
	}

	// Delete our OpengL context
	SDL_GL_DeleteContext(context);

	// Destroy our window
	SDL_DestroyWindow(window);

	// Shutdown SDL
	SDL_Quit();

	return 0;
}
//==========================================
#if defined(_UNIX)

int main(int argc, char *argv[])
	{ return SDL_main(argc, argv); }

#endif
//==========================================

/// https://github.com/arkenidar/sdl-cb/blob/master/main-sdl.c

// . sdl-cross-compile.sh
// wine sdl-app.exe

#if defined(_WIN64)

#include <windows.h>

int WINAPI WinMain( HINSTANCE hInstance,    // HANDLE TO AN INSTANCE.  This is the "handle" to YOUR PROGRAM ITSELF.
                    HINSTANCE hPrevInstance,// USELESS on modern windows (totally ignore hPrevInstance)
                    LPSTR szCmdLine,        // Command line arguments.  similar to argv in standard C programs
                    int iCmdShow )          // Start window maximized, minimized, etc.
{
  //char* argv[]={"main-sdl","--window"};
  //return main(sizeof(argv)/sizeof(argv[0]),argv);

  // https://stackoverflow.com/questions/27363851/pass-winmain-or-wwinmain-arguments-to-normal-main

	int returned = 0;

  	int w_argc = 0;
	LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
	if (w_argv)
	{
		char** my_argv = new char*[w_argc];
		int my_argc = 0;

		for (int i = 0; i < w_argc; ++i)
		{
			int w_len = lstrlenW(w_argv[i]);
			int len = WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
			my_argv[my_argc] = new char[len+1];
			WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, my_argv[my_argc], len+1, NULL, NULL);
			++my_argc;
		}

		returned = SDL_main(my_argc, my_argv);

		for (int i = 0; i < my_argc; ++i)
			delete[] my_argv[i];
		delete[] my_argv;

		LocalFree(w_argv);
	}
	return returned;
}

#endif