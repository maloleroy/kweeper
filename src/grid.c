#include <grid.h>
#include <paint.h>

static void drop_mines(grid* g);
static void grid_uncover_cell(struct ij loc, grid* g);
static void propagate_uncover_cell(struct ij loc, grid* g);
static void fast_clear(struct ij loc, grid* g);
static struct ij cell_of_click(struct xy pos, grid* g);
static struct xy grid_anchor(void);
static int grid_side(void);
static int grid_cell_margin(void);
static int grid_cell_size(void);

SDL_bool is_none_ij(struct ij loc) {
    return (loc.i == -1 && loc.j == -1);
}

grid* create_grid(void) {
    static grid _g;
    _g.uncovered = 0;
    _g.status = ONGOING;
    update_grid_size(&_g);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            _g.cells[i][j] = (struct cell){.loc={i, j}, .value=0, .flagged=0, .status=COVERED};
        }
    }
    drop_mines(&_g);
    return &_g;
}

void grid_begin_game(struct xy pos, grid* g, run_status* rs) {
    struct ij loc = cell_of_click(pos, g);
    if (!is_none_ij(loc)) {
        restart_new_game(g);
        while (CELL(g, loc).value != 0) {
            restart_new_game(g);
        }
        rs->virgin = SDL_FALSE;
        g->game_time = SDL_GetTicks();
    }
}

void restart_new_game(grid* g) {
    g->uncovered = 0;
    g->status = ONGOING;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            g->cells[i][j] = (struct cell){.loc={i, j}, .value=0, .flagged=0, .status=COVERED};
        }
    }
    drop_mines(g);
}

static void drop_mines(grid* g) {
    struct ij array[NB_MINES];
    random_ij_array_non_repeat(array, NB_MINES, SIZE);
    for (int k = 0; k < NB_MINES; k++) {
        int i = array[k].i;
        int j = array[k].j;
        g->cells[i][j].value = -1;
        // all neighbours, clockwise from top left
        if (i > 0 && j > 0 && g->cells[i-1][j-1].value != MINE) g->cells[i-1][j-1].value++;
        if (i > 0 && g->cells[i-1][j].value != MINE) g->cells[i-1][j].value++;
        if (i > 0 && j < SIZE-1 && g->cells[i-1][j+1].value != MINE) g->cells[i-1][j+1].value++;
        if (j < SIZE-1 && g->cells[i][j+1].value != MINE) g->cells[i][j+1].value++;
        if (i < SIZE-1 && j < SIZE-1 && g->cells[i+1][j+1].value != MINE) g->cells[i+1][j+1].value++;
        if (i < SIZE-1 && g->cells[i+1][j].value != MINE) g->cells[i+1][j].value++;
        if (i < SIZE-1 && j > 0 && g->cells[i+1][j-1].value != MINE) g->cells[i+1][j-1].value++;
        if (j > 0 && g->cells[i][j-1].value != MINE) g->cells[i][j-1].value++;
    }
}

void grid_handle_left_click(struct xy pos, grid* g) {
    struct ij loc = cell_of_click(pos, g);
    if (!is_none_ij(loc)) {
        switch (CELL(g, loc).status) {
            case COVERED:
                propagate_uncover_cell(loc, g);
                break;
            case UNCOVERED:
                fast_clear(loc, g);
                break;
            default:
                break;
        }
    }
}

void grid_handle_right_click(struct xy pos, grid* g) {
    struct ij loc = cell_of_click(pos, g);
    if (!is_none_ij(loc)) {
        EXTRACT_CELL(loc);
        int delta = 0;
        if (CELL(g, loc).status == COVERED) {
            CELL(g, loc).status = FLAGGED;
            delta = 1;
        } else if (CELL(g, loc).status == FLAGGED) {
            CELL(g, loc).status = COVERED;
            delta = -1;
        }
        if (i > 0 && j > 0) g->cells[i-1][j-1].flagged += delta;
        if (i > 0) g->cells[i-1][j].flagged += delta;
        if (i > 0 && j < SIZE-1) g->cells[i-1][j+1].flagged += delta;
        if (j < SIZE-1) g->cells[i][j+1].flagged += delta;
        if (i < SIZE-1 && j < SIZE-1) g->cells[i+1][j+1].flagged += delta;
        if (i < SIZE-1) g->cells[i+1][j].flagged += delta;
        if (i < SIZE-1 && j > 0) g->cells[i+1][j-1].flagged += delta;
        if (j > 0) g->cells[i][j-1].flagged += delta;
    }
}

void grid_reveal_all_cells(grid* g) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid_uncover_cell((struct ij){i, j}, g);
        }
    }
}

static void grid_uncover_cell(struct ij loc, grid* g) {
    if (CELL(g, loc).status == COVERED) {
        if (CELL(g, loc).value == MINE) {
            CELL(g, loc).status = EXPLODED;
            g->status = LOST;
            grid_reveal_all_cells(g);
        } else {
            g->uncovered++;
            CELL(g, loc).status = UNCOVERED;
            if (g->status == ONGOING && g->uncovered == SIZE*SIZE - NB_MINES) {
                g->status = WON;
                g->game_time = SDL_GetTicks() - g->game_time;
            }
        }
    }
}

