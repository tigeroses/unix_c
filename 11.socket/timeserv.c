/*
 * timeserv.c - a socket based time of day server
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define PORTNUM 7000
#define HOSTLEN 256
#define oops(msg)   {perror(msg); exit(1);}

int main(int ac, char *av[])
{
    struct sockaddr_in  saddr;      // build our address here
    struct hostent  *hp;            // this is part of our address
    char hostname[HOSTLEN];
    int sock_id, sock_fd;
    FILE *sock_fp;
    char *ctime();                  // convert secs to string
    time_t  thetime;

    // Step1: ask kernel for a socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1)
        oops("socket");

    // Step2: bind address to socket. Address is host, port
    bzero((void *)&saddr, sizeof(saddr));       // clear out struct

    gethostname(hostname, HOSTLEN);
    // printf("%s", hostname);
    hp = gethostbyname(hostname);

    // fill in host part
    bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
    saddr.sin_port = htons(PORTNUM);        // fill in socket port
    saddr.sin_family = AF_INET;             // fill in addr family

    if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
        oops("bind");

    // Step3: allow incoming calls with Qsize=1 on socket
    if (listen(sock_id, 1) != 0)
        oops("listen");

    // main loop: accept(), write(), close()
    while (1)
    {
        sock_fd = accept(sock_id, NULL, NULL);      // wait for a call
        printf("Wow! got a call!\n");
        if (sock_fd == -1)
            oops("accept");

        sock_fp = fdopen(sock_fd, "w");             // wirte to the socket as a stream
        if (sock_fp == NULL)
            oops("fdopen");

        thetime = time(NULL);

        fprintf(sock_fp, "The time here is ..");
        fprintf(sock_fp, "%s", ctime(&thetime));

        fclose(sock_fp);
    }
}
