/*
 * pipedemo.c
 * Demonstrates: how to create and use a pipe
 * Effect: creates a pipe, writes into writing
 *          end, then runs around and reads from reading
 *          end, A little weird, but demonstrates the idea
 */

#include <stdio.h>
#include <unistd.h>

main()
{
    int len, i, apipe[2];
    char buf[BUFSIZ];

    // get a pipe
    if (pipe(apipe) == -1)
    {
        perror("could not make pipe");
        exit(1);
    }
    printf("Got a pipe!It is file descriptors: {%d %d}\n",
                apipe[0], apipe[1]);

    // read from stdin, write into pipe, read from pipe, print
    while (fgets(buf, BUFSIZ, stdin))
    {
        len = strlen(buf);
        if (write(apipe[1], buf, len) != len)
        {
            perror("writing to pipe");
            break;
        }
        for (i = 0; i < len; i++)
            buf[i] = 'X';
        len = read(apipe[0], buf, BUFSIZ);
        if (len == -1)
        {
            perror("reading from pipe");
            break;
        }
        if (write(1, buf, len) != len)
        {
            perror("writing to stdout");
            break;
        }
    }
}
