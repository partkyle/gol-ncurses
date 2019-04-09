
build: target/game

target:
	mkdir -p target

target/game: target main.c target/game_of_life.o
	cc -l ncurses main.c target/game_of_life.o -o target/game

target/game_of_life.o: target game_of_life.c game_of_life.h
	cc -shared game_of_life.c -o target/game_of_life.o

clean:
	rm -rf target
.PHONY: clean
