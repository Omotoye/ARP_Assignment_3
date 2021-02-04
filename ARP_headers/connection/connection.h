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
* Well-known port for the Network: 5080
* Each node must use this port in order to
* communicate within the network. Please make sure
* you have this port enabled on your Azure VM.
**************************************************/
#define     ARPNET_STD_PORTNO     5080

/**********************************************//**
* Initializing the main socket in a server code.
*
* Inside this function the socket gets generated,
* a bind() is tried and, if succesful, is followed
* by a listen().
*
* You can see the portno using the constant
*       ARPNET_STD_PORT
*
* Return
*	sockfd - the file descriptor of the opened
*		and binded socket, if successful, otherwise
*		the error code associated with the first
*		error occurred in the process;
**************************************************/
int net_server_init( );


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
*		of the client will be inserted; NULL if 
*       not used
*
* Return
*	newsockfd - the file descriptor of the newly
*		opened socket where the communication with
*		the client can take place, if successful,
*		otherwise the error code associated with
*		the accept() failed call;
**************************************************/
int net_accept_client(int sockfd, struct sockaddr_in* cli_addr);


/**********************************************//**
* Trying to connect to a server which IPv4 address
* and open port number are known
*
* Inside this function a socket is opened and abort
* connection with the server indicated is 
* attempted.
*
* You can see the portno using the constant
*       ARPNET_STD_PORT
*
* Arguments
*	IPaddr - a string representing the name 
*		associated with the server;
*
* Return
*	sockfd - the file descriptor of the opened
*		socket where the communication with the
*		desired server can take place;		
**************************************************/
int net_client_connection(char *IPaddr);

#endif