/*
 * test_mutex.c
 *
 * pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex)
 *      使线程挂起直到另一个线程通过条件变量发出消息.先自动释放指定的锁，
 *      然后等待条件变量的变化
 * pthread_cond_signal(pthread_cond_t * cond)
 *      通过条件变量cond 发消息
 */

#include <stdio.h>
#include <pthread.h>

int times = 100000000;
int total = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;

struct arg_set
{
    int count;
    int times;
};

struct arg_set * mailbox;

int main()
{
    pthread_t t1, t2;

    void * add2(void *);

    int reports_in = 0;

    struct arg_set a1, a2;

    pthread_mutex_lock(&lock);

    a1.count = 0;
    a1.times = 10000000;
    a2.count = 0;
    a2.times = 1000;
    pthread_create(&t1, NULL, add2, (void *)&a1);
    pthread_create(&t2, NULL, add2, (void *)&a2);

    while (reports_in < 2)
    {
        printf("MAIN: waiting for flag\n");
        pthread_cond_wait(&flag, &lock);
        printf("MAIN: I have the lock\n");
        printf("%d\n", mailbox->count);
        total += mailbox->count;
        if (mailbox == &a1)
            pthread_join(t1, NULL);
        if (mailbox == &a2)
            pthread_join(t2, NULL);
        mailbox = NULL;
        pthread_cond_signal(&flag);
        reports_in ++;
    }

    printf("total %d\n", total);

    return 0;
}

void * add2(void * a)
{
    struct arg_set *arg = a;
    for (int i = 0; i < arg->times; i++)
    {
        arg->count++;
    }

    printf("COUNT: waiting to get lock\n");
    pthread_mutex_lock(&lock);
    printf("COUNT: have lock\n");
    if (mailbox != NULL)
        pthread_cond_wait(&flag, &lock);
    mailbox = arg;
    pthread_cond_signal(&flag);
    pthread_mutex_unlock(&lock);

    return NULL;
}
