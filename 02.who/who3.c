/* who3.c 
 * a second version of the who program
 * open, read UTMP file, and show results
 * and filter some white record
 * and add buffter to fast the execute
 */

#include <stdio.h>
#include <utmp.h>   // define the structure which readed
#include <fcntl.h>  // function of open
#include <unistd.h> // read, close
#include <time.h>

// #define SHOWHOST    // include remote machine on output

void show_time(long);
void show_info(struct utmp * utbufp);

int main()
{
    struct utmp * utbufp, // holds pointer to next rec
                * utmp_nex(); // returns pointer to next

    if (utmp_open(UTMP_FILE) == -1)
    {
        perror(UTMP_FILE);      // UTMP_FILE is in utmp.h
        exit(1);
    }

    while ((utbufp = utmp_next()) != ((struct utmp *)NULL))
    {
        // printf("%d %d\n", reclen, utmpfd);
        show_info(utbufp);
    }
    utmp_close();
    return 0;                   // went ok
}

/* 
 * show_info()
 * display contents of the utmp struct in human readable form
 * note: these sizes should not be hardwired
 */
void show_info(struct utmp * utbufp)
{
    if (utbufp->ut_type != USER_PROCESS)
        return;

    printf("% -8.8s", utbufp->ut_user); // logname
    printf(" ");
    printf("% -8.8s", utbufp->ut_line); // tty
    printf(" ");
    show_time(utbufp->ut_tv.tv_sec);
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
        printf("(%s)", utbufp->ut_host);    // host
#endif
    printf("\n");
}

/* 
 * display time in a format fit for human consumption
 * uses ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wid and LIMTS
 * it to 12 chars.
 */
void show_time(long timeval)
{
    // char * cp;              // to hold address of time
    // cp = ctime(&timeval);   // convert time to string
                            // string looks like
                            // Mon Feb 4 00:46:40 EST 1991
                            // 01234567938272
    // printf("%12.12s", cp+4);// pick 12 chars from pos 4
    struct tm * cp = localtime(&timeval);
    printf("%d-%d-%d %d:%d", cp->tm_year+1900, cp->tm_mon+1, cp->tm_mday,
                            cp->tm_hour, cp->tm_min);// pick 12 chars from pos 4
                
}

