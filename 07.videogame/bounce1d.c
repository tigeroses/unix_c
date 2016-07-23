/* bounce1d.c
 * purpose: animation with user controlled speed and direction
 * note: the handler does the animation
 *       the main program reads keyboard input
 */
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>

// some global settings main and the handler use
#define MESSAGE "hello"
#define BLANK   "     "

int row;
int col;
int dir;

int main()
{
    int delay;
    int ndelay;
    int c;
    void move_msg(int);

    initscr();
    crmode();
    noecho();
    clear();

    row = 10;
    col = 0;
    dir = 1;
    delay = 200;        // 200ms = 0.2seconds

    move(row, col);
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while (1)
    {
        ndelay = 0;
        c = getch();
        if (c == 'Q') break;
        if (c == ' ') dir = -dir;
        if (c == 'f' && delay > 2) ndelay = delay/2;
        if (c == 's') ndelay = delay * 2;
        if (ndelay > 0)
            set_ticker(delay = ndelay);
    }
    endwin();

    return 0;
}

void move_msg(int signum)
{
    signal(SIGALRM, move_msg);  // reset, just in case
    move(row, col);
    addstr(BLANK);
    col += dir;                 // move to new column
    move(row, col);             // then set cursor
    addstr(MESSAGE);            // redo message
    refresh();

    // now handle borders
    if (dir == -1 && col <= 0)
        dir = 1;
    else if (dir == 1 && col+strlen(MESSAGE) >= COLS)
        dir = -1;
}

/* [from set_ticker.c]
 * set_ticker(number_of_milliseconds)
 * arranges for interval timer to issue SIGALRMs at regular intervals
 * returns -1 on error, 0 for ok
 * arg in milliseconds, converted into whole seconds and mocroseconds
 * note: set_ticker(0) turns off ticker
 */
int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;             // int part
    n_usecs = (n_msecs % 1000) * 1000L; // remainder

    new_timeset.it_interval.tv_sec = n_sec;    // set reload
    new_timeset.it_interval.tv_usec = n_usecs; // new ticker value

    new_timeset.it_value.tv_sec = n_sec;    // store this
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
