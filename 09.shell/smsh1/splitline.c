/*
 * splitline.c - command reading and parsing functions for smsh
 *
 * char * next_cmd(char *prompt, FILE *fp) - get next command
 * char ** splitline(char *str) - parse a string
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

char * next_cmd(char * prompt, FILE * fp)
/*
 * purpose: read next command line from fp
 * returns: dynamically allocated string holding command line
 * errors: NULL at EOF (not really an error)
 *          calls fatal from emalloc()
 * notes: allocates space in BUFSIZ chunks
 */
{
    char *buf;      // the buffer
    int bufspace = 0;  // total size
    int pos = 0;        // current position
    int c;              // input char

    printf("%s", prompt);
    while ((c = getc(fp)) != EOF)
    {
        if (pos + 1 >= bufspace)
        {
            if (bufspace == 0)
                buf = emalloc(BUFSIZ);
            else
                buf = erealloc(buf, bufspace+BUFSIZ);
            bufspace += BUFSIZ;
        }

        if (c == '\n')
            break;

        buf[pos++] = c;
    }
    if (c == EOF && pos == 0)
        return NULL;
    buf[pos] = '\0';
    return buf;
}

/*
 * splitline (parse a line into an array of strings)
 */
char ** splitline(char * line)
/*
 * purpose: split a line into array of white - space separated tokens
 * returns: a NULL-terminated array of pointers to copies of the
 *          tokens or NULL if line if no tokens on the line
 * action: travers the array, locate strings, make copies
 * note: strtok() could work, but we may want to add quotes later
 */
{
    char * newstr();
    char **args;
    int spots = 0;      // spots in table
    int bufspace = 0;   // bytes in table
    int argnum = 0;     // slots used
    char *cp = line;    // pos in string
    char *start;
    int len;

    if (line == NULL)
        return NULL;

    args = emalloc(BUFSIZ);
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char *);

    while (*cp != '\0')
    {
        while (isspace(*cp))       // skip leading spaces
            cp++;
        if (*cp == "\0")
            break;

        // make sure the array has room(+1 for NULL)
        if (argnum + 1 >= spots)
        {
            args = erealloc(args, bufspace+BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/sizeof(char *));
        }
        // mark start, then find end of word
        start = cp;
        len = 1;
        while (*++cp != '\0' && !(isspace(*cp)))
        {
            len++;
        }
        args[argnum++] = newstr(start, len);
    }

    args[argnum] = NULL;
    return args;
}

/*
 * purpose: constructor for strings
 * returns: a string, never NULL
 */
char * newstr(char *s, int l)
{
    char * rv = emalloc(l + 1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char ** list)
/*
 * purpose: free the list returned by splitline
 * returns: nothing
 * action: free all strings in list and then free the list
 */
{
    char ** cp = list;
    while (*cp)
        free(*cp++);
    free(list);
}

void * emalloc(size_t n)
{
    void * rv;
    if ((rv = malloc(n)) == NULL)
        fatal("out of memory", "", 1);
    return rv;
}

void * erealloc(void *p, size_t n)
{
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", 1);
    return rv;
}
