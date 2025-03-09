/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "board.h"

// Grid dimensions
#define ROWS (16)
#define COLS (30)
#define NUM_MINES (99)
#define CELL_WIDTH (25)

// Header (constant)
#define HDR_HEIGHT (80)

// Borders
#define BDR_WIDTH (8)
#define SHAD_WIDTH (4)

// Padding
#define SCORE_PADDING (6)
#define SCORE_KERNING (1)

static const Color BG_COLOR  = (Color) {150, 150, 150, 255};
static const Color BDR_COLOR = (Color) {160, 160, 160, 255};
static const Color SHAD_COLOR_DK = (Color) {100, 100, 100, 255};
static const Color SHAD_COLOR_LT = (Color) {200, 200, 200, 255};
static const Color GRID_COLOR = GREEN;

static const size_t HEIGHT =
    HDR_HEIGHT + (CELL_WIDTH * ROWS) + (2 * SHAD_WIDTH) + BDR_WIDTH;

static const size_t WIDTH =
    (CELL_WIDTH * COLS) + (2 * SHAD_WIDTH) + (2 * BDR_WIDTH);

static const Rectangle PLAY_BOUNDS = (Rectangle){
  BDR_WIDTH + SHAD_WIDTH,
  HDR_HEIGHT + SHAD_WIDTH,
  CELL_WIDTH *COLS,
  CELL_WIDTH * ROWS
};

typedef enum {
    CS_COVERED,
    CS_REVEALED,
    CS_FLAGGED
} CellState;

typedef struct {
    CellState state;
    uint8_t val;
} Cell;

typedef struct {
    Font score_font;
    Texture face_tex;
    float face_scale;
    Cell *cells;
    size_t cell_count;
    size_t mines_remaining;
    size_t time_elapsed;
    Vector2 mouse_pos;
    int button_pressed;
} RenderState;

typedef enum {
    MARK,
    REVEAL,
    NONE,
} Action;

typedef struct {
    Vector2 mouse_pos;
    Action action;
} Input;

// TODO: consider combining draw_top.. and draw_bottom. The only difference is
//       the direction of the end triangles.
void draw_top_beveled_edge(size_t x, size_t y, size_t l, size_t w, Color c)
{
    DrawRectangle(x + w, y, l - (2 * w), w, c);
    DrawTriangle((Vector2) {x, y},
	         (Vector2) {x + w, y + w},
	         (Vector2) {x + w, y},
	         c);
    DrawTriangle((Vector2) {x + l - w, y},
	         (Vector2) {x + l - w, y + w},
	         (Vector2) {x + l, y},
	         c);
}

void draw_bottom_beveled_edge(size_t x, size_t y, size_t l, size_t w, Color c)
{
    DrawRectangle(x + w, y, l - (2 * w), w, c);
    DrawTriangle((Vector2) {x, y + w},
	         (Vector2) {x + w, y + w},
	         (Vector2) {x + w, y},
	         c);
    DrawTriangle((Vector2) {x + l - w, y},
	         (Vector2) {x + l - w, y + w},
	         (Vector2) {x + l, y + w},
	         c);
}

void draw_left_beveled_edge(size_t x, size_t y, size_t l, size_t w, Color c)
{
    DrawRectangle(x, y + w, w, l - (2 * w), c);
    DrawTriangle((Vector2) {x, y},
	         (Vector2) {x, y + w},
	         (Vector2) {x + w, y + w},
	         c);
    DrawTriangle((Vector2) {x, y + l - w},
	         (Vector2) {x, y + l},
	         (Vector2) {x + w, y + l - w},
	         c);

}

void draw_right_beveled_edge(size_t x, size_t y, size_t l, size_t w, Color c)
{
    DrawRectangle(x, y + w, w, l - (2 * w), c);
    DrawTriangle((Vector2) {x + w, y + w},
	         (Vector2) {x + w, y},
	         (Vector2) {x, y + w},
	         c);
    DrawTriangle((Vector2) {x + w, y + l - w},
	         (Vector2) {x, y + l - w},
	         (Vector2) {x + w, y + l},
	         c);
}


