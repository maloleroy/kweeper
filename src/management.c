#include <time.h>
#include <stdlib.h>

#include <args.h>
#include <management.h>

wr *create_context(int argc, char *argv[]) {
	static wr _context;
	if (argc == -1) return &_context;
	_context.verbose = is_flag("verbose", argc, argv);
	return &_context;
}

wr* get_context(void) {
	return create_context(-1, NULL);
}

void init_random(void) {
	srand(time(0));
}

void init_sdl(void) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) exit_with_error("initialization");
}

void init_size(SDL_bool defsize) {
	verb("SDL initialized (%s size).\n", defsize ? "default" : "auto");
	if (defsize) {
		get_context()->width = DEF_WIN_WIDTH;
		get_context()->height = DEF_WIN_HEIGHT;
	} else {
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);
		get_context()->width = dm.w;
		get_context()->height = dm.h;
	}
}

void create_window_renderer(void) {
	get_context()->window = SDL_CreateWindow("Kweeper",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // x and y position
		get_context()->width, get_context()->height,
		SDL_WINDOW_RESIZABLE);
	if (get_context()->window == NULL)
		exit_with_error("window creation");
	get_context()->renderer = SDL_CreateRenderer(get_context()->window, -1, SDL_RENDERER_ACCELERATED);
	if (get_context()->renderer == NULL)
		exit_with_error("renderer creation");
	if (SDL_SetRenderDrawBlendMode(get_context()->renderer, SDL_BLENDMODE_BLEND))
		exit_with_error("renderer blending mode setting");

	verb("Window and renderer created.\n");
}

void update_window_size(void) {
	SDL_GetWindowSize(get_context()->window, &(get_context()->width), &(get_context()->height));
}

void wait(unsigned int *frame_limit) {
	verb("\x1b[40DComputing time : %4ums", FPS_LIMIT - limitFPS(*frame_limit));
	*frame_limit = SDL_GetTicks() + FPS_LIMIT;
	fflush(stdout);
}

unsigned int limitFPS(unsigned int limit) {
	unsigned int ticks = SDL_GetTicks();
	if (limit < ticks)
		return 0;
	else if (limit > ticks + FPS_LIMIT) {
		SDL_Delay(FPS_LIMIT);
		return FPS_LIMIT;
	} else {
		SDL_Delay(limit - ticks);
		return limit-ticks;
	}
}

void pre_exit(void) {
	verb("Program closed.\n");
	SDL_Exit();
}

void exit_with_error(const char *msg) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s (%s).\n", msg, SDL_GetError());
	SDL_Exit();
	exit(EXIT_FAILURE);
}

void SDL_Exit(void) {
	if (get_context() != NULL) {
		if (get_context()->window != NULL) SDL_DestroyWindow(get_context()->window);
		if (get_context()->renderer != NULL) SDL_DestroyRenderer(get_context()->renderer);
	}
	SDL_Quit();
}
