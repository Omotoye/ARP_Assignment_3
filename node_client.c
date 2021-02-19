#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

#include "arpnet.h"

char command;

// function for error management
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // Socket Creation
    int sockfd, portno, n, h_height, l_height;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char feedback[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],
          (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* NODE SERVER CREATION */
    pid_t hoist_pid = fork();
    if (hoist_pid == -1)
        error("ERROR in fork");
    if (hoist_pid == 0)
    {

        if ((execlp("./node_server", "./node_server", (char *)NULL)) == -1)
        {
            perror("Exec Receiver");
        }
        exit(EXIT_FAILURE);
    }
    return 0;
}