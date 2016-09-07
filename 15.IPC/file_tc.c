/*
 * file_tc.c
 */

#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>

#define BUFLEN 100

main()
{
    char file[20] = "file.txt";
    int fd, nread;
    char buf[BUFLEN];

    if ((fd = open(file, O_RDONLY)) == -1)
        printf("error");


    lock_operation(fd, F_RDLCK);

    while ((nread = read(fd, buf, BUFLEN) > 0))
    {
        printf("buf %s", buf);
    }


    lock_operation(fd, F_UNLCK);

    close(fd);
}

lock_operation(int fd, int op)
{
    struct flock lock;

    lock.l_whence = SEEK_SET;
    lock.l_start = lock.l_len = 0;
    lock.l_pid = getpid();
    lock.l_type = op;

    if (fcntl(fd, F_SETLKW, &lock) == -1)
        printf("error");
}
