#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "macros.h"
#include "data.h"

static SDL_AppResult handle_key_event_(SnakeContext *ctx, SDL_Scancode key_code) {
	switch (key_code) {
		/* Quit. */
		case SDL_SCANCODE_ESCAPE:
		case SDL_SCANCODE_Q:
			return SDL_APP_SUCCESS;
			/* Restart the game as if the program was launched. */
		case SDL_SCANCODE_R:
			snake_initialize(ctx);
			break;
			/* Decide new direction of the snake. */
		case SDL_SCANCODE_RIGHT:
			snake_redir(ctx, SNAKE_DIR_RIGHT);
			break;
		case SDL_SCANCODE_UP:
			snake_redir(ctx, SNAKE_DIR_UP);
			break;
		case SDL_SCANCODE_LEFT:
			snake_redir(ctx, SNAKE_DIR_LEFT);
			break;
		case SDL_SCANCODE_DOWN:
			snake_redir(ctx, SNAKE_DIR_DOWN);
			break;
		default:
			break;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	AppState *as = (AppState *)appstate;
	SnakeContext *ctx = &as->snake_ctx;
	const Uint64 now = SDL_GetTicks();
	SDL_FRect r;
	unsigned i;
	unsigned j;
	int ct;

	// run game logic if we're at or past the time to run it.
	// if we're _really_ behind the time to run it, run it
	// several times.
	while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
		snake_step(ctx);
		as->last_step += STEP_RATE_IN_MILLISECONDS;
	}

	r.w = r.h = SNAKE_BLOCK_SIZE_IN_PIXELS;
	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);
	for (i = 0; i < SNAKE_GAME_WIDTH; i++) {
		for (j = 0; j < SNAKE_GAME_HEIGHT; j++) {
			ct = snake_cell_at(ctx, i, j);
			if (ct == SNAKE_CELL_NOTHING)
				continue;
			set_rect_xy_(&r, i, j);
			if (ct == SNAKE_CELL_FOOD)
				SDL_SetRenderDrawColor(as->renderer, 80, 80, 255, SDL_ALPHA_OPAQUE);
			else /* body */
				SDL_SetRenderDrawColor(as->renderer, 0, 128, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(as->renderer, &r);
		}
	}
	SDL_SetRenderDrawColor(as->renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); /*head*/
	set_rect_xy_(&r, ctx->head_xpos, ctx->head_ypos);
	SDL_RenderFillRect(as->renderer, &r);
	SDL_RenderPresent(as->renderer);
	return SDL_APP_CONTINUE;
}

static const struct {
	const char *key;
	const char *value;
} extended_metadata[] = {
	{ SDL_PROP_APP_METADATA_URL_STRING, "https://examples.libsdl.org/SDL3/demo/01-snake/" },
	{ SDL_PROP_APP_METADATA_CREATOR_STRING, "SDL team" },
	{ SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed in the public domain" },
	{ SDL_PROP_APP_METADATA_TYPE_STRING, "game" }
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	size_t i;

	if (!SDL_SetAppMetadata("Example Snake game", "1.0", "com.example.Snake")) {
		return SDL_APP_FAILURE;
	}

	for (i = 0; i < SDL_arraysize(extended_metadata); i++) {
		if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) {
			return SDL_APP_FAILURE;
		}
	}

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
	if (!as) {
		return SDL_APP_FAILURE;
	}

	*appstate = as;

	if (!SDL_CreateWindowAndRenderer("examples/demo/snake", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 0, &as->window, &as->renderer)) {
		return SDL_APP_FAILURE;
	}

	snake_initialize(&as->snake_ctx);

	as->last_step = SDL_GetTicks();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	SnakeContext *ctx = &((AppState *)appstate)->snake_ctx;
	switch (event->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		case SDL_EVENT_KEY_DOWN:
			return handle_key_event_(ctx, event->key.scancode);
		default:
			break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	if (appstate != NULL) {
		AppState *as = (AppState *)appstate;
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		SDL_free(as);
	}
}

