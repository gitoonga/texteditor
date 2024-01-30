CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

kilo: kilo.c rawmode.c
	$(CC) kilo.c rawmode.c errors.c editor.c -o kilo $(CFLAGS)
