#include <string.h>
#include "connection.h"

/**
 * initialize server connection without accept a client
 */
int server_init(int portno){
	
	//structures server address
	struct sockaddr_in serv_addr;
	int sockfd;
	int ret;
	
	// Creating socket file descriptor 
    if((ret = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        perror("ERROR in creating socket"); 
		return ret;
    } 
    sockfd = ret;
    //initialize socket address structure
	memset((char *)&serv_addr, 0, sizeof(serv_addr)); // bzero() deprecated
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    //attaching socket to the port passed as parameters 
    if ((ret = bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr))) < 0) {
		perror("ERROR on binding");
		close(sockfd);
		return ret;
	}
	
    //listen for connections with max 5 connection
    if((ret = listen(sockfd, 5)) <0){
		perror("ERROR on listen"); 
		close(sockfd);
        return ret; 
	}
	return sockfd;
}

/**
 * function to accept a client
 **/
int accept_client(int sockfd, struct sockaddr_in* p_cli_addr){
	
	int newsockfd, 	 	//file descriptor result of accept
		clilen;			//client address size
	
	//accept a connection
	clilen = sizeof(*p_cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) p_cli_addr, &clilen);
   
    if (newsockfd < 0) {
         perror("ERROR on accept");
		 // notice the possible errore value is propagated through newsockfd
    }
    
    return newsockfd;
} 


/**
 * function to connect client to server specifying IP address and port num
 */
int client_connection(char *IPaddr, int portno) {
	
	//socket file descriptor
	int sockfd, ret;

	//structures for socket connection
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    //create socket
    if ((ret = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR opening socket");
        return ret;
	}
	sockfd = ret;
	//get IP address
	server = gethostbyname(IPaddr);
    if (server == NULL) {
        printf("ERROR, no such host\n");
        return -1;
    }
    
    //setting data for socket connection
	memset((char *)&serv_addr, 0, sizeof(serv_addr)); // bzero() deprecated
    serv_addr.sin_family = AF_INET;
	memcpy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); // bcopy() deprecated
    serv_addr.sin_port = htons(portno);
    
    //connection
    if ((ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("ERROR in connecting");
        return ret;
	}
        
    return sockfd;  
}

