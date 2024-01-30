#ifndef KILO_H
#define KILO_H

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

//Structs

struct termios orig_termios;

struct editorConfig {
    int screenrows;
    int screencols;
    struct termios orig_termios;    
};

struct editorConfig globalConfig;

/**
 * @brief Extracts the control character from a key code.
 * This macro takes a key code as input and masks it with 0x1f,
 * effectively extracting the corresponding control character.
 * Control characters are obtained by setting the lower 5 bits of
 * the key code to 0.
 *
 * @param k The key code to extract the control character from.
 * @return The control character obtained by masking the key code.
 */
#define CTRL_KEY(k) ((k) & 0x1f)

void disableRawMode();
void enableRawMode();
void die(const char *s);
char editorReadKey();
void editorProcessKeyPress();
void editorClearScreen();
void editorRefreshScreen();
void editorDrawRows();
void initEditor();
int getCursorPosition(int *rows, int *cols);

#endif