#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "game_of_life.h"

void draw_board(int32 *board, int rows, int cols, int generation) {
	printw("Generation %d\n", generation);
	printw("\n");

	printw(" ");
	for (int i = 0; i < cols; i++) {
		printw("_");
	}
	printw("\n");
	for (int j = 0; j < rows; j++) {
		printw("|");
		for (int i = 0; i < cols; i++) {
			if (get_board_value(board, rows, cols, i, j) == 1) {
				printw("*");
			} else {
				printw(" ");
			}
		}
		printw("|\n");
	}

	printw(" ");
	for (int i = 0; i < cols; i++) {
		printw("_");
	}
	printw("\n");
}


void make_glider(int32 *board, int rows, int cols, int32 x, int32 y) {
	// everyone loves a glider
	set_board_value(board, rows, cols, x+0, y+1, 1);
	set_board_value(board, rows, cols, x+1, y+2, 1);
	set_board_value(board, rows, cols, x+2, y+0, 1);
	set_board_value(board, rows, cols, x+2, y+1, 1);
	set_board_value(board, rows, cols, x+2, y+2, 1);
}

int main() {
	initscr();
	// allow looping over getch without blocking
	timeout(1);

	int generation = 0;

	int rows = LINES - 5;
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
	while ((key = getch()) != 27) {
		if (key == KEY_RESIZE) {
			rows = LINES - 5;
			cols = COLS - 4;
			continue;
		}

		clear();

		next_generation(board_2, board, rows, cols);
		draw_board(board, rows, cols, generation);

		usleep(100*1000);

		refresh();
		generation++;

		int *tmp;
		tmp = board_2;
		board_2 = board;
		board = tmp;
	}

	endwin();

	return 0;
}
