/* filename: more01.c
 * read and print 24 lines then pause for a few special commands
 */

#include <stdio.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

int main(int argc, char * argv[])
{
    FILE * fp;
    if (argc == 1)
        do_more(stdin);
    else
        while(--argc)
            if ((fp = fopen(* ++argv, "r")) != NULL)
            {
                do_more(fp);
                fclose(fp);
            }
            else
                exit(1);
    return 0;
}

void do_more(FILE * fp)
{
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(), reply;

    while(fgets(line, LINELEN, fp))         // more input
    {
        if (num_of_lines == PAGELEN)        // full screen?
        {
            reply = see_more();             // y: ask user
            if (reply == 0)                 // n: done
                break;
            num_of_lines -= reply;          // reset count
        }
        if (fputs(line, stdout) == EOF)     // show line
            exit(1);                        // or die
        num_of_lines++;                     // count it
    }
}

int see_more()
{
    int c;
    printf("\033[7m more?\033[m");          // reverse on a vt100
    while ((c = getchar()) != EOF)          // get response
    {
        if (c == 'q')                       // a -> N
            return 0;
        if (c == ' ')                       // ' ' -> next page
            return PAGELEN;                 // how many to show
        if (c == '\n')                      // Enter key -> 1 line
            return 1; 
    }
    return 0;
}

