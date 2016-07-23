/* bounce_aio.c
 * purpose: animation with user control, using aio_read() etc
 * note: set_ticker() sends SIGALRM, handler does animation
 * keyboard sends SIGIO, main only calls pause()
 * compile: cc bounce_aio.c -lrt -l curses -o bounce_aio
 */
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <curses.h>
#include <sys/time.h>
#include <aio.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

struct aiocb kbcbuf;        // an aio control buf

main()
{
    void on_alarm(int);     // handler for alarm
    void on_input(int);     // handler for keybd
    void set_aio_buffer();

    initscr();
    crmode();
    noecho();
    clear();

    signal(SIGIO, on_input);        // install the handler
    set_aio_buffer();
    aio_read(&kbcbuf);

    signal(SIGALRM, on_alarm);
    set_ticker(delay);              // start ticking

    mvaddstr(row, col, MESSAGE);

    while (1)
    {
        if (done == 1) break;
        pause();
    }
    endwin();
}

/*
 * handler called when aio_read() has stuff to read
 * First check for any error codes, and if ok, then get the return code
 */
void on_input(int signum)
{
    int c;
    char * cp = (char *)kbcbuf.aio_buf;     // cast to char *

    // check the error
    if (aio_error(&kbcbuf) != 0)
        perror("reading failed");
    else
        // get number of chars read
        if (aio_return(&kbcbuf) == 1)
        {
            c = *cp;
            if (c == 'Q' || c == EOF)
            {
                done = 1;
            }
            else if (c == ' ')
            {
                dir = -dir;
            }
        }
    // place a new request
    aio_read(&kbcbuf);
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
 * set members of struct
 * First specify args like those for read(fd, buf, num) and offset
 * Then specify what to do(send signal) and what signal(SIGIO)
 */
void set_aio_buffer()
{
    static char input[1];
    // describe what to read
    kbcbuf.aio_fildes = 0;
    kbcbuf.aio_buf = input;
    kbcbuf.aio_nbytes = 1;
    kbcbuf.aio_offset = 0;

    // describe what to do when read is ready
    kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    kbcbuf.aio_sigevent.sigev_signo = SIGIO;
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
