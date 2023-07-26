#ifndef EVENTS_H
#define EVENTS_H

#include <SDL.h>
#include <management.h>
#include <grid.h>

typedef struct {
    SDL_bool launched, paused, pausable, refresh;
} run_status;

run_status *create_run_status(void);

void handle_all_events(grid* g, run_status* rs);

#endif
