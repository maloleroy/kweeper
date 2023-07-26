#ifndef EVENTS_H
#define EVENTS_H

#include <SDL.h>
#include <management.h>
#include <grid.h>

run_status *create_run_status(void);

void handle_all_events(grid* g, run_status* rs);

#endif
