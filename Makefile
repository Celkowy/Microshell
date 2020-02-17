CC=clang
CFLAGS=-WALL -g

microshell:
	gcc microshell.c -o microshell

clean:
	rm -f microshell microshell.o
