#ifndef GRID_H
#define GRID_H

#include <management.h>

#define SIZE 8 // the size of the grid
#define NB_MINES 10 // # the initial number of mines

typedef struct {
    SDL_bool launched, paused, pausable, refresh, virgin;
} run_status;

/* Generic type to store 2-dimension coordinates */
struct xy {
	int x /* x pos, width or x text-margin */;
	int y /* y pos, height or y text-margin */;
};

/* Stores a position in the 12x12 grid of the game
 * More abstract than the previous type
 */
struct ij {
    int i;
	int j;
};
#define NONE_IJ (struct ij){-1, -1}
SDL_bool is_none_ij(struct ij);

/* The number usually displayed on the cell, or MINE (i.e. -1) if is a mine. */
#define MINE -1

typedef enum {COVERED, UNCOVERED, FLAGGED, EXPLODED} cell_status;

/* Stores all content regarding a cell in the grid */
struct cell {
	struct ij loc; // position in the grid
	int value; // mine neighbors
	int flagged; // flagged neighbors
	cell_status status;
};

typedef struct {
	struct cell cells[SIZE][SIZE];
	struct xy anchor; // anchor of the grid (graphically)
	int side; // size of the entire grid
	int cell_size; // size of one cell
	int cell_margin; // margin between cells
	int uncovered; // total uncovered cells
} grid;

#define CELL(g, loc) (g->cells)[loc.i][loc.j]
#define EXTRACT_CELL(loc) int i = loc.i, j = loc.j;

/* Create a unique new grid */
grid* create_grid(void);

/* Begin a new game whose first click is at pos */
void grid_begin_game(struct xy pos, grid* g, run_status* rs);

/* Restart, begin a new game */
void restart_new_game(grid* g);

/* Return the anchor of a given cell in the grid */
struct xy get_cell_anchor(struct ij loc, grid* g);

/* Return the size of a dot, in pixels */
int dot_size(void);

/* Handle a left click event that might have effects on the grid */
void grid_handle_left_click(struct xy pos, grid* g);

/* Handle a right click event that might have effects on the grid */
void grid_handle_right_click(struct xy pos, grid* g);

/* Reveal all cells (including yet hidden) */
void grid_reveal_all_cells(grid* g);

/* Call when the window size is changed */
void update_grid_size(grid* g);

/* Fills array with `size` random different values in {0, ..., max-1} */
void random_array_non_repeat(int array[], int size, int max);

/* Fills array with `size` random different names with i, j in {0, ..., max-1} */
void random_ij_array_non_repeat(struct ij* array, int size, int max);

#endif
