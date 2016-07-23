/* bounce_async.c
 * purpose: animation with user control, using O_ASYNC on fd
 * note: set_ticker() sends SIGALRM, handler does animation
 *       keyboard sends SIGIO, main only calls pause()
 */
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <curses.h>
#include <sys/time.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

main()
{
    void on_alarm(int);     // handler for alarm
    void on_input(int);     // handler for keybd
    void enable_kbd_signals();

    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);        // install the handler
    enable_kbd_signals();
    signal(SIGALRM, on_alarm);
    set_ticker(delay);              // start ticking

    move(row, col);
    addstr(MESSAGE);

    while (1)
    {
        if (done == 1) break;
        pause();
    }
    endwin();
}

void on_input(int signum)
{
    int c = getch();

    if (c == 'Q' || c == EOF)
    {
        done = 1;
    }
    else if (c == ' ')
    {
        dir = -dir;
    }
}

void on_alarm(int signum)
{
    signal(SIGALRM, on_alarm);          // reset, just in case
    mvaddstr(row, col, BLANK);
    col += dir;
    mvaddstr(row, col, MESSAGE);
    refresh();

    // now handle borders
    if (dir == -1 && col <= 0)
        dir = 1;
    else if (dir == 1 && col + strlen(MESSAGE) >= COLS)
        dir = -1;
}

/*
 * install a handler, tell kernel who to notify on input, enable
 * signals
 */
void enable_kbd_signals()
{
    int fd_flags;

    fcntl(0, F_SETOWN, getpid());
    fd_flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
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
