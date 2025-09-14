#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef DATA 
#define DATA 

typedef enum {
	SNAKE_CELL_NOTHING = 0U,
	SNAKE_CELL_SRIGHT = 1U,
	SNAKE_CELL_SUP = 2U,
	SNAKE_CELL_SLEFT = 3U,
	SNAKE_CELL_SDOWN = 4U,
	SNAKE_CELL_FOOD = 5U
} SnakeCell;

typedef enum {
	SNAKE_DIR_RIGHT,
	SNAKE_DIR_UP,
	SNAKE_DIR_LEFT,
	SNAKE_DIR_DOWN
} SnakeDirection;

typedef struct {
	unsigned char cells[(SNAKE_MATRIX_SIZE * SNAKE_CELL_MAX_BITS) / 8U];
	char head_xpos;
	char head_ypos;
	char tail_xpos;
	char tail_ypos;
	char next_dir;
	char inhibit_tail_step;
	unsigned occupied_cells;
} SnakeContext;

// use the app state for any rendering or other game data
typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SnakeContext snake_ctx;
	Uint64 last_step;
} AppState;

#endif
