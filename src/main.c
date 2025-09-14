#define SDL_MAIN_USE_CALLBACKS 1 

/*  
TODO: 
seperate code into different files
allow for the snake to have different macro colors
allow for the background to have different macro colors
kill the snake at the border
show the score on the screen
change the number of apples that are on the screen
macro for frame rate
condense the two addTo Functions into one
macro for number of food available
make a better collision system for the snake (not just iterating over all of the snake values)
seperate the game logic and the rendering logic?
*/

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
SDL_FRect drawRect;
int direction;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// snake
SDL_Point *snake, *food;
size_t snakeSize, snakeCap, foodSize, foodCap;
bool grow;



/////////////////
/// Functions ///
/////////////////
void addToSnake(SDL_Point point) {
	if (snakeSize == snakeCap) {
		size_t newCap = (snakeCap == 0) ? 1 : snakeCap * 2;
		SDL_Point* tmp_snake = (SDL_Point*) realloc(snake, newCap * sizeof(SDL_Point));

		if (tmp_snake == NULL) {
			free(snake);
			free(food);
			exit(1);
		}

		snake = tmp_snake;
		snakeCap = newCap;
	}

	snake[snakeSize] = point;
	snakeSize++;
}

void addToFood(SDL_Point point) {
	if (foodSize == foodCap) {
		size_t newCap = (foodCap == 0) ? 1 : foodCap * 2;
		SDL_Point* tmp_food = (SDL_Point*) realloc(food, newCap * sizeof(SDL_Point));

		if (tmp_food == NULL) {
			free(snake);
			free(food);
			exit(1);
		}

		food = tmp_food;
		foodCap = newCap;
	}

	food[foodSize] = point;
	foodSize++;
}

SDL_Point randomTile() {
	return (SDL_Point) { 
		rand() % (WINDOW_WIDTH / TILE_SIZE - 1),
		rand() % (WINDOW_HEIGHT / TILE_SIZE - 1)
	};
}

void resetGame() {
	snakeSize = foodSize = 0;

	// init food size
	for (int i = 0; i < 3; i++) {
		addToFood(randomTile());
	}

	addToSnake(randomTile());
	direction = (rand() % 4);
	grow = false;
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

	drawRect.w = drawRect.h = TILE_SIZE;
	srand(time(NULL));

	resetGame();

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
				// Resetting the game
				case SDL_SCANCODE_R:
					resetGame();
					break;
				// moving the snake
				case SDL_SCANCODE_UP:
				case SDL_SCANCODE_K:
					direction = UP;
					break;
				case SDL_SCANCODE_DOWN:
				case SDL_SCANCODE_J:
					direction = DOWN;
					break;
				case SDL_SCANCODE_LEFT:
				case SDL_SCANCODE_H:
					direction = LEFT;
					break;
				case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_L:
					direction = RIGHT;
					break;
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
	
	// render the food
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (size_t i = 0; i < foodSize; i++) {
		drawRect.x = food[i].x * TILE_SIZE;
		drawRect.y = food[i].y * TILE_SIZE;
		SDL_RenderFillRect(renderer, &drawRect);

		// if the food ends up getting "eaten" (or collides with the head of the snake) 
		// pick a new spot for the food
		if ((snakeSize > 0) && (food[i].x == snake[0].x) && (food[i].y == snake[0].y)) {
			food[i] = randomTile();
			grow = true;
		}
	}

	// collision detection with the snake
	if (grow) addToSnake(snake[snakeSize]);
	grow = false;
	for (size_t i = snakeSize; i > 0; i--) { // discludes the head
		if ((snakeSize > 0) && (snake[0].x == snake[i].x) && (snake[0].y == snake[i].y)) {
			printf("You died. \nTotal Size: %ld\n", snakeSize);
			resetGame();
		}
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	// render the snake body	
	SDL_SetRenderDrawColor(renderer, 0, 200, 0, SDL_ALPHA_OPAQUE);
	for (size_t i = snakeSize; i > 0; i--) {
		drawRect.x = snake[i].x * TILE_SIZE;
		drawRect.y = snake[i].y * TILE_SIZE;
		SDL_RenderFillRect(renderer, &drawRect);
	}
	
	if (snakeSize > 0) {
		// move the snake
		if (direction == UP) { snake[0].y--; }
		if (direction == DOWN) { snake[0].y++; }
		if (direction == LEFT) { snake[0].x--; }
		if (direction == RIGHT) { snake[0].x++; }

		// don't have the snake collide with the edges
		if(snake[0].x	>= (WINDOW_WIDTH / TILE_SIZE)) { snake[0].x = 0; }
		if(snake[0].y	>= (WINDOW_HEIGHT / TILE_SIZE)) { snake[0].y = 0; }
		if(snake[0].x < 0) { snake[0].x = (WINDOW_WIDTH / TILE_SIZE) -1; }
		if(snake[0].y < 0) { snake[0].y = (WINDOW_HEIGHT / TILE_SIZE) -1; }

		// render the snake
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
		drawRect.x = snake[0].x * TILE_SIZE;
		drawRect.y = snake[0].y * TILE_SIZE;
		SDL_RenderFillRect(renderer, &drawRect);
	}


	// SDL_Log("Snake Length: %ld", snakeSize);
	// SDL_Log("Head X: %d", snake[0].x);
	// SDL_Log("Head Y: %d", snake[0].y);

	// win logic
	if (snakeSize >= (WINDOW_WIDTH / TILE_SIZE) * (WINDOW_HEIGHT / TILE_SIZE)) {
		printf("You won!\n");
		resetGame();
	}


	// frame rate
	SDL_Delay(90);
	// show what we just did to the renderer
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

// called at the end, and handles some of the exits
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	free(snake);
	free(food);
}

