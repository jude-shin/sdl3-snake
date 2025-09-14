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

// called at the beginning
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	return SDL_APP_CONTINUE;
}

// handle events like key presses
SDL_AppResult SDL_AppEvent(void **appstate, SDL_Event *event) {
	return SDL_APP_CONTINUE;
}

// handles the main loop (animation rendering and game logic)
SDL_AppResult SDL_AppIterate(void **appstate) {
	return SDL_APP_CONTINUE;
}

// called at the end, and handles some of the exits
void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}