static void propagate_uncover_cell(struct ij loc, grid* g) {
    EXTRACT_CELL(loc);
    if (CELL(g, loc).status != COVERED) return;
    grid_uncover_cell(loc, g);
    if (CELL(g, loc).value == 0) {
        if (i > 0 && j > 0) propagate_uncover_cell((struct ij){i-1, j-1}, g);
        if (i > 0) propagate_uncover_cell((struct ij){i-1, j}, g);
        if (i > 0 && j < SIZE-1) propagate_uncover_cell((struct ij){i-1, j+1}, g);
        if (j < SIZE-1 ) propagate_uncover_cell((struct ij){i, j+1}, g);
        if (i < SIZE-1 && j < SIZE-1) propagate_uncover_cell((struct ij){i+1, j+1}, g);
        if (i < SIZE-1) propagate_uncover_cell((struct ij){i+1, j}, g);
        if (i < SIZE-1 && j > 0) propagate_uncover_cell((struct ij){i+1, j-1}, g);
        if (j > 0) propagate_uncover_cell((struct ij){i, j-1}, g);
    }
}

static void fast_clear(struct ij loc, grid* g) {
    EXTRACT_CELL(loc);
    if (CELL(g, loc).value == CELL(g, loc).flagged) {
        if (i > 0 && j > 0 && g->cells[i-1][j-1].status != FLAGGED) propagate_uncover_cell((struct ij){i-1, j-1}, g);
        if (i > 0 && g->cells[i-1][j].status != FLAGGED) propagate_uncover_cell((struct ij){i-1, j}, g);
        if (i > 0 && j < SIZE-1 && g->cells[i-1][j+1].status != FLAGGED) propagate_uncover_cell((struct ij){i-1, j+1}, g);
        if (j < SIZE-1 && g->cells[i][j+1].status != FLAGGED) propagate_uncover_cell((struct ij){i, j+1}, g);
        if (i < SIZE-1 && j < SIZE-1 && g->cells[i+1][j+1].status != FLAGGED) propagate_uncover_cell((struct ij){i+1, j+1}, g);
        if (i < SIZE-1 && g->cells[i+1][j].status != FLAGGED) propagate_uncover_cell((struct ij){i+1, j}, g);
        if (i < SIZE-1 && j > 0 && g->cells[i+1][j-1].status != FLAGGED) propagate_uncover_cell((struct ij){i+1, j-1}, g);
        if (j > 0 && g->cells[i][j-1].status != FLAGGED) propagate_uncover_cell((struct ij){i, j-1}, g);
    }
}

struct xy get_cell_anchor(struct ij loc, grid* g) {
    return (struct xy){
        .x=g->anchor.x + (loc.j+1)*(g->cell_margin) + (loc.j*g->cell_size),
        .y=g->anchor.y + (loc.i+1)*(g->cell_margin) + (loc.i*g->cell_size)};
}

int dot_size(void) {
    return window_constraint() / 100;
}

struct ij cell_of_click(struct xy pos, grid* g) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            struct xy a = get_cell_anchor((struct ij){i, j}, g);
            if (a.x < pos.x && pos.x < g->cell_size+a.x &&
                a.y < pos.y && pos.y < g->cell_size+a.y) {
                return (struct ij){i, j};
            }
        }
    }
    return NONE_IJ;
}

void update_grid_size(grid *g) {
    g->anchor = grid_anchor();
    g->side = grid_side();
    g->cell_size = grid_cell_size();
    g->cell_margin = grid_cell_margin();
}

static struct xy grid_anchor(void) {
    int d = grid_side() / 2;
    return (struct xy){.x=get_context()->width/2 - d, .y=get_context()->height/2 - d};
}

static int grid_side(void) {
    return 4 * window_constraint() / 5;
}

static int grid_cell_size(void) {
    return (grid_side() - (SIZE+1) * grid_cell_margin()) / SIZE;
}

static int grid_cell_margin(void) {
    return window_constraint() / 100;
}

void random_array_non_repeat(int array[], int size, int max) {
	for (int i = 0; i < size; i++) {
		SDL_bool pass = SDL_FALSE;
		int r = 0;
		while (!pass) {
			r = rand() % max;
			pass = SDL_TRUE;
			for (int j = 0; j < i; j++) {
				if (array[j] == r) pass = SDL_FALSE;
			}
		}
		array[i] = r;
	}
}

/* Fills given array with values in {0, ..., max-1} */
void random_ij_array_non_repeat(struct ij* array, int size, int max) {
    for (int n = 0; n < size; n++) {
		SDL_bool pass = SDL_FALSE;
		int i, j;
		while (!pass) {
			i = rand() % max;
            j = rand() % max;
			pass = SDL_TRUE;
			for (int k = 0; k < n; k++) {
				if (array[k].i == i && array[k].j == j) {
                    pass = SDL_FALSE;
                    break;
                }
			}
		}
		array[n] = (struct ij){i, j};
	}
}
