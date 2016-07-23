/*
 * my sort 
 */
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

void my_sort(int * lst, int length);
void my_sort(string * lst, int length);
void my_sort(char * lst, int length);

void print_lst(int *lst, int length);
void print_lst(char *lst, int length);
void print_lst(string *lst, int length);

int main()
{
    // int lst[] = {1, 3,2, 5,4};
    // string lst[] = {"lnliu", "fxzhao", "mm"};
    char lst[] = {'l', 'f', 'm'};
    int length = sizeof(lst) / sizeof(lst[0]);
    char *sorted_lst = lst;
    print_lst(sorted_lst, length);

    my_sort(sorted_lst, length);

    print_lst(sorted_lst, length);


    return 0;
}

void my_sort(int * lst, int length)
{
    for (int i = 0; i < length; ++i)
    {
        for (int j = i+1; j < length; ++j)
        {
            // printf("%d %d %d %d\n", i, j, lst[i], lst[j]);
            if (lst[i] > lst[j])
            {
                int tmp = lst[j];
                lst[j] = lst[i];
                lst[i] = tmp;
            }
        }
    }
}

void my_sort(string * lst, int length)
{
    for (int i = 0; i < length; ++i)
    {
        for (int j = i+1; j < length; ++j)
        {
            // printf("%d %d %d %d\n", i, j, lst[i], lst[j]);
            if (lst[i] > lst[j])
            {
                string tmp = lst[j];
                lst[j] = lst[i];
                lst[i] = tmp;
            }
        }
    }
}

void my_sort(char * lst, int length)
{
    for (int i = 0; i < length; ++i)
    {
        for (int j = i+1; j < length; ++j)
        {
            // printf("%d %d %d %d\n", i, j, lst[i], lst[j]);
            if (lst[i] > lst[j])
            {
                char tmp = lst[j];
                lst[j] = lst[i];
                lst[i] = tmp;
            }
        }
    }
}
void print_lst(int *lst, int length)
{
    for (int i = 0; i < length; ++i)
        printf("%d ", lst[i]);
    printf("\n");
}

void print_lst(string *lst, int length)
{
    for (int i = 0; i < length; ++i)
        std::cout<<lst[i]<<" ";
    printf("\n");
}

void print_lst(char *lst, int length)
{
    for (int i = 0; i < length; ++i)
        printf("%c ", lst[i]);
    printf("\n");
}
