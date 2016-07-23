/* bounce2d 1.0
 * bounce a character around the screen
 * defined by some parameters
 *
 * user input: s slow down x component
 *             S slow down y component
 *             f speed up x component
 *             F speed up y component
 *             Q quit
 *
 * blocks on read, but timer tick sends SIGALRM caught by ball_move
 * build: cc bounce2d.c -l curses -o bounce2d
 */
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include "bounce.h"

struct ppball the_ball;
struct board the_board;

// the main loop
void set_up();
void wrap_up();

int main()
{
    FILE *fp;
    fp = fopen("result", "w");
    
    int c;
    set_up();
    while (1)
    {
        c = getch();
        if (c == 'Q') break;

        fputc(c, fp);
        if (c == 'f') the_ball.x_ttm--;
        else if (c == 's') the_ball.x_ttm++;
        else if (c == 'F') the_ball.y_ttm--;
        else if (c == 'S') the_ball.y_ttm++;
        else if (c == 'a') 
        {
            if (the_board.left > LEFT_EDGE)
                the_board.how = -1;
        }
        else if (c == 'd')
        {
            if (the_board.left+the_board.length < RIGHT_EDGE)
                the_board.how = 1;
        }
        else
            the_board.how = 0;

    }
    wrap_up();

    fclose(fp);
    return 0;
}

// init structure and other stuff
void set_up()
{
    the_board.length = 3;
    the_board.left = (RIGHT_EDGE+LEFT_EDGE)/2;
    the_board.row = ERROR+1;
    the_board.how = 0;

    the_ball.y_pos = Y_INIT;
    the_ball.x_pos = X_INIT;
    the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
    the_ball.x_ttg = the_ball.x_ttm = X_TTM;
    the_ball.y_dir = 1;
    the_ball.x_dir = 1;
    the_ball.symbol = DFL_SYMBOL;

    
    initscr();
    noecho();
    crmode();

    int i;
    for (i = LEFT_EDGE-1; i <= RIGHT_EDGE+1; ++i)
    {
        mvaddch(TOP_ROW-1, i, X_BUND);
        //mvaddch(ERROR+1, i, XX_BUND);
        //mvaddch(i, TOP_ROW, X_BUND);
        //mvaddch(i, BOT_ROW, X_BUND);
    }
    for (i = TOP_ROW; i <= ERROR+1; ++i)
    {
        mvaddch(i, LEFT_EDGE-1, Y_BUND);
        mvaddch(i, RIGHT_EDGE+1, Y_BUND);
        //mvaddch(RIGHT_EDGE, i, Y_BUND);
        //mvaddch(LEFT_EDGE, i, Y_BUND);
    }


    for (i = the_board.left; i < the_board.left + the_board.length; ++i)
        mvaddch(the_board.row, i, '_');

    void ball_move(int);
    signal(SIGINT, SIG_IGN);
    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
    refresh();
    

    signal(SIGALRM, ball_move);
    set_ticker(1000/TICKS_PER_SEC);

}

void wrap_up()
{
    set_ticker(0);
    endwin();
}

void ball_move(int signum)
{
    int y_cur, x_cur, moved;

    signal(SIGALRM, SIG_IGN);       // dont get caught now
    y_cur = the_ball.y_pos;         // old spot
    x_cur = the_ball.x_pos;
    moved = 0;

    if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1)
    {
        the_ball.y_pos += the_ball.y_dir;       // move
        the_ball.y_ttg = the_ball.y_ttm;        // reset
        moved = 1;
    }

    if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1)
    {
        the_ball.x_pos += the_ball.x_dir;   // move
        the_ball.x_ttg = the_ball.x_ttm;        // reset
        moved = 1;
    }

    if (the_board.how != 0)
        change_board(the_board.how);
    if (moved)
    {
        mvaddch(y_cur, x_cur, BLANK);
        mvaddch(y_cur, x_cur, BLANK);
        mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
        bounce_or_lose(&the_ball);
        move(LINES-1, COLS-1);
        refresh();
    }

    signal(SIGALRM, ball_move);     // for unreliable systems
}


int bounce_or_lose(struct ppball * bp)
{
    int return_val = 0;

    if (bp->y_pos == TOP_ROW)
    {
        bp->y_dir = 1;
        return_val = 1;
    }
    /*
    else if (bp->y_pos == BOT_ROW)
    {
        bp->y_dir = -1;
        return_val = 1;
    }
    */
    else if (bp->y_pos == ERROR + 1)
    {
        if (bp->x_pos < the_board.left || bp->x_pos > the_board.left + the_board.length)
            game_over();
        else
        {
            bp->y_dir = -1;
            return_val = 1;
        }
    }

    if (bp->x_pos == LEFT_EDGE)
    {
        bp->x_dir = 1;
        return_val = 1;
    }
    else if (bp->x_pos == RIGHT_EDGE)
    {
        bp->x_dir = -1;
        return_val = 1;
    }
    return return_val;
}

void game_over()
{
    char * s = "GAME OVER";
    int i, j;
    for (i = (LEFT_EDGE+RIGHT_EDGE)/2-4, j = 0; j <= sizeof(s)/sizeof(char); ++i, ++j)
        mvaddch((TOP_ROW+BOT_ROW)/2, i, s[j]);
    refresh();
    //wrap_up();
    signal(SIGALRM, SIG_DFL);
    sleep(2);
    endwin();
    exit(-1);
}

void change_board(int num)
{
    int i;
    for (i = the_board.left; i < the_board.left + the_board.length; ++i)
        mvaddch(the_board.row, i, ' ');
    the_board.left += num;
    for (i = the_board.left; i < the_board.left + the_board.length; ++i)
        mvaddch(the_board.row, i, '_');
    the_board.how = 0;
    //refresh();
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
