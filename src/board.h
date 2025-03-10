/* board.h */
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    MK_NONE = 0,
    MK_FLAG,
    MK_REVEALED,
} Mark;

typedef struct {
    uint8_t *values;
    Mark    *marks;
    int     rows;
    int     cols;
    size_t count;
} Board;

extern const uint8_t MINE_VAL;

bool in_bounds(Board *, int, int);
uint8_t value_at(Board *, int, int);
bool is_revealed_at(Board *, int, int);
Mark mark_at(Board *, int, int);
bool reveal_at(Board *, int, int);
bool is_win(Board *);
bool is_loss(Board *);
bool toggle_flag(Board *, int, int);
Board make_board(int, int, int);
void destroy_board(Board *);

#endif /* BOARD.H */
