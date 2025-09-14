#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "macros.h"
#include "data.h"

#ifndef FUNCTIONS 
#define FUNCTIONS 

SnakeCell snake_cell_at(const SnakeContext *ctx, char x, char y);
void set_rect_xy_(SDL_FRect *r, short x, short y);
void put_cell_at_(SnakeContext *ctx, char x, char y, SnakeCell ct);
int are_cells_full_(SnakeContext *ctx);
void new_food_pos_(SnakeContext *ctx);
void snake_initialize(SnakeContext *ctx);
void snake_redir(SnakeContext *ctx, SnakeDirection dir);
void wrap_around_(char *val, char max);
void snake_step(SnakeContext *ctx);

#endif
