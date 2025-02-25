/* main.c */
#include <_stdio.h>
#include <stdio.h>
#include "raylib.h"

// BG Color
#define BG_COLOR ((Color) {150, 150, 150, 255})


// Screen sizes
#define W (16)
#define H (9)
#define SCREEN_FACT (60)
#define WIDTH (W * SCREEN_FACT)
#define HEIGHT (H * SCREEN_FACT)

// Header
#define H_HEIGHT (85)

// Borders
#define B_WIDTH (8)
#define B_COLOR  ((Color){160, 160, 160, 255})
#define SHADOW_WIDTH (4)
#define SHADOW_COLOR_DK ((Color){100, 100, 100, 255})
#define SHADOW_COLOR_LT ((Color){200, 200, 200, 255})

// Padding
#define SCORE_PADDING (8)
#define SCORE_KERNING (2)

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
    draw_top_beveled_edge(0, 0, WIDTH, B_WIDTH, B_COLOR);
    draw_top_beveled_edge(B_WIDTH, B_WIDTH, WIDTH - (2 * B_WIDTH), SHADOW_WIDTH, SHADOW_COLOR_DK);

    draw_left_beveled_edge(0, 0, H_HEIGHT, B_WIDTH, B_COLOR);
    draw_left_beveled_edge(B_WIDTH, B_WIDTH, H_HEIGHT - (2 * B_WIDTH), SHADOW_WIDTH, SHADOW_COLOR_DK);

    draw_bottom_beveled_edge(0, H_HEIGHT - B_WIDTH, WIDTH, B_WIDTH, B_COLOR);
    draw_bottom_beveled_edge(B_WIDTH, H_HEIGHT - SHADOW_WIDTH - B_WIDTH, WIDTH - (2 * B_WIDTH), SHADOW_WIDTH, SHADOW_COLOR_LT);

    draw_right_beveled_edge(WIDTH - B_WIDTH, 0, H_HEIGHT, B_WIDTH, B_COLOR);
    draw_right_beveled_edge(WIDTH - B_WIDTH - SHADOW_WIDTH, B_WIDTH, H_HEIGHT - (2 * B_WIDTH), SHADOW_WIDTH, SHADOW_COLOR_LT);
}

void draw_game_borders(void)
{
    draw_top_beveled_edge(0, H_HEIGHT - B_WIDTH, WIDTH, B_WIDTH, B_COLOR);
    draw_top_beveled_edge(B_WIDTH, H_HEIGHT, WIDTH - (2 * B_WIDTH), SHADOW_WIDTH, SHADOW_COLOR_DK);

    draw_left_beveled_edge(0, H_HEIGHT - B_WIDTH, HEIGHT - H_HEIGHT + B_WIDTH, B_WIDTH, B_COLOR);
    draw_left_beveled_edge(B_WIDTH, H_HEIGHT, HEIGHT - H_HEIGHT - B_WIDTH, SHADOW_WIDTH, SHADOW_COLOR_DK);

    draw_bottom_beveled_edge(0, HEIGHT - B_WIDTH, WIDTH, B_WIDTH, B_COLOR);
    draw_bottom_beveled_edge(B_WIDTH, HEIGHT - B_WIDTH - SHADOW_WIDTH, WIDTH - (2 * B_WIDTH), SHADOW_WIDTH, SHADOW_COLOR_LT);

    draw_right_beveled_edge(WIDTH - B_WIDTH, H_HEIGHT - B_WIDTH, HEIGHT - H_HEIGHT + B_WIDTH, B_WIDTH, B_COLOR);
    draw_right_beveled_edge(WIDTH - B_WIDTH - SHADOW_WIDTH, H_HEIGHT, HEIGHT - H_HEIGHT - B_WIDTH, SHADOW_WIDTH, SHADOW_COLOR_LT);
}

void draw_score_panel(size_t x, size_t y, size_t width, size_t height, size_t score, Font font)
{
    DrawRectangle(x, y, width, height, BLACK);
    const size_t BUF_SIZE = 4;
    char text[BUF_SIZE] = { 0 };
    snprintf(text, BUF_SIZE, "%03zu", score);
    DrawTextEx(font, text, (Vector2){x + (2 * SCORE_KERNING), y}, (float)height, SCORE_KERNING, RED);
    /* (float)font.baseSize * 1.4 */
}

void draw_mines_remaining(size_t mines_count, Font font)
{
    size_t panel_height = H_HEIGHT - (2 * B_WIDTH) - (2 * SHADOW_WIDTH) - (2 * SCORE_PADDING);
    size_t panel_width = ((panel_height * 3) / 2) + (2 * SCORE_KERNING);
    size_t panel_x = B_WIDTH + SHADOW_WIDTH + SCORE_PADDING;
    size_t panel_y = B_WIDTH + SHADOW_WIDTH + SCORE_PADDING;
    draw_score_panel(panel_x, panel_y, panel_width, panel_height, mines_count, font);
}

void draw_elapsed_time(size_t time_elapsed, Font font)
{
    size_t panel_height = H_HEIGHT - (2 * B_WIDTH) - (2 * SHADOW_WIDTH) - (2 * SCORE_PADDING);
    size_t panel_width = ((panel_height * 3) / 2) + (2 * SCORE_KERNING);
    size_t panel_x = WIDTH - B_WIDTH - SHADOW_WIDTH - SCORE_PADDING - panel_width;
    size_t panel_y = B_WIDTH + SHADOW_WIDTH + SCORE_PADDING;
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
	SHADOW_COLOR_LT);
    draw_left_beveled_edge(
	bounds.x,
	bounds.y,
	bounds.width,
	bezel_width,
	SHADOW_COLOR_LT);
    draw_right_beveled_edge(
	bounds.x + bounds.width - bezel_width,
	bounds.y,
	bounds.width,
	bezel_width,
	SHADOW_COLOR_DK);
    draw_bottom_beveled_edge(
	bounds.x,
	bounds.y + bounds.width - bezel_width,
	bounds.width,
	bezel_width,
	SHADOW_COLOR_DK);
}

void draw_smiley(Texture smiley, float scale)
{
    int bounds_width = WIDTH / 20;
    Rectangle bounds =  (Rectangle) {
	WIDTH / 2.0 - bounds_width / 2.0,
	H_HEIGHT / 2.0 - bounds_width / 2.0,
	bounds_width,
	bounds_width
    };
    draw_button(bounds);
    int x = (int)(WIDTH / 2.0 - smiley.width * scale / 2.0);
    int y = (int)(H_HEIGHT / 2.0 - smiley.height * scale / 2.0);
    DrawTextureEx(smiley, (Vector2){x, y}, 0.0, scale, RAYWHITE);
}

void draw(Font font, Texture smiley, float scale)
{
    draw_header_borders();
    draw_game_borders();
    draw_mines_remaining(99, font);
    draw_elapsed_time(3, font);
    draw_smiley(smiley, scale);
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Mines");
    Font font = LoadFont("./resources/fonts/digital7mono/Digital7Mono-Yz9J4.ttf");
    Image smiley_img = LoadImage("./resources/images/1F642_color.png");
    Texture smiley_tex = LoadTextureFromImage(smiley_img);
    UnloadImage(smiley_img);
    float scale = 0.075;

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