// TODO: Consider combining draw_header_borders and draw_game_borders into a single
//       function called draw_frame(Rectangle);
void draw_header_borders(void) {
    draw_top_beveled_edge(0, 0, WIDTH, BDR_WIDTH, BDR_COLOR);
    draw_top_beveled_edge(BDR_WIDTH, BDR_WIDTH, WIDTH - (2 * BDR_WIDTH), SHAD_WIDTH, SHAD_COLOR_DK);

    draw_left_beveled_edge(0, 0, HDR_HEIGHT, BDR_WIDTH, BDR_COLOR);
    draw_left_beveled_edge(BDR_WIDTH, BDR_WIDTH, HDR_HEIGHT - (2 * BDR_WIDTH), SHAD_WIDTH, SHAD_COLOR_DK);

    draw_bottom_beveled_edge(0, HDR_HEIGHT - BDR_WIDTH, WIDTH, BDR_WIDTH, BDR_COLOR);
    draw_bottom_beveled_edge(BDR_WIDTH, HDR_HEIGHT - SHAD_WIDTH - BDR_WIDTH, WIDTH - (2 * BDR_WIDTH), SHAD_WIDTH, SHAD_COLOR_LT);

    draw_right_beveled_edge(WIDTH - BDR_WIDTH, 0, HDR_HEIGHT, BDR_WIDTH, BDR_COLOR);
    draw_right_beveled_edge(WIDTH - BDR_WIDTH - SHAD_WIDTH, BDR_WIDTH, HDR_HEIGHT - (2 * BDR_WIDTH), SHAD_WIDTH, SHAD_COLOR_LT);
}

void draw_game_borders(void)
{
    draw_top_beveled_edge(0, HDR_HEIGHT - BDR_WIDTH, WIDTH, BDR_WIDTH, BDR_COLOR);
    draw_top_beveled_edge(BDR_WIDTH, HDR_HEIGHT, WIDTH - (2 * BDR_WIDTH), SHAD_WIDTH, SHAD_COLOR_DK);

    draw_left_beveled_edge(0, HDR_HEIGHT - BDR_WIDTH, HEIGHT - HDR_HEIGHT + BDR_WIDTH, BDR_WIDTH, BDR_COLOR);
    draw_left_beveled_edge(BDR_WIDTH, HDR_HEIGHT, HEIGHT - HDR_HEIGHT - BDR_WIDTH, SHAD_WIDTH, SHAD_COLOR_DK);

    draw_bottom_beveled_edge(0, HEIGHT - BDR_WIDTH, WIDTH, BDR_WIDTH, BDR_COLOR);
    draw_bottom_beveled_edge(BDR_WIDTH, HEIGHT - BDR_WIDTH - SHAD_WIDTH, WIDTH - (2 * BDR_WIDTH), SHAD_WIDTH, SHAD_COLOR_LT);

    draw_right_beveled_edge(WIDTH - BDR_WIDTH, HDR_HEIGHT - BDR_WIDTH, HEIGHT - HDR_HEIGHT + BDR_WIDTH, BDR_WIDTH, BDR_COLOR);
    draw_right_beveled_edge(WIDTH - BDR_WIDTH - SHAD_WIDTH, HDR_HEIGHT, HEIGHT - HDR_HEIGHT - BDR_WIDTH, SHAD_WIDTH, SHAD_COLOR_LT);
}

// TODO: Instead of re-drawing this every time, I could render it onto a texture and
//       place the texture in the game play frame. Possibly even with the correct
//       numbers in place since they do not change after the first click
void draw_grid(void)
{
    size_t width = (WIDTH - (2 * SHAD_WIDTH) - (2 * BDR_WIDTH)) / COLS;
    size_t height = (HEIGHT - HDR_HEIGHT - (2 * SHAD_WIDTH) - BDR_WIDTH) / ROWS;
    size_t start_x = BDR_WIDTH + SHAD_WIDTH;
    size_t start_y = HDR_HEIGHT + SHAD_WIDTH;
    for (size_t row = 0; row < ROWS; row++) {
	for (size_t col = 0; col < COLS; col++) {
	    DrawRectangleLines(start_x + (col * width), start_y + (row * height), width, height, GRID_COLOR);
	}
    }

}

