/*
 * shm_tc.c
 */

#include <stdio.h>
#include <sys/shm.h>
#include <time.h>

#define TIME_MEM_KEY 99
#define SEG_SIZE ((size_t)100)

main()
{
    int seg_id;
    char * mem_ptr, *ctime();
    long now;

    // create a shared memory segment
    seg_id = shmget(TIME_MEM_KEY, SEG_SIZE, 0777);
    if (seg_id == -1) 
        printf("error");

    // attach to it and get a pointer to where it attaches
    mem_ptr = shmat(seg_id, NULL, 0);
    if (mem_ptr == (void *)-1)
        printf("error");

    printf("Time is %s", mem_ptr);

    shmdt(mem_ptr);
}
