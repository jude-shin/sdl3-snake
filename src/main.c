#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "global.h"

bool init() {
	bool success = SDL_Init(SDL_INIT_VIDEO);
	if (!success) {
		SDL_Log("Initialization failed! Error: %s\n", SDL_GetError());
	}
	return success;
}

bool makeWindow() {
	gWindow = SDL_CreateWindow("Snake", kScreenWidth, kScreenHeight, 0);

	if (gWindow == NULL) {
		SDL_Log("Window creation failed! Error: %s\n", SDL_GetError());
		return false;
	}

	gScreenSurface = SDL_GetWindowSurface(gWindow);
	
	return true;
}

void end() {
	SDL_Quit();
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
}

void loop() {
	bool quit = false;
	SDL_Event e;
	SDL_zero(e);

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}

		SDL_FillSurfaceRect(
				gScreenSurface, 
				NULL, 
				SDL_MapSurfaceRGB(gScreenSurface, bgColorR, bgColorG, bgColorB));

		SDL_BlitSurface(
				gHelloWorld,
				NULL,
				gScreenSurface, 
				NULL);

		SDL_UpdateWindowSurface(gWindow);
	}
}


int main(int argc, char *argv[]) {
	bool success = true;

	if (!init()) {
		return -1;
	}

	if (!makeWindow()) {
		return -2;
	}

	loop();

	end();

	return 0;
}
