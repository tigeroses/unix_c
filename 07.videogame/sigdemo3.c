/* sigdemo3.c
 * purpose: show answers to signal questions
 * q1: dose the handler stay in effect after a signal arrivers?
 * q2: what if a signalX arrivers while handling signalX?
 * q3: what if a signalX arrivers while handling signalY?
 * q4: what happens to read() when a signal arrives?
 */
#include <stdio.h>
#include <signal.h>

#define INPUTLEN 100

main(int ac, char * av[])
{
    void inthandler(int);
    void quithandle(int);
    char input[INPUTLEN];
    int nchars;

    signal(SIGINT, inthandler); // set handler
    signal(SIGQUIT, quithandle); // set handler

    do
    {
        printf("\nType a message\n");
        nchars = read(0, input, (INPUTLEN-1));
        if (nchars == -1)
            perror("read returned an error");
        else
        {
            input[nchars] = '\0';
            printf("You typed: %s", input);
        }
    }
    while (strncmp(input, "quit", 4) != 0);
}

void inthandler(int s)
{
    printf("Received signal %d..waiting\n", s);
    sleep(2);
    printf("Leaving inthandler \n");
}

void quithandle(int s)
{
    printf("Received signal %d..waiting\n", s);
    sleep(3);
    printf("Leaving quithandle \n");
}
