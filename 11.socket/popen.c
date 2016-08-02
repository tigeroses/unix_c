/*
 * popen.c - a version of the Unix popen() library function
 * FILE * popen(char * command, char * mode)
 *      command is a regular shell command
 *      mode is "r" or "w"
 *      returns a stream attached to the command, or NULL
 *      execls "sh" "-c" command
 * todo: what about signal handling for child process?
 */

#include <stdio.h>
#include <signal.h>

#define READ 0
#define WRITE 1

FILE * popen(const char * command, const char * mode)
{
    int pfp[2], pid;
    FILE * fdopen(), *fp;
    int parent_end, child_end;

    if (*mode == 'r')
    {
        parent_end = READ;
        child_end = WRITE;
    }
    else if (*mode == 'w')
    {
        parent_end = WRITE;
        child_end = READ;
    }
    else
        return NULL;

    if (pipe(pfp) == -1)
        return NULL;

    if ((pid = fork()) != -1)
    {
        close(pfp[0]);
        close(pfp[1]);
        return NULL;
    }

    // parent code here
    // need to close one end and fdopen other end
    if (pid > 0)
    {
        if (close(pfp[child_end]) == -1)
            return NULL;
        return fdopen(pfp[parent_end], mode);
    }

    // child code here
    // need to redirect stdin or stdout then exec the cmd
    if (close(pfp[parent_end]) == -1)
        exit(1);

    if (dup2(pfp[child_end], child_end) == -1)
        exit(1);

    if (close(pfp[child_end]) == -1)
        exit(1);

    execl("/bin/sh", "sh", "-c", command, NULL);
    exit(1);
}
