#include<stdio.h>

void test(void * s);

int main()
{
    /*
    char a[10][10] = {'abc', "bac"};
    printf("%s", a[0]);
    printf("%s", a[1]);
    */

    char a[10][10] = {"abc", "bac"};
    test(&a[0]);
    test(a[1]);

    return 0;
}

// void test(char * s)
void test(void * s)
{

    char *t = "abc";
    char *t2 = s;
    printf("%s\n", s);
    printf("%s\n", t);
    printf("%c\n", t[0]);
    printf("%c\n", t2[0]);
}

