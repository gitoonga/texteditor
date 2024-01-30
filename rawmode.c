#include "kilo.h"

/**
 * disableRawMode - Disable raw mode for terminal input
 *
 * This function restores the original terminal attributes, disabling raw mode
 * for terminal input.
 */
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &globalConfig.orig_termios) == -1)
        die("tcsetattr");
}

/**
 * enableRawMode - Enable raw mode for terminal input
 *
 * This function modifies the terminal attributes to enable raw
 * mode for terminal input.
 * In raw mode, input characters are not automatically echoed
 * to the screen, providing more control over the
 * input handling process.
 */
void enableRawMode()
{

    /** Get the current terminal attributes and store
     * them in the 'orig_termios' structure
     */
    if (tcgetattr(STDIN_FILENO, &globalConfig.orig_termios) == -1)
        die("tcgetattr");
    /**
     * Ensure we leave the terminal attributes the way we
     * found them when our program exits, whether it exits by
     * returning from main(), or by calling the exit() function.
     */
    atexit(disableRawMode);

    /** Copy of the orig_termios struct which we'll target with
     *  our changes
     */
    struct termios raw = globalConfig.orig_termios;

    /**
     * The IXON flag Disables Ctrl-S and Ctrl-Q (start/stop
     * output control)
     * The ICRNL Maps CR to NL on input. Now Ctrl-M will
     * print 13 not 10
     * BRKINT - a break condition will cause a SIGINT signal to
     * be sent to the program
     * INPCK enables parity checking
     * ISTRIP causes the 8th bit of each input byte to be
     * stripped, meaning it will set it to 0.
     */
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);

    /**
     * The OPOST flag will turn off Post-process output.
     */
    raw.c_oflag &= ~(OPOST);

    /**
     * CS8 is not a flag, it is a bit mask with multiple bits,
     * which we set using the bitwise-OR (|) operator unlike all
     * the flags we are turning off. It sets the character size
     * (CS) to 8 bits per byte
     */
    raw.c_cflag |= (CS8);

    /**
     * Modify the 'c_lflag' field in the 'raw' structure:
     * The 'ECHO' flag is cleared, disabling automatic echoing
     * of input characters
     * The ICANON flag allows us to turn off canonical mode.
     * This means we will be reading input byte-by-byte,
     * instead of line-by-line.
     * The IEXTEN flag turns off implementation-defined input
     * processing. (when you type Ctrl-V, the terminal waits for
     * you to type another character and then sends that
     * character literally)
     * The ISIG flag Turns off Ctrl-C and Ctrl-Z signals.
     */
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    /**
     * The VMIN value sets the minimum number of bytes of input
     * needed before read() can return. We set it to 0 so that
     * read() returns as soon as there is any input to be read.
     */
    raw.c_cc[VMIN] = 0;

    /**
     * The VTIME value sets the maximum amount of time to wait
     * before read() returns. It is in tenths of a second, so we
     * set it to 1/10 of a second, or 100 milliseconds. If read()
     * times out, it will return 0, which makes sense because its
     * usual return value is the number of bytes read.
     */
    raw.c_cc[VTIME] = 1;

    /**
     * Set the modified terminal attributes back to the terminal:
     * TCSAFLUSH specifies that the change should occur after all
     * output has been transmitted
     */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}