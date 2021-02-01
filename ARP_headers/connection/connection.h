#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**********************************************//**
* Initializing the main socket in a server code.
*
* Inside this function the socket gets generated,
* a bind() is tried and, if succesful, is followed
* by a listen().
*
* Arguments
*	portno - Port number the calling process will
*		attach the socket to;
*
* Return
*	sockfd - the file descriptor of the opened
*		and binded socket, if successful, otherwise
*		the error code associated with the first
*		error occurred in the process;
**************************************************/
int server_init(int portno);


/**********************************************//**
* Performing an accept on a connect request.
*
* Inside this function an accept() is performed,
* and errors in such process checked.
*
* Arguments
*	sockfd - the file descriptor of the socket the
*		process is waiting of for a connection 
*		request;
*	p_cli_addr - a pointer to a struct where data
*		of the client will be inserted;
*
* Return
*	newsockfd - the file descriptor of the newly
*		opened socket where the communication with
*		the client can take place, if successful,
*		otherwise the error code associated with
*		the accept() failed call;
**************************************************/
int accept_client(int sockfd, struct sockaddr_in* cli_addr);


/**********************************************//**
* Trying to connect to a server which IPv4 address
* and open port number are known
*
* Inside this function a socket is opened and abort
* connection with the server indicated is 
* attempted.
*
* Arguments
*	IPaddr - a string representing the name 
*		associated with the server;
*	portno - the port number the server is 
*		supposed to be listening to;
*
* Return
*	sockfd - the file descriptor of the opened
*		socket where the communication with the
*		desired server can take place;		
**************************************************/
int client_connection(char *IPaddr, int portno);

#endif