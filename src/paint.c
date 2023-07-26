#include <paint.h>

static void paint_dot4_digit(struct xy anchor, int n, rgba color);

void change_color(rgba color) {
    if (SDL_SetRenderDrawColor(get_context()->renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE) != 0)
        exit_with_error("change render draw color");
}

void paint_rect(SDL_Rect r, rgba color) {
    change_color(color);
    if (SDL_RenderFillRect(get_context()->renderer, &r) != 0)
        exit_with_error("fill rectangle");
}

void paint_rect_anchor(struct xy anchor, int width, int height, rgba color) {
    SDL_Rect r = {anchor.x, anchor.y, width, height};
    paint_rect(r, color);
}

void paint_square_anchor(struct xy anchor, int side, rgba color) {
    paint_rect_anchor(anchor, side, side, color);
}

void paint_dot(struct xy anchor, rgba color) {
    paint_square_anchor(anchor, dot_size(), color);
}

struct xy delta_x(struct xy pos, int delta) {
    return (struct xy){pos.x + delta, pos.y};
}

struct xy delta_y(struct xy pos, int delta) {
    return (struct xy){pos.x, pos.y + delta};
}

struct xy delta_xy(struct xy pos, int dx, int dy) {
    return (struct xy){pos.x + dx, pos.y + dy};
}

int window_constraint(void) {
    if (get_context()->width < get_context()->height) {
        return get_context()->width;
    }
    return get_context()->height;
}

const rgba nord[16] = {
    {0x2e, 0x34, 0x40, 255},
    {0x3b, 0x42, 0x52, 255},
    {0x43, 0x4c, 0x5e, 255},
    {0x4c, 0x56, 0x6a, 255},
    {0xd8, 0xde, 0xe9, 255},
    {0xe5, 0xe9, 0xf0, 255},
    {0xec, 0xef, 0xf4, 255},
    {0x8f, 0xbc, 0xbb, 255},
    {0x88, 0xc0, 0xd0, 255},
    {0x81, 0xa1, 0xc1, 255},
    {0x5e, 0x81, 0xac, 255},
    {0xbf, 0x61, 0x6a, 255},
    {0xd0, 0x87, 0x70, 255},
    {0xeb, 0xcb, 0x8b, 255},
    {0xa3, 0xbe, 0x8c, 255},
    {0xb4, 0x8e, 0xad, 255},
};

void paint_dot4(struct xy anchor, char *str, rgba color) {
    int i = 0;
    while (str[i] != 0) {
        struct xy a = delta_x(anchor, 5*dot_size()*i);
        switch (str[i]) {
            case '0':
                paint_dot4_digit(a, 0, color);
                break;
            case '1':
                paint_dot4_digit(a, 1, color);
                break;
            case '2':
                paint_dot4_digit(a, 2, color);
                break;
            case '3':
                paint_dot4_digit(a, 3, color);
                break;
            case '4':
                paint_dot4_digit(a, 4, color);
                break;
            case '5':
                paint_dot4_digit(a, 5, color);
                break;
            case '6':
                paint_dot4_digit(a, 6, color);
                break;
            case '7':
                paint_dot4_digit(a, 7, color);
                break;
            case '8':
                paint_dot4_digit(a, 8, color);
                break;
            case '9':
                paint_dot4_digit(a, 9, color);
                break;
            case '.':
                paint_dot(delta_xy(a, dot_size(), 3*dot_size()), color);
            default:
                break;
        }
        i++;
    }
}

static const int digits4[10] = {
    27030, 19534, 26927, 58926, 9970, 64556, 26862, 57928, 59982, 60972
};

static void paint_dot4_digit(struct xy anchor, int n, rgba color) {
    if (n < 0 || n > 9) return;
    int d = dot_size();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if ((digits4[n] >> (15 - (4*i+j))) & 1) {
                paint_dot(delta_xy(anchor, j*d, i*d), color);
            }
        }
    }
}
