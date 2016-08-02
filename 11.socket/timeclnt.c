/*
 * timeclnt.c - a client for timeserv.c
 * usage: timeclnt hostname protnumber
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define oops(msg)   {perror(msg); exit(1);}

int main(int ac, char *av[])
{
    struct sockaddr_in  servadd;     
    struct hostent  *hp;            
    int sock_id, sock_fd;
    char message[BUFSIZ];
    int messlen;

    // Step1: get a socket
    sock_id = socket(PF_INET, SOCK_STREAM, 0);
    if (sock_id == -1)
        oops("socket");

    // Step2: connect to server
    //          need to build address(host,port) of server first
    bzero(&servadd, sizeof(servadd));       // clear out struct

    hp = gethostbyname(av[1]);
    if (hp == NULL)
        oops(av[1]);

    // fill in host part
    bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
    servadd.sin_port = htons(atoi(av[2]));        // fill in socket port
    servadd.sin_family = AF_INET;             // fill in addr family

    if (connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
        oops("connect");

    // Step3: transfer data from server, then hangup
    messlen = read(sock_id, message, BUFSIZ);
    if (messlen == -1)
        oops("read");
    if (write(1, message, messlen) != messlen)
        oops("write");
    
    close(sock_id);
}
