#define SDL_MAIN_USE_CALLBACKS 1 

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/////////////////
/// CALLBACKS ///
/////////////////
// no need for a main function loop anymore
// SDL_Log("Couldn't initalize SDL: %s", SDL_GetError());

// called at the beginning
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	// check to see that sdl initalized 
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initalize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	// try and render the window given the title, width, height, widnow pointer, and renderer pointer
	if (!SDL_CreateWindowAndRenderer("Snake", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		SDL_Log("Couldn't initalize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

// handle events like key presses
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	switch(event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		case SDL_EVENT_KEY_DOWN:
			switch(event->key.scancode) {
				case SDL_SCANCODE_ESCAPE:
				case SDL_SCANCODE_Q:
					return SDL_APP_SUCCESS;
			}
			break;
	}
	return SDL_APP_CONTINUE;
}

// handles the main loop (animation rendering and game logic)
SDL_AppResult SDL_AppIterate(void *appstate) {
	// set the color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	// set the color to everywhere
	SDL_RenderClear(renderer);
	// show what we just did to the renderer
	SDL_RenderPresent(renderer);
	
	return SDL_APP_CONTINUE;
}

// called at the end, and handles some of the exits
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}

