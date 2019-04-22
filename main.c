#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "game_of_life.h"

void draw_board(int32 *board, int rows, int cols, int generation) {
	addch(ACS_ULCORNER);
	for (int i = 0; i < cols; i++) {
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);
	for (int j = 0; j < rows; j++) {
		move(j+1,0);

		addch(ACS_VLINE);
		for (int i = 0; i < cols; i++) {
			if (get_board_value(board, rows, cols, i, j) == 1) {
				addch('*');
			} else {
				addch(' ');
			}
		}
		addch(ACS_VLINE);
	}
	move(rows+1, 0);

	addch(ACS_LLCORNER);
	for (int i = 0; i < cols; i++) {
		addch(ACS_HLINE);
	}
	addch(ACS_LRCORNER);

	move(0, 0);
	printw("Generation %d", generation);
}


void make_glider(int32 *board, int rows, int cols, int32 x, int32 y) {
	// everyone loves a glider
	set_board_value(board, rows, cols, x+0, y+1, 1);
	set_board_value(board, rows, cols, x+1, y+2, 1);
	set_board_value(board, rows, cols, x+2, y+0, 1);
	set_board_value(board, rows, cols, x+2, y+1, 1);
	set_board_value(board, rows, cols, x+2, y+2, 1);
}

// clear out an entire board so all data is set to 0
// prevents oddities when resizing
void clear_board(int32 *board, int rows, int cols) {
	for (int y=0; y < rows; y++) {
		for (int x=0; x< cols; x++) {
			set_board_value(board, rows, cols, x, y, 0);
		}
	}
}

void copy_board(int32 *from, int from_rows, int from_cols, int32 *to, int to_rows, int to_cols) {
	clear_board(to, to_rows, to_cols);

	for (int y=0; y < from_rows; y++) {
		for (int x=0; x< from_cols; x++) {
			if (x >= to_cols || y >= to_rows) continue;
			int val = get_board_value(from, from_rows, from_cols, x, y);
			set_board_value(to, to_rows, to_cols, x, y, val);
		}
	}
}

void swap(int32 **a, int32 **b)
{
	int32 *tmp = *a;
	*a = *b;
	*b = tmp;
}

int main() {
	WINDOW *window = initscr();
	// allow looping over getch without blocking
	timeout(1);

	int generation = 0;

	int rows = LINES - 2;
	int cols = COLS - 4;

	// this means would could make a board up to 10x the size
	// that should be very sufficient
	int extra = 10;
	int32 *board = malloc(extra*rows*cols*sizeof(int32));
	int32 *board_2 = malloc(extra*rows*cols*sizeof(int32));

	// verify that the boards exist
	assert(board != 0);
	assert(board_2 != 0);

	make_glider(board, rows, cols, 0, 0);
	make_glider(board, rows, cols, 10, 10);

	int key;
	while ((key = wgetch(window)) != 27) {
		if (key == KEY_RESIZE) {
			int prev_rows = rows;
			int prev_cols = cols;

			rows = LINES - 2;
			cols = COLS - 4;

			// reuse the previous generation to rebuild the board state at the new size
			// hopefully nobody dies in the process
			// Some weird things happen sometimes, but that's part of the fun I suppose
			copy_board(board_2, prev_rows, prev_cols, board, rows, cols);

			continue;
		}

		next_generation(board_2, board, rows, cols);

		clear();
		draw_board(board, rows, cols, generation);

		usleep(100*1000);

		refresh();

		generation++;

		swap(&board, &board_2);
	}

	endwin();

	return 0;
}
