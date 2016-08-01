/*
 * pipe.c
 * Demonstrates how to create a pipeline from on process to another
 *  Takes two args, each a command, and connects
 *  av[1]'s ouput to input of av[2]
 *  usage: pipe cmd1 cmd2
 *  effect: cmd1 | cmd2
 *  Limitations: commands do not take arguements
 *  uses execlp() since known number of args
 *  Note: exchange child and parent and watch fun
 */

#include <stdio.h>
#include <unistd.h>

#define oops(m,x)   {perror(m); exit(x);}

main(int ac, char ** av)
{
    int thepipe[2], newfd, pid;

    if (ac != 3)
    {
        fprintf(stderr, "usage: pipe cmd1 cmd2\n");
        exit(1);
    }
    if (pipe(thepipe) == -1)
        oops("Cannot get a pipe", 1);

    /*--------------------------------------------------*/
    /* now we have a pipe, now let's get two processes  */

    if ((pid = fork()) == -1)
        oops("Cannot fork", 2);

    /*--------------------------------------------------*/
    /* Right Here, there are two processes */
    /*              parent will read from pipe */

    if (pid > 0)                                // parent will exec av[2]
    {
        close(thepipe[1]);                      // parent doesn't write to pipe

        if (dup2(thepipe[0], 0) == -1)
            oops("could not redirect stdin", 3);

        close(thepipe[0]);
        execlp(av[2], av[2], NULL);
        oops(av[2], 4);
    }

    /* child execs av[1] and writes into pipe */

    close(thepipe[0]);

    if (dup2(thepipe[1], 1) == -1)
        oops("could not redirect stdout", 4);

    close(thepipe[1]);
    execlp(av[1], av[1], NULL);
    oops(av[1], 5);

}
