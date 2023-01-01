
all: numgame.c console_util.o
	gcc -Wall -Wextra -pedantic -std=c99 numgame.c console_util.o -o numgame.exe

console_util.o: console_util.c
	gcc -Wall -Wextra -pedantic -std=c99 -c console_util.c -o console_util.o


.PHONY: clean
clean:
	rm console_util.o
	rm numgame.exe
