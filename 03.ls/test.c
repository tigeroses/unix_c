#include <stdio.h>
#include <stdlib.h>

int comp_int(const void *a, const void *b);
int comp_char(const void *a, const void *b);
int comp_string(const void *a, const void *b);

int main()
{
    // int a[4] = {3, 4, 1, 2};
    // char a[4] = {'b', 'a', 'd', 'c'};
    char a[][20] = {};
    // char a[4][20] = {"bbb", "aaa", "bba", "ccc"};

    for (int i = 0; i < 4; ++i)
    {
        printf("%s ", a[i]);
    }
    printf("\n");

    qsort(a, 4, sizeof(a[0]), comp_string);

    for (int i = 0; i < 4; ++i)
        printf("%s ", a[i]);
    printf("\n");

    return 0;
}

int comp_int(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

int comp_char(const void *a, const void *b)
{
    return *(char *)a - *(char *)b;
}

int comp_string(const void *a, const void *b)
{
    int return_val;
    char *t1 = a;
    char *t2 = b;
    printf("%s %s\n", t1, t2);

    
    for (int i = 0; i < strlen(t1), i < strlen(t2); ++i)
    {
        if (t1[i] == t2[i])
            continue;
        else
            return t1[i] - t2[i];
    }
    
    return 0;
}
