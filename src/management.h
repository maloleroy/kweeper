#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <SDL.h>

#define DEF_WIN_WIDTH 1920
#define DEF_WIN_HEIGHT 1080

// 16 for 60fps, 33 for 30fps, 100 for 10fps
#define FPS_LIMIT 33

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    SDL_bool verbose;
} wr;

typedef struct {
    unsigned int frame_limit, waited;
} time_status;

wr* create_context(int argc, char *argv[]);
wr* get_context(void);
void init_random(void);
void init_sdl(void);
void init_size(SDL_bool defsize);
void create_window_renderer(void);
void update_window_size(void);

void wait(unsigned int *frame_limit);
unsigned int limitFPS(unsigned int limit);

void pre_exit(void);
void exit_with_error(const char *msg);

void SDL_Exit(void);

#endif
