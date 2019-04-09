#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "game_of_life.h"

#define ROWS 20
#define COLS 40

void draw_top() {
}

void draw_board(int32 *board, int generation) {
	printw("Generation %d\n", generation);
	printw("\n");

	printw(" ");
	for (int i = 0; i < COLS; i++) {
		printw("_");
	}
	printw("\n");
	for (int j = 0; j < ROWS; j++) {
		printw("|");
		for (int i = 0; i < COLS; i++) {
			if (get_board_value(board, ROWS, COLS, i, j) == 1) {
				printw("*");
			} else {
				printw(" ");
			}
		}
		printw("|\n");
	}

	printw(" ");
	for (int i = 0; i < COLS; i++) {
		printw("_");
	}
	printw("\n");
}

int main() {
	initscr();

	int generation = 0;

	int32 *board = malloc(ROWS*COLS*sizeof(int32));
	int32 *board_2 = malloc(ROWS*COLS*sizeof(int32));

	assert(board != 0);
	assert(board_2 != 0);

	// everyone loves a glider
	set_board_value(board, ROWS, COLS, 0, 1, 1);
	set_board_value(board, ROWS, COLS, 1, 2, 1);
	set_board_value(board, ROWS, COLS, 2, 0, 1);
	set_board_value(board, ROWS, COLS, 2, 1, 1);
	set_board_value(board, ROWS, COLS, 2, 2, 1);

  while (true) {
		clear();

		next_generation(board_2, board, ROWS, COLS);
		draw_board(board, generation);

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
