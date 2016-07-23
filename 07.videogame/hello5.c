/*
 * hello5.c
 * purpose: bounce a message back and forth across the screen
 */
#include <curses.h>

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

main()
{
    char * message = "Hello";
    char * blank   = "     ";
    int dir = +1;
    int pos = LEFTEDGE;
    
    initscr();      // turn on curses, send requests
    clear();        // clear screen

    
    while(1)
    {
        move(ROW, pos);
        addstr( message );
        move(LINES - 1, COLS - 1);      // park the cursor
        refresh();
        sleep(1);
        move(ROW, pos);
        addstr( blank );                  // erase string
        pos += dir;                     // advance position
        if (pos >= RIGHTEDGE)
            dir = -1;
        if (pos <= LEFTEDGE)
            dir = +1;

    }
    
    endwin();

}
