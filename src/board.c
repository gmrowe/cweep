/* board.c */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "board.h"

const uint8_t MINE_VAL = 100;

bool in_bounds(Board *board, int row, int col)
{
    return row >= 0
	&& row < board->rows
	&& col >= 0
	&& col < board->cols;
}

static inline int idx(Board *board, int row, int col)
{
    return row * board->cols + col;
}

uint8_t value_at(Board *board, int row, int col)
{
    return board->values[idx(board, row, col)];
}

bool is_revealed_at(Board *board, int row, int col)
{
    return board->marks[(idx(board, row, col))] == MK_REVEALED;
}

Mark mark_at(Board *board, int row, int col)
{
    return board->marks[(idx(board, row, col))];
}

bool reveal_at(Board *board, int row, int col)
{
    int i = idx(board, row, col);
    if (board->marks[i] == MK_FLAG || board->marks[i] == MK_REVEALED) {
        // We do not reveal flagged, or previously revealed cells
	return false;
    }

    board->marks[i] = MK_REVEALED;

    // Flood reveal zeroes
    if (board->values[i] == 0) {
	for (int r = row - 1; r < row + 2; r++) {
	    for (int c = (int)col - 1; c < (int)col + 2; c++) {
		if (in_bounds(board, r, c)) {
		    reveal_at(board, r, c);
		}
	    }
	}
    }
    return true;
}

bool is_win(Board *board)
{
    for (size_t i = 0; i < board->count; i++) {
	if (board->values[i] != MINE_VAL && board->marks[i] != MK_REVEALED) {
	    return false;
	}
    }
    return true;
}

bool is_loss(Board *board) {
    for (size_t i = 0; i < board->count; i++) {
	if (board->values[i] == MINE_VAL && board->marks[i] == MK_REVEALED) {
	    return true;
	}
    }
    return false;
}

bool toggle_flag(Board *board, int row, int col)
{
    int i = idx(board, row, col);
    switch (board->marks[i]) {
    case MK_REVEALED: return false;
    case MK_NONE: board->marks[i] = MK_FLAG; return true;
    case MK_FLAG: board->marks[i] = MK_NONE; return true;
    }
}

static bool place_mine_at_idx(Board *board, int index) {
    assert((size_t)index < board->count && "Index out of range!");
    // Don't place a mine in a square that already has one
    if (board->values[index] == MINE_VAL) {
	return false;
    }

    board->values[index] = MINE_VAL;
    int row = index / board->cols;
    int col = index % board->cols;
    for (int r = (int)row - 1; r < row + 2; r++) {
	for (int c = (int)col - 1; c < col + 2; c++) {
	    if (in_bounds(board, r, c)) {
		int i = idx(board, r, c);
		if (board->values[i] != MINE_VAL) {
		    board->values[i] += 1;
		}
	    }
	}
    }
    return true;
}


// An alternate algorithm would be to shuffle the indices of the array
// and then place bombs at the first num_mines indices. This would eliminate
// collisions at higher mine densities and would likely be faster. But
// this is easier to implement and seems to be fast enough
static void place_random_mines(Board *board, int num_mines)
{
    // Keep trying to place mines at random until num_mines are placed
    while (num_mines > 0) {
	int idx = arc4random() % board->count;
	if (place_mine_at_idx(board, idx)) {
	    num_mines -= 1;
	}
    }
}

Board make_board(int rows, int cols, int num_mines)
{
    assert(rows > 0 && cols > 0);
    size_t count = (size_t)(rows * cols);
    uint8_t *values = calloc(count, sizeof(uint8_t));
    Mark *marks = calloc(count, sizeof(Mark));
    Board b = (Board) { values, marks, rows, cols, count };
    place_random_mines(&b, num_mines);
    return b;
}

void destroy_board(Board *board)
{
    free(board->values);
    board->values = NULL;
    free(board->marks);
    board->marks = NULL;
    board = NULL;
}

void dump_revealed_board(Board *board) {
    for (int row = 0; row < board->rows; row++) {
	for (int col = 0; col < board->cols; col++) {
	    if (board->values[idx(board, row, col)] == MINE_VAL) {
		putchar('*');
	    } else {
		putchar('0' + board->values[idx(board, row, col)]);
	    }
	}
	putchar('\n');
    }
    putchar('\n');
}

void dump_board(Board *board) {
    for (int row = 0; row < board->rows; row++) {
	for (int col = 0; col < board->cols; col++) {
	    switch (mark_at(board, row, col)) {
	    case MK_REVEALED: {
		uint8_t val = value_at(board, row, col);
		char glyph;
		switch (val) {
		case MINE_VAL: glyph = '*';       break;
		case 0:        glyph = '.';       break;
		default:       glyph = '0' + val; break;
		}
		putchar(glyph);
		break;
	    };
	    case MK_FLAG: {
		putchar('F');
		break;
	    };

	    case MK_NONE: {
		putchar('@');
	    }
	    }
	}
	putchar('\n');
    }
    putchar('\n');
}
