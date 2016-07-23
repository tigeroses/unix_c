#include "stdio.h"
#include "string.h"

main(int ac, char * av[])
{
    // int *p = (int *)malloc(sizeof(int) * 10);
    // char (*p)[10] = (char *)malloc(sizeof(char*) * 10);
    char *p[10] = {};
    
    for (int i = 0; i < 14; ++i)
    {
        // printf("%d\n", i);
        // *p = i;
        // p ++;
        char s[10] = "abc";
        strcpy(p[i], s);
    }
    
    
    // p[0] = 222;
   
    
    
    for (int i = 0; i < 12; ++i)
        printf("%s\n", p[i]);
    
    
    
}
