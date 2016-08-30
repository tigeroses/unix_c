/*
 * socklib.c
 *
 * This file contains functions used lots when writing internet
 * client/server programs. The two main functions here are:
 *
 * int make_server_socket(portnum) returns a server socket or -1 
 *      if error
 * int make_server_socket_q(portnum, backlog)
 *
 * int connect_to_server(char * hostname, int portnum)
 *      returns a connected socket or -1 if error
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define HOSTLEN 256
#define BACKLOG 1

int make_server_socket_q(int, int);

int make_server_socket(int portnum)
{
    return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog)
{
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN];
    int sock_id;

    sock_id = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_id == -1)
        return -1;

    bzero((void *)&saddr, sizeof(saddr));
    gethostname(hostname, HOSTLEN);
    printf("hostname %s\n", hostname);
    hp = gethostbyname(hostname);
    char str[32];
    char **pptr;
    pptr = hp->h_addr_list;
    // printf("id %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));
    for (int i = 0; i < hp->h_length; ++i)
    {
        printf("%d",(int)hp->h_addr[i]);
        if (i != hp->h_length-1)
            printf(".");
    }
    printf("\n");



    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(portnum);
    saddr.sin_family = AF_INET;
    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        return -1;

    if (listen(sock_id, backlog) != 0)
        return -1;

    return sock_id;
}

int connect_to_server(char *host, int portnum)
{
    int sock;
    struct sockaddr_in servadd;
    struct hostent *hp;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return -1;

    bzero(&servadd, sizeof(servadd));
    hp = gethostbyname(host);
    if (hp == NULL)
        return -1;
    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(portnum);
    servadd.sin_family = AF_INET;

    if (connect(sock, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        return -1;

    return sock;
}
