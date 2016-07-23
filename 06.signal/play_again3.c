/*
 * play_again3.c
 * purpose: ask if user wants another transaction
 * method: set tty into chr-by-chr mode and no-echo mode
 *          set tty into no-delay mode
 *          read char, return result
 * returns: 0 => yes, 1 => no, 2=> timeout
 * better: reset terminal mode on Interrupt
 */
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#define ASK "Do you want another transaction"
#define TRIES 3     // max tries
#define SLEEPTIME 2 // time per try
#define BEEP putchar('\a')  // alert user

int main()
{
    int response;
    tty_mode(0);                        // save tty mode
    set_cr_noecho_mode();               // set -icanon, -echo
    set_nodelay_mode();                 // noinput => EOF
    response = get_response(ASK, TRIES);
    tty_mode(1);                        // restore tty mode
    return response;
}

int get_response(char * question, int maxtries)
{
    int input;
    printf("%s (y/n)?", question);
    fflush(stdout);                 // force output
    while(1)
    {
        sleep(SLEEPTIME);               // wait a bit
        input = tolower(get_ok_char()); // get next chr
        if (input == 'y')
            return 0;
        if (input == 'n')
            return 1;
        if (maxtries-- == 0)
            return 2;
        BEEP;
    }
}

// skip over non-legal chars and return y,Y,n,N or EOF
get_ok_char()
{
    int c;
    while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
        ;
    return c;
}

set_cr_noecho_mode()
/*
 * purpose: put file descriptior 0 into chr-by-chr mode and noecho mode
 * method: use bits in termios
 */
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);        // read curr. setting
    ttystate.c_lflag &= ~ICANON;    // no buffering
    ttystate.c_lflag &= ~ECHO;      // no echo either
    ttystate.c_cc[VMIN] = 1;        // get one char at a time
    tcsetattr(0, TCSANOW, &ttystate);   // install setting
}

set_nodelay_mode()
/* purose: put file descriptor 0 into no-delay mode
 * method: use fcntl to set bits
 * notes: tcsetattr() will do something similar, but it is complicated
 */
{
    int termflags;
    termflags = fcntl(0, F_GETFL);      // read curr. settings
    termflags |= O_NDELAY;              // flip on nodelay bit
    fcntl(0, F_SETFL, termflags);       // and install 'em
}

// how == 0 => save currentmode
// how == 1 => restore mode
// this version handles termios and fcntl flags
tty_mode(int how)
{
    static struct termios original_mode;
    static int original_flags;
    if (how == 0)
    {
        tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETFL);
    }
    else
    {
        tcsetattr(0, TCSANOW, &original_mode);
        fcntl(0, F_SETFL, original_flags);
    }
}
