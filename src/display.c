#include <display.h>
#include <paint.h>

static void display_background(void);
static void display_grid(grid* g);
static void display_cell(grid* g, struct ij loc);
static int uncovered_cell_color_number(grid* g, struct ij loc);
static void display_cell_value(grid* g, struct ij loc);

void display_everything(grid* g) {
    display_background();
    display_grid(g);
}

static void display_background(void) {
    paint_rect((SDL_Rect){0, 0, get_context()->width, get_context()->height}, nord[10]);
}

static void display_grid(grid* g) {
    int d = -2 * g->cell_margin;
    paint_square_anchor(delta_xy(g->anchor, d, d), g->side, nord[0]);
    paint_square_anchor(g->anchor, g->side, nord[1]);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            display_cell(g, (struct ij){i, j});
        }
    }
}

static void display_cell(grid* g, struct ij loc) {
    struct xy anchor = get_cell_anchor(loc, g);
    switch (CELL(g, loc).status) {
        case COVERED:
            paint_square_anchor(anchor, g->cell_size, nord[2]);
            break;
        case UNCOVERED:
            paint_square_anchor(anchor, g->cell_size,
                nord[uncovered_cell_color_number(g, loc)]);
            display_cell_value(g, loc);
            break;
        case EXPLODED:
            paint_square_anchor(anchor, g->cell_size, nord[11]);
            break;
        case FLAGGED:
            paint_square_anchor(anchor, g->cell_size, nord[2]);
            int s = (g->cell_size - dot_size()) / 2;
            paint_dot(delta_xy(anchor, s, s), nord[11]);
            break;
    }
}

static int uncovered_cell_color_number(grid* g, struct ij loc) {
    switch (CELL(g, loc).value) {
        case 0:
            return 4;
        case 1:
            return 14;
        case 2:
            return 13;
        case 3:
            return 12;
        case 4:
            return 15;
        case 5:
            return 10;
        case 6:
            return 9;
        default:
            return 8;
    }
}

static void display_cell_value(grid* g, struct ij loc) {
    struct xy a = get_cell_anchor(loc, g);
    int d = dot_size();
    int s = (g->cell_size - d) / 2;
    switch (CELL(g, loc).value) {
        case 0:
            break;
        case 1:
            paint_dot(delta_xy(a, s, s), nord[0]);
            break;
        case 2:
            paint_dot(delta_xy(a, s, s-d), nord[0]);
            paint_dot(delta_xy(a, s, s+d), nord[0]);
            break;
        case 3:
            paint_dot(delta_xy(a, s, s), nord[0]);
            paint_dot(delta_xy(a, s-2*d, s-2*d), nord[0]);
            paint_dot(delta_xy(a, s+2*d, s+2*d), nord[0]);
            break;
        case 4:
            paint_dot(delta_xy(a, s-2*d, s+2*d), nord[0]);
            paint_dot(delta_xy(a, s+2*d, s-2*d), nord[0]);
            paint_dot(delta_xy(a, s-2*d, s-2*d), nord[0]);
            paint_dot(delta_xy(a, s+2*d, s+2*d), nord[0]);
            break;
        case 5:
            paint_dot(delta_xy(a, s, s), nord[0]);
            paint_dot(delta_xy(a, s-2*d, s+2*d), nord[0]);
            paint_dot(delta_xy(a, s+2*d, s-2*d), nord[0]);
            paint_dot(delta_xy(a, s-2*d, s-2*d), nord[0]);
            paint_dot(delta_xy(a, s+2*d, s+2*d), nord[0]);
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
    }
}
