/*
 * hello2.c
 * purpose: show how to use curses functions with a loop
 * outline: initilize, draw stuff, wrap up
 */
#include <stdio.h>
#include <curses.h>

main()
{
    int i;
    
    initscr();      // turn on curses, send requests
    clear();        // clear screen
    for (i = 0; i < LINES; ++i)
    {
        move(i, i+i);
        if (i % 2 == 1)
            standout();
        addstr("Hello, World");
        if (i % 2 == 1)
            standend();
    }

    refresh();      // update the screen
    getch();        // wait for user input
    endwin();       // turn off curses

}
