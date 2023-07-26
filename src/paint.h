#ifndef PAINT_H
#define PAINT_H

#include <grid.h>

typedef struct {
	unsigned char r, g, b, a;
} rgba;

const rgba nord[16];

void change_color(rgba color);
void paint_rect(SDL_Rect r, rgba color);
void paint_rect_anchor(struct xy anchor, int width, int height, rgba color);
void paint_square_anchor(struct xy anchor, int side, rgba color);
void paint_dot(struct xy anchor, rgba color);

/* Apply a shift to the position */
struct xy delta_x(struct xy pos, int delta);
struct xy delta_y(struct xy pos, int delta);
struct xy delta_xy(struct xy pos, int dx, int dy);

/* Return the maximal size of a square fitting the window */
int window_constraint(void);

#endif
