#include "types.h"
#include "common.h"

int
constrain(int x, int bound);

int32
get_board_value(const int32 *board, int32 rows, int32 cols, int32 x, int32 y);

void
set_board_value(int32 *board, int32 rows, int32 cols, int32 x, int32 y, int32 value);

int32
next_generation(int32 *next, const int32 *prev, int32 rows, int32 cols);