#define BUF_SIZE (4)

void draw_score_panel(size_t x, size_t y, size_t width, size_t height, size_t score, Font font)
{
    int v_offset = 8;
    DrawRectangle(x, y, width, height, BLACK);

    char text[BUF_SIZE] = { 0 };
    snprintf(text, BUF_SIZE, "%03zu", score);
    float scale = 0.6;
    DrawTextEx(font, text, (Vector2){x + SCORE_KERNING, y + v_offset}, (float)height * scale, SCORE_KERNING, RED);
}

void draw_mines_remaining(size_t mines_count, Font font)
{
    size_t panel_height = HDR_HEIGHT - (2 * BDR_WIDTH) - (2 * SHAD_WIDTH) - (2 * SCORE_PADDING);
    size_t panel_width = ((panel_height * 3) / 2) + (2 * SCORE_KERNING);
    size_t panel_x = BDR_WIDTH + SHAD_WIDTH + SCORE_PADDING;
    size_t panel_y = BDR_WIDTH + SHAD_WIDTH + SCORE_PADDING;
    draw_score_panel(panel_x, panel_y, panel_width, panel_height, mines_count, font);
}

void draw_elapsed_time(size_t time_elapsed, Font font)
{
    size_t panel_height = HDR_HEIGHT - (2 * BDR_WIDTH) - (2 * SHAD_WIDTH) - (2 * SCORE_PADDING);
    size_t panel_width = ((panel_height * 3) / 2) + (2 * SCORE_KERNING);
    size_t panel_x = WIDTH - BDR_WIDTH - SHAD_WIDTH - SCORE_PADDING - panel_width;
    size_t panel_y = BDR_WIDTH + SHAD_WIDTH + SCORE_PADDING;
    draw_score_panel(panel_x, panel_y, panel_width, panel_height, time_elapsed, font);
}

void draw_button(Rectangle bounds, Color color)
{
    int bezel_width  = bounds.width / 8;
    DrawRectangleRec(bounds, color);
    draw_top_beveled_edge(
	bounds.x,
	bounds.y,
	bounds.width,
	bezel_width,
	SHAD_COLOR_LT);
    draw_left_beveled_edge(
	bounds.x,
	bounds.y,
	bounds.width,
	bezel_width,
	SHAD_COLOR_LT);
    draw_right_beveled_edge(
	bounds.x + bounds.width - bezel_width,
	bounds.y,
	bounds.width,
	bezel_width,
	SHAD_COLOR_DK);
    draw_bottom_beveled_edge(
	bounds.x,
	bounds.y + bounds.width - bezel_width,
	bounds.width,
	bezel_width,
	SHAD_COLOR_DK);
}

void draw_smiley(Texture smiley, float scale)
{
    int bounds_width = HDR_HEIGHT - (2 * BDR_WIDTH) - (2 * SHAD_WIDTH) - (3 * SCORE_PADDING);
    Rectangle bounds =  (Rectangle) {
	WIDTH / 2.0 - bounds_width / 2.0,
	HDR_HEIGHT / 2.0 - bounds_width / 2.0,
	bounds_width,
	bounds_width
    };
    draw_button(bounds, BG_COLOR);
    int x = (int)(WIDTH / 2.0 - smiley.width * scale / 2.0);
    int y = (int)(HDR_HEIGHT / 2.0 - smiley.height * scale / 2.0);
    DrawTextureEx(smiley, (Vector2){x, y}, 0.0, scale, RAYWHITE);
}

void draw_cells(RenderState *render_state)
{
    for (size_t i = 0; i < render_state->cell_count; i++) {
        Cell cell = render_state->cells[i];
	size_t row = i / COLS;
	size_t col = i % COLS;
	int x = PLAY_BOUNDS.x + (col * CELL_WIDTH);
	int y = PLAY_BOUNDS.y + (row * CELL_WIDTH);
	if (cell.state == CS_COVERED) {
	    draw_button((Rectangle) {x, y, CELL_WIDTH, CELL_WIDTH}, BG_COLOR);
	}

	if (cell.state == CS_FLAGGED) {
	    draw_button((Rectangle) {x, y, CELL_WIDTH, CELL_WIDTH}, RED);
	}
    }

}

