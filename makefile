all:
	gcc -Wall main.c main_header.h functions.c debug.c -o ./testdir/find -lm
