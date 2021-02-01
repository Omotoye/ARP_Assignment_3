/**************************************//* 
*	This is not an executable or even 
*	compilable file, but simply a 
*	collection of "how-to" regarding the
*	use of the presented libraries.
*	Note that this has still to undergo
*	revision, so take it as "general,
*	hopefully correct, guidance".
*****************************************/
// First, including
#include "address-table-simple/addresstable.h"
#include "init-step/votation.h"
#include "message/message.h"
#include "arpnet/arpnet.h"
#include "misc/random_index.h"

#define OUTERLOOPS 10
#define INNERLOOPS 10

node_id MY_INDEX = 42; // hard coding our position in the IP_table
// Not the best, but the easiest, other ideas to retrieve it are more
// than welcomed
int WELL_DEF_PORT = 54321; // again, hard coding the port each node 
// should listen to/ send from, it would be better if inserted in one
// of the libs, still to define which one

/*
	Votation step, from within the current TURN LEADER
*/
START_VOTATION_STEP:
{
	// Last thing a Turn Leader does before passing the torch
	votation_t vot_msg;
	node_id elected_node;
	
	vote_init(&vot_msg); //::votation.h
	vote_do_votation(&vot_msg);	//::votation.h
	
	// Send this to the next node in the table
	char* next_addr_str = iptab_getaddr( (MY_INDEX+1)%iptab_len() ); //::addresstable.h
	int next_sockfd = client_connection(next_addr_str, WELL_DEF_PORT); //::arpnet.h
	write(next_sockfd, &vot_msg, sizeof(vot_msg));
	
	// wait for a response, read the elected node
	int prev_sockfd = accept_client(); //::arpnet.h
	read(prev_sockfd, &vot_msg, sizeof(vot_msg));
	elected_node = vote_getWinner(&vot_msg); //::votation.h
	
	// open a communication channel with the elected node
	char* nextTL_addr_str = iptab_getaddr(elected_node); //::addresstable.h
	int nextTL_sockfd = client_connection(nextTL_addr_str, WELL_DEF_PORT); //::arpnet.h
	
	// send to the new Turn Leader an empty message, containing only the
	// node_id of the Turn Leader, aka that same node
	message_t msg;
	msg_init(&msg);	//::message.h
	msg_set_ids(&msg, MY_INDEX, elected_node); //::message.h
	msg_mark(&msg, MY_INDEX); //::message.h
	write(nextTL_sockfd, &msg, sizeof(msg));
	
}

// Votation step as seen from any node, before starting
// to send messages
INNER_VOTATION_STEP:
{
	votation_t vot_msg;
	// wait for a response, read the elected node
	int prev_sockfd = accept_client(); //::arpnet.h
	read(prev_sockfd, &vot_msg, sizeof(vot_msg));
	
	vote_do_votation(&vot_msg);	//::votation.h
	
	// Send this to the next node in the table
	char* next_addr_str = iptab_getaddr( (MY_INDEX+1)%iptab_len() ); //::addresstable.h
	int next_sockfd = client_connection(next_addr_str, WELL_DEF_PORT); //::arpnet.h
	write(next_sockfd, &vot_msg, sizeof(vot_msg));
	
}

MESSAGE_SEND_RAND:
{
	// wait for a message, read
	message_t msg;
	int prev_sockfd = accept_client(); //::arpnet.h
	read(prev_sockfd, &msg, sizeof(msg));
	msg_set_recv(&msg); // overwrite receival time as soon as it arrives
	// from the received message we're actually interested only in
	// the "already visited nodes" list, since the content has already
	// been routed towards the current Turn Leader
	node_id next_node;
	if(msg_numberVisited(&msg)<INNERLOOPS){
		// open a communication channel with a random unvisited node
		next_node = msg_rand(&msg);  //::message.h
	}
	else{ // this is the 10th node, the next hop should be the Turn Leader
		next_node = msg.turnLeader;  //::message.h
	}
	char* next_addr_str = iptab_getaddr(next_node); //::addresstable.h
	int next_sockfd = client_connection(next_addr_str, WELL_DEF_PORT); //::arpnet.h
	// Fill the various fields, setting this node_id as marked
	msg_set_ids(&msg, MY_INDEX, msg.turnLeader); //::message.h
	msg_mark(&msg, MY_INDEX); //::message.h
	msg_set_sent(&msg); //::message.h
	write(next_sockfd, &msg, sizeof(msg));
	
}

// the entire thing, in broad terms, pseudocode
/*
	NOTICE: each node that isn't a turn leader
	has no idea of how many messages it will 
	still receive before having to go back to
	the VOTATION phase (it could be visited 10,
	0 or any inbetween number of times): to
	detect that we should have a special msg,
	of the same type as the ones containing 
	informations, but with some sort of flag
	that indicates that a new round has to
	start. This pasrticular message will have
	to be sent to all nodes in the net (using
	once again the "chain" would be optimal),
	so that they can stop waiting for a message
	to reply to and participate in the new
	votation step.
*/
WHOLE_PROCESS:
{
	for(int i=0; i<OUTERLOOPS; i++){
		INNER_VOTATION_STEP
		MESSAGE_SEND_RAND // first iteration outside the loop, to gather whetere this node is the new TL
		AM_I_THE_TURN_LEADER?
		YES:
			for(int j=0; j<INNERLOOPS-1; j++){
				MESSAGE_SEND_RAND
				for(int k=0; k<INNERLOOPS; k++){
					COLLECT_DATA
				}
				SEND_DATA_TO_SERVER
			}
			START_VOTATION_STEP
		NO:
			for(int j=0; j<INNERLOOPS-1; j++){{
				MESSAGE_SEND_RAND
			}
	}
	
}