/*
 * test_mutex.c
 */

#include <stdio.h>
#include <pthread.h>

int total = 0;
int times = 100;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct arg_set
{
    int count;
};

int main()
{
    pthread_t t1, t2;

    /*
    void * add();

    pthread_create(&t1, NULL, add, NULL);
    pthread_create(&t2, NULL, add, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", total);
    */

    void * add2(void *);


    struct arg_set a1, a2;
    a1.count = 0;
    a2.count = 0;
    pthread_create(&t1, NULL, add2, (void *)&a1);
    pthread_create(&t2, NULL, add2, (void *)&a2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", a1.count + a2.count);

    return 0;
}

// First method: add mutex
void * add()
{
    for (int i = 0; i < times; i++)
    {
        pthread_mutex_lock(&lock);
        total++;
        pthread_mutex_unlock(&lock);
    }
}

// Second method: cal single
void * add2(void * a)
{
    struct arg_set *arg = a;
    for (int i = 0; i < times; i++)
    {
        arg->count++;
    }
}