void draw(RenderState *render_state)
{
    BeginDrawing();
    ClearBackground(BG_COLOR);
    draw_header_borders();
    draw_game_borders();
    draw_mines_remaining(render_state->mines_remaining, render_state->score_font);
    draw_elapsed_time(render_state->time_elapsed, render_state->score_font);
    draw_smiley(render_state->face_tex, render_state->face_scale);
    draw_grid();
    draw_cells(render_state);
    EndDrawing();
}

Input collect_input(void)
{
    Action act;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE)) {
	act = REVEAL;
    } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsKeyPressed(KEY_M)) {
	act = MARK;
    } else {
	act = NONE;
    }
    return (Input) { .mouse_pos = GetMousePosition(), .action = act };
}

RenderState init_render_state(char *font_path, char *smiley_img_path)
{
    Image smiley_img = LoadImage(smiley_img_path);
    const size_t cell_count = ROWS * COLS;
    Cell *cells = malloc(sizeof(Cell) * cell_count);
    for (size_t i = 0; i < cell_count; i++) {
	cells[i] = (Cell) { .state = CS_COVERED};
    }

    RenderState render_state = {
	.score_font = LoadFont(font_path),
	.face_tex = LoadTextureFromImage(smiley_img),
	.face_scale = 0.05,
	.time_elapsed = 102,
	.mines_remaining = 99,
	.cells = cells,
	.cell_count = cell_count,
	.mouse_pos = (Vector2) GetMousePosition(),
    };
    UnloadImage(smiley_img);
    return render_state;
}

void destroy_render_state(RenderState *state) {
    UnloadTexture(state->face_tex);
    UnloadFont(state->score_font);
    free(state->cells);
    state->cells = NULL;
    state = NULL;
}

bool cell_for_pos(Vector2 mouse_pos, Vector2 *cell_pos)
{
    bool pointer_in_play_bounds = CheckCollisionPointRec(mouse_pos, PLAY_BOUNDS);
    if (pointer_in_play_bounds) {
	cell_pos->y = (mouse_pos.y - PLAY_BOUNDS.y) / CELL_WIDTH;
	cell_pos->x = (mouse_pos.x - PLAY_BOUNDS.x) / CELL_WIDTH;
    }
    return pointer_in_play_bounds;
}

void update_rs(RenderState *render_state, Board *board)
{
    for (int row = 0; row < board->rows; row++) {
	for (int col = 0; col < board->cols; col++) {
	    size_t cell_index = row * board->cols + col;
	    Cell *render_cell = &render_state->cells[cell_index];
	    switch (mark_at(board, row, col)) {
	    case MK_NONE: break;
	    case MK_FLAG: render_cell->state = CS_FLAGGED; break;
	    case MK_REVEALED: render_cell->state = CS_REVEALED; break;
	    }
	}
    }
}

void update(Board *b, Input in)
{
    Vector2 cell_index;
    if (cell_for_pos(in.mouse_pos, &cell_index)) {
	size_t row = cell_index.y;
	size_t col = cell_index.x;
	switch (in.action) {
	case MARK:
	    toggle_flag(b, row, col);
	    break;
	case REVEAL: reveal_at(b, row, col); break;
	case NONE: break;
	}
    }
}

int main(void)
{
    char *font_path = "./resources/fonts/fonts-DSEG_v046/DSEG7-Classic-MINI/DSEG7ClassicMini-Regular.ttf";
    char *smiley_img_path = "./resources/images/1F642_color.png";
    char *title = "Mines";
    Board b = make_board(ROWS, COLS, NUM_MINES);

    InitWindow(WIDTH, HEIGHT, title);
    RenderState render_state = init_render_state(font_path, smiley_img_path);
    while (!WindowShouldClose())
    {
	Input in = collect_input();
	update(&b, in);
	update_rs(&render_state, &b);
	draw(&render_state);
    }

    destroy_board(&b);
    destroy_render_state(&render_state);
    CloseWindow();
    return EXIT_SUCCESS;
}
