/* spwd.c: a simplified version of pwd
 *
 * starts in current directory and recursively
 * climbs up to root of filesystem, prints top part
 * then prints current part
 *
 * uses readdir() to get info about each thing
 *
 * bug: prints an empty string if run from "/"
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int ROOT_FLAG = 0;

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    // printf("\n");
    return 0;
}

void printpathto(ino_t this_inode)
/*
 * prints path leading down to an object with this inode
 * kindof recursive
 */
{
    ino_t my_node;
    char its_name[BUFSIZ];
    if (get_inode("..") != this_inode)
    {
        ROOT_FLAG = 1;
        chdir("..");
        inum_to_name(this_inode, its_name, BUFSIZ);
        my_node = get_inode(".");
        printpathto(my_node);
        printf("/%s", its_name);
    }
    if (ROOT_FLAG == 0)
        printf("/%s", its_name);

}

void inum_to_name(ino_t inode_to_find, char * namebuf, int buflen)
/*
 * looks through current directory for a file with this inode
 * number and copies its name int namebuf
 */
{
    DIR * dir_ptr;
    struct dirent * direntp;
    dir_ptr = opendir(".");
    if(dir_ptr == NULL)
    {
        perror(".");
        exit(1);
    }
    /*
     * search directory for a file with specified inum
     */
    while((direntp = readdir(dir_ptr)) != NULL)
        if (direntp->d_ino == inode_to_find)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }
    fprintf(stderr, "error looking for inum %d\n", inode_to_find);
    exit(1);
}

ino_t get_inode(char * fname)
/*
 * returns inode number of the file
 */
{
    struct stat info;
    if (stat(fname, &info) == -1)
    {
        fprintf(stderr, "Cannot stat");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}
