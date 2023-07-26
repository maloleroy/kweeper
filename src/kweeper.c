#include <SDL.h>
#include <stdint.h>
#include <string.h>

#include <args.h>
#include <events.h>
#include <management.h>
#include <grid.h>
#include <display.h>

#define VERSION "0.0.0"

int main(int argc, char *argv[]) {
	create_context(argc, argv);
	run_status* rs = create_run_status();
	unsigned int frame_limit;

	if (is_flag("help", argc, argv)) {
		printf("%s", help_message);
		pre_exit();
		return 0;
	}

	init_random();
	init_sdl();
	init_size(is_flag("defsize", argc, argv));
	create_window_renderer();
	verb("Kweeper version %s\n", VERSION);
	frame_limit = SDL_GetTicks() + FPS_LIMIT;

	grid* g = create_grid();

	while (true) {
		rs->refresh = SDL_FALSE;
		handle_all_events(g, rs);
		if (rs->launched == SDL_FALSE) break;

		if (rs->refresh == SDL_TRUE) {
			display_everything(g);
			SDL_RenderPresent(get_context()->renderer);
		}
		wait(&frame_limit);
	}
	pre_exit();
	return 0;
}
