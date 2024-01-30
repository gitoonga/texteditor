#include "kilo.h"

/**
 * die - Print an error message and terminate the program
 * @s: The error message to print
 *
 * This function prints an error message using perror and then terminates
 * the program with exit code 1.
 */
void die(const char *s)
{
    editorClearScreen();

    perror(s);
    exit(1);
}