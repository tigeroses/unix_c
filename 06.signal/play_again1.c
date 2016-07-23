/*
 * play_again1.c
 * purpose: ask if user wants another transaction
 * method: ask a question, wait for yes/no answer
 * returns: 0 => yes, 1 => no
 * better: do no echo inappropriate input
 */
#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int main()
{
    int response;
    tty_mode(0);                        // save tty mode
    set_crmode();                       // set chr-by-chr mode
    response = get_response(QUESTION);
    tty_mode(1);                        // restore tty mode
    return response;
}

int get_response(char * question)
{
    int input;
    printf("%s (y/n)?", question);
    while(1)
    {
        switch(input = getchar())
        {
            case 'y':
            case 'Y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1;
            default:
                printf("\ncannot understand %c", input);
                printf("Please type y or n\n");
        }
    }
}

set_crmode()
/*
 * purpose: put file descriptior 0 into chr-by-chr mode
 * method: use bits in termios
 */
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);        // read curr. setting
    ttystate.c_lflag &= ~ICANON;    // no buffering
    ttystate.c_cc[VMIN] = 1;        // get one char at a time
    tcsetattr(0, TCSANOW, &ttystate);   // install setting
}

// how == 0 => save currentmode
// how == 1 => restore mode
tty_mode(int how)
{
    static struct termios original_mode;
    if (how == 0)
        tcgetattr(0, &original_mode);
    else
        return tcsetattr(0, TCSANOW, &original_mode);
}
