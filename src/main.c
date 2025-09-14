#define SDL_MAIN_USE_CALLBACKS 1 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

////////////////////////////
/// Variable Declaration ///
////////////////////////////

// graphics
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define TILE_SIZE 20

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

// movement
int direction;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// snake
SDL_Point* snake;
size_t snakeSize, snakeCap;



/////////////////
/// Functions ///
/////////////////
void addToSnake(SDL_Point point) {
	if (snakeSize == snakeCap) {
		size_t newCap = (snakeCap == 0) ? 1 : snakeCap * 2;
		SDL_Point* tmp_snake = (SDL_Point*) realloc(snake, newCap * sizeof(SDL_Point));

		if (tmp_snake == NULL) {
			free(snake);
			exit(1);
		}

		snake = tmp_snake;
		snakeCap = newCap;
	}

	snake[snakeSize] = point;
	snakeSize ++;
}

SDL_Point randomTile() {
	return (SDL_Point) { 
		rand() % (WINDOW_WIDTH / TILE_SIZE - 1),
		rand() % (WINDOW_HEIGHT / TILE_SIZE - 1)
	};
}

void resetGame() {
	snakeSize = 0;
	addToSnake(randomTile());
	direction = (rand() % 4);
}

/////////////////
/// Callbacks ///
/////////////////
// no need for a main function loop anymore

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
				// Quitting the game
				case SDL_SCANCODE_ESCAPE:
				case SDL_SCANCODE_Q:
					return SDL_APP_SUCCESS;
				// moving the snake
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_K:
					direction = UP;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_J:
					direction = DOWN;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_H:
					direction = LEFT;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_L:
					direction = RIGHT;
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
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}

