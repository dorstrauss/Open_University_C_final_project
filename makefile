assembler: main.o
	gcc -ansi -Wall -pedantic main.o -o assembler
main.o: main.c main.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
