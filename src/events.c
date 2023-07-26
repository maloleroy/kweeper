#include <args.h>
#include <events.h>
#include <grid.h>

static void handle_event(SDL_Event event, grid* g, run_status* rs);
static void handle_key_press(SDL_Keycode code, grid* g, run_status* rs);
static void handle_click(Uint8 button, struct xy pos, grid* g);
static void toggle_pause(run_status* rs);
static void handle_window_event(SDL_Event event, grid* g);

run_status *create_run_status(void) {
    static run_status _rs = {
        .launched = SDL_TRUE,
        .paused = SDL_FALSE,
        .pausable = SDL_FALSE,
        .refresh = SDL_TRUE,
    };
    return &_rs;
}

void handle_all_events(grid* g, run_status* rs) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) handle_event(event, g, rs);
}

static void handle_event(SDL_Event event, grid* g, run_status* rs) {
    switch (event.type) {
        case SDL_QUIT:
            rs->launched = SDL_FALSE;
            verb("  \n\nQuit event detected.\n");
            break;
        case SDL_WINDOWEVENT:
            rs->refresh = SDL_TRUE;
            handle_window_event(event, g);
            break;
        case SDL_MOUSEBUTTONUP:
            rs->refresh = SDL_TRUE;
            handle_click(event.button.button, (struct xy){event.button.x, event.button.y}, g);
            break;
        case SDL_KEYDOWN:
            rs->refresh = SDL_TRUE;
            handle_key_press(event.key.keysym.sym, g, rs);
            break;
        default:
            break;
    }
}

static void handle_key_press(SDL_Keycode code, grid* g, run_status* rs) {
    switch (code) {
        case SDLK_ESCAPE:
            rs->launched = SDL_FALSE;
            verb("\n\nQuit event detected (Esc).\n");
            break;
        case SDLK_SPACE:
            toggle_pause(rs);
            break;
        case SDLK_r:
            restart_new_game(g);
            break;
        case SDLK_s:
            grid_reveal_all_cells(g);
            break;
        default:
            break;
    }
}

static void toggle_pause(run_status* rs) {
    if (rs->paused == SDL_FALSE) {
        rs->paused = SDL_TRUE;
        verb("\n\x1b[50DProgram paused.  \x1b[1A");
    } else {
        rs->paused = SDL_FALSE;
        verb("\n\x1b[50DProgram unpaused.\x1b[1A");
    }
}

static void handle_click(Uint8 button, struct xy pos, grid* g) {
    grid_handle_click(pos, g);
}


static void handle_window_event(SDL_Event event, grid* g) {
    switch (event.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            update_window_size();
            update_grid_size(g);
            // Re-calculating the zooms, positions (centered & responsive), and speeds
            verb("\x1b[1A\x1b[50DSize : %d by %d px\n", get_context()->width, get_context()->height);
            break;
        default:
            break;
    }
}
