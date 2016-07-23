/* who1.c 
 * a first version of the who program
 * open, read UTMP file, and show results
 */

#include <stdio.h>
#include <utmp.h>   // define the structure which readed
#include <fcntl.h>  // function of open
#include <unistd.h> // read, close

#define SHOWHOST    // include remote machine on output

void show_info(struct utmp * utbufp);

int main()
{
    struct utmp current_record; // read info into here
    int         utmpfd;         // read from this descriptor
    int         reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE);      // UTMP_FILE is in utmp.h
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
    {
        // printf("%d %d\n", reclen, utmpfd);
        show_info(&current_record);
    }
    close(utmpfd);
    return 0;                   // went ok
}

/* 
 * show_info()
 * display contents of the utmp struct in human readable form
 * note: these sizes should not be hardwired
 */
void show_info(struct utmp * utbufp)
{
    printf("% -8.8s", utbufp->ut_user); // logname
    printf(" ");
    printf("% -8.8s", utbufp->ut_line); // tty
    printf(" ");
    printf("% -8.8ld", utbufp->ut_tv); // login time
    printf(" ");
#ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);    // host
#endif
    printf("\n");
}
