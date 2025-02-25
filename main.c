/* main.c */
#include <stdio.h>
#include "raylib.h"

// BG Color

// Grid dimensions
#define ROWS (9) //(16)
#define COLS (9) //(30)

#define CELL_WIDTH (25)

// Header (constant)
#define HDR_HEIGHT (80)

// Borders
#define BDR_WIDTH (8)
#define SHAD_WIDTH (4)


static const Color BG_COLOR  = (Color) {150, 150, 150, 255};
static const Color BDR_COLOR = (Color) {160, 160, 160, 255};
static const Color SHAD_COLOR_DK = (Color) {100, 100, 100, 255};
static const Color SHAD_COLOR_LT = (Color) {200, 200, 200, 255};
static const Color GRID_COLOR = GREEN;

static const size_t HEIGHT =
    HDR_HEIGHT + (CELL_WIDTH * ROWS) + (2 * SHAD_WIDTH) + BDR_WIDTH;

static const size_t WIDTH =
    (CELL_WIDTH * COLS) + (2 * SHAD_WIDTH) + (2 * BDR_WIDTH);


// Padding
#define SCORE_PADDING (6)
#define SCORE_KERNING (1)

// TODO: conider combining draw_top.. and draw_bottom. The only difference is
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

void draw_score_panel(size_t x, size_t y, size_t width, size_t height, size_t score, Font font)
{
    int v_offset = 8;
    DrawRectangle(x, y, width, height, BLACK);
    const size_t BUF_SIZE = 4;
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

void draw_button(Rectangle bounds)
{
    int bezel_width  = bounds.width / 8;
    DrawRectangleLinesEx(bounds, 1.0, BLACK);
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
    draw_button(bounds);
    int x = (int)(WIDTH / 2.0 - smiley.width * scale / 2.0);
    int y = (int)(HDR_HEIGHT / 2.0 - smiley.height * scale / 2.0);
    DrawTextureEx(smiley, (Vector2){x, y}, 0.0, scale, RAYWHITE);
}

void draw(Font font, Texture smiley, float scale)
{
    draw_header_borders();
    draw_game_borders();
    draw_mines_remaining(99, font);
    draw_elapsed_time(3, font);
    draw_smiley(smiley, scale);
    draw_grid();
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Mines");
    Font font = LoadFont("./resources/fonts/fonts-DSEG_v046/DSEG7-Classic-MINI/DSEG7ClassicMini-Regular.ttf");
    Image smiley_img = LoadImage("./resources/images/1F642_color.png");
    Texture smiley_tex = LoadTextureFromImage(smiley_img);
    UnloadImage(smiley_img);
    float scale = 0.05;

    while (!WindowShouldClose())
    {
        BeginDrawing();
	ClearBackground(BG_COLOR);
	draw(font, smiley_tex, scale);
        EndDrawing();
    }

    UnloadFont(font);
    UnloadTexture(smiley_tex);
    CloseWindow();
    return 0;
}
