#include "kilo.h"

/**
 * @brief Reads a single keypress from the standard input.
 *
 * This function reads a single character from the standard input and
 * returns it. It ensures that the read operation is non-blocking and
 * retries until a keypress is detected.
 *
 * @return The character representing the keypress.
 */
char editorReadKey()
{
    int bytesRead;
    char key;

    while ((bytesRead = read(STDIN_FILENO, &key, 1) != 1))
    {
        if (bytesRead == -1 && errno != EAGAIN)
        {
            die("read");
        }
    }
    return key;
}

/**
 * @brief Processes a keypress in the editor.
 *
 * This function reads a single keypress using the editorReadKey
 * function and performs actions based on the detected key.
 */
void editorProcessKeyPress()
{
    char key = editorReadKey();

    switch (key)
    {
    case CTRL_KEY('q'):
        /**
         * @note Exits the editor gracefully upon detecting 'Ctrl-Q'.
         *       The editor state is not preserved.
         */
        editorClearScreen();
        exit(0);
        break;
    }
}

void editorClearScreen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void editorRefreshScreen()
{
    editorClearScreen();
    editorDrawRows();

    write(STDOUT_FILENO, "\x1b[H", 3);
}

void editorDrawRows()
{
    int y;
    for (y = 0; y < globalConfig.screenrows; y++)
    {
        write(STDOUT_FILENO, "~\r\n", 3);
    }
}

int getWindowSize(int *rows, int *cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    // If ioctl fails or the column size is 0, write ANSI escape sequences to move the cursor far right and bottom
    {
        if (write(STDOUT_FILENO, "\xb[999C\xb[999B", 12) != 12)
            return -1;
        return getCursorPosition(rows, cols);
    }
    else
    {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

int getCursorPosition(int *rows, int *cols)
{
    if (write(STDOUT_FILENO, "\xb[6n", 4) != 4)
        return -1;

    printf("\r\n");

    char c;

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d('%c')\r\n", c, c);
        }
    }
    editorReadKey();

    return -1;
}

void initEditor()
{
    if (getWindowSize(&globalConfig.screenrows, &globalConfig.screencols) == -1)
        die("getWindowSize");
}
