/*
 * file_ts.c
 *
 * 文件锁　分两类：
 * 一个是写数据锁，告诉其他进程，在完成之前必须等待
 * 一个是读数据锁，告诉其他进程，要写文件必须等待，读文件不受影响
 */

#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <time.h>

main()
{
    char file[20] = "file.txt";
    int fd;
    time_t now;
    char *message;

    if ((fd = open(file, O_CREAT|O_TRUNC|O_WRONLY, 0644)) == -1)
        printf("error");

    while (1)
    {
        time(&now);
        message = ctime(&now);

        lock_operation(fd, F_WRLCK);

        if (lseek(fd, 0L, SEEK_SET) == -1)
            printf("error");
        if (write(fd, message, strlen(message)) == -1)
            printf("error");

        lock_operation(fd, F_UNLCK);
        sleep(1);
    }
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
