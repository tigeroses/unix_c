/*
 * clnt.c
 */

#include <stdio.h>

int main(int ac, char *av[])
{
    printf("begin\n");
    setup();
    if (get_ticket() != 0)
        exit(0);

    do_regular_work();

    release_ticket();
    shut_down();
}


do_regular_work()
{
    printf("Sleep\n");
    sleep(10);
}
