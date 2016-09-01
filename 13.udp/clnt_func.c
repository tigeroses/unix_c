/*
 * clnt_func.c: functions for the client of the license server
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Important variables used throughout
static int pid = -1;        // pid
static int sd = -1;         // communications socket
static struct sockaddr serv_addr;   // server address
static socklen_t serv_alen;         // length of address
static char ticket_buf[128];        // buffer to hold our ticket
static have_ticket = 0;             // set when we have a ticket

#define MSGLEN 128          // size of datagrams
#define SERVER_PORTNUM 2020
#define HOSTLEN 512
#define oops(p) {perror(p); exit(1);}

char * do_transcation();

// setup: get pid, socket, address of server
setup()
{
    char hostname[BUFSIZ];

    pid = getpid();
    // printf("pid %d\n", pid);
    sd = make_dgram_client_socket();
    // printf("sock_id %d\n", sd);
    if (sd == -1)
        oops("Cannot create socket");

    gethostname(hostname, HOSTLEN);
    make_internet_address(hostname, SERVER_PORTNUM, &serv_addr);
    serv_alen = sizeof(serv_addr);
    // printf("serv_alen %d\n", serv_alen);
}

shut_down()
{
    close(sd);
}

// get_ticket: get a ticket from server
int get_ticket()
{
    char * response;
    char buf[MSGLEN];

    if (have_ticket)
        return(0);

    sprintf(buf, "HELO %d", pid);

    if ((response = do_transcation(buf)) == NULL)
        return(-1);

    // parse the response and see if we got a ticket
    if (strncmp(response, "TICK", 4) == 0)
    {
        strcpy(ticket_buf, response+5);
        have_ticket = 1;
        printf("ticket_buf %s\n", ticket_buf);
        narrate("got ticket", ticket_buf);
        return(0);
    }

    if (strncmp(response, "FAIL", 4) == 0)
        narrate("Could not get ticket", response);
    else
        narrate("Unknown message:", response);

    return(-1);
}

// release_ticket: give a ticket back to the server
int release_ticket()
{
    char * response;
    char buf[MSGLEN];

    if (!have_ticket)
        return(0);

    sprintf(buf, "GBYE %s", ticket_buf);

    if ((response = do_transcation(buf)) == NULL)
        return(-1);

    // parse the response and see if we give a ticket success
    if (strncmp(response, "THNX", 4) == 0)
    {
        have_ticket = 0;
        narrate("release a ticket", "");
        return(0);
    }

    if (strncmp(response, "FAIL", 4) == 0)
        narrate("release failed", response+5);
    else
        narrate("Unknown message:", response);

    return(-1);
}

// do_transcation: send a request to the server and get a response back
char * do_transcation(char * msg)
{
    static char buf[MSGLEN];
    struct sockaddr retaddr;
    socklen_t addrlen = sizeof(retaddr);
    int ret;

    // printf("before sendto msg %s\n", msg);
    ret = sendto(sd, msg, strlen(msg), 0, &serv_addr, serv_alen);
    // printf("after sendto ret %d\n", ret);
    if (ret == -1)
    {
        syserr("sendto");
        return(NULL);
    }

    // printf("before recvfrom\n");
    ret = recvfrom(sd, buf, MSGLEN, 0, &retaddr, &addrlen);
    // printf("after recvfrom ret %d\n", ret);
    if (ret == -1)
    {
        syserr("recvfrom");
        return(NULL);
    }

    return(buf);
}

narrate(char *msg1, char *msg2)
{
    sprintf(stderr, "CLIENT[%d]: %s %s\n", pid, msg1, msg2);
}

syserr(char *msg1)
{
    char buf[MSGLEN];
    sprintf(buf, "CLIENT[%d]: %s", pid, msg1);
    perror(buf);
}
