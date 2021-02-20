#include "arpnet.h"
#define MY_IP_ADDR "13.80.128.21"

int retval;

int main(int argc, char *argv[])
{
    // get my index in the table
    node_id MY_INDEX = iptab_get_ID_of(MY_IP_ADDR);
    handshake_t hs_mes;

    // Initiailizing the Socket
    int sockfd;
    if ((sockfd = net_server_init()) < 0)
        perror("Error in Connection");

    // Accepting Connection
    int newsockfd;
    if ((newsockfd = net_accept_client(sockfd, NULL)) < 0)
        perror("Error in Accepting Connection");

    // Reading the handshake message
    if (read(newsockfd, &hs_mes, sizeof(handshake_t)) == -1)
        error("ERROR in reading from client");

    // Checking availability of the node
    retval = hsh_check_availability(MY_INDEX, &hs_mes);

    // Checking the next available node
    node_id next_avail_node;
    char *next_avail_IP;
    char *turn_leader_IP;
    next_avail_node = iptab_get_next(MY_INDEX);
    next_avail_IP = iptab_getaddr(next_avail_node);

    // Connecting to the next available node
    if ((sockfd = net_client_connection(next_avail_IP)) < 0)
        perror("Error in connecting to client");

    // Writing the handshake message to the next available node
    if (write(sockfd, &hs_mes, sizeof(handshake_t)) == -1)
        perror("ERROR in writing to server");

    // Exiting incase the header file is not up to date
    if (retval == 0)
    {
        printf("The header file version is not up-to-date\n");
        return -1;
    }

    // Waiting for the message to return after going through the cycle
    if ((sockfd = net_server_init()) < 0)
        perror("Error in Connection");

    // Accepting the Connection
    int newsockfd;
    if ((newsockfd = net_accept_client(sockfd, NULL)) < 0)
        perror("Error in Accepting Connection");

    // Reading the final handshake message
    if (read(newsockfd, &hs_mes, sizeof(handshake_t)) == -1)
        error("ERROR in reading from client");

    // Updating the address table
    hsh_update_iptab(&hs_mes);

    // Checking the next available node
    next_avail_node = iptab_get_next(MY_INDEX);
    next_avail_IP = iptab_getaddr(next_avail_node);

    // Connecting to the next available node
    if ((sockfd = net_client_connection(next_avail_IP)) < 0)
        perror("Error in connecting to client");

    // Writing the handshake message to the next available node
    if (write(sockfd, &hs_mes, sizeof(handshake_t)) == -1)
        perror("ERROR in writing to server");

    // initializing the votation message
    votation_t vote_mes;

    // Initiailizing the Socket
    if ((sockfd = net_server_init()) < 0)
        perror("Error in Connection");

    // Accepting Connection
    if ((newsockfd = net_accept_client(sockfd, NULL)) < 0)
        perror("Error in Accepting Connection");

    // Reading the votation message
    if (read(newsockfd, &vote_mes, sizeof(votation_t)) == -1)
        error("ERROR in reading votation message");

    // Vote for a random node
    vote_do_votation(&vote_mes);

    // Checking the next available node
    next_avail_node = iptab_get_next(MY_INDEX);
    next_avail_IP = iptab_getaddr(next_avail_node);

    // Connecting to the next available node
    if ((sockfd = net_client_connection(next_avail_IP)) < 0)
        perror("Error in connecting to client");

    // Writing the updated votation message to the next available node
    if (write(sockfd, &vote_mes, sizeof(votation_t)) == -1)
        perror("ERROR in writing to node");

    // Waiting for the common message
    node_id turn_leader;
    message_t common_mes;
    // Initiailizing the Socket
    if ((sockfd = net_server_init()) < 0)
        perror("Error in Connection");

    // Accepting Connection
    if ((newsockfd = net_accept_client(sockfd, NULL)) < 0)
        perror("Error in Accepting Connection");

    // Reading the votation message
    if (read(newsockfd, &common_mes, sizeof(message_t)) == -1)
        error("ERROR in reading votation message");

    turn_leader = common_mes.turnLeader;
    if (turn_leader == MY_INDEX)
    {
        printf("I am the turn leader\n");

        struct timeval recv_time_store;
        struct timeval sent_time_store;

        stat_t stat_mes;
        stat_message_init(&stat_mes);
        char *RURZ_addr;

        // Initialize the common message as the turn leader
        msg_init(&common_mes);

        // Setting the required ID's for the common message
        msg_set_ids(&common_mes, MY_INDEX, turn_leader);

        // Mark this node as visited
        msg_mark(&common_mes, MY_INDEX);

        // Mark all the unavailable nodes as unvisited
        for (int k = 0; k < iptab_len(); k++)
        {
            if (!iptab_is_available(k))
                msg_mark(&common_mes, k);
        }

        // Choose random node
        int rand_node = msg_rand(&common_mes);
        char *rand_node_IP = iptab_getaddr(rand_node);

        // Setting sending time
        msg_set_sent(&common_mes);

        // Storing the sending time
        struct timeval my_first_send;
        struct timeval my_recv_last;
        struct timeval send_time;
        struct timeval recv_time;
        long int difference;
        long int fly_time;
        long int total_time;

        my_first_send = msg_get_sent(&common_mes);

        // Connecting to the next available node
        if ((sockfd = net_client_connection(rand_node_IP)) < 0)
            perror("Error in connecting to client");

        // Writing the updated common message to the next available unvisited node
        if (write(sockfd, &common_mes, sizeof(message_t)) == -1)
            perror("ERROR in writing to node");

        for (int j = 0; j < (iptab_len_av() - 1); j++)
        {
            // Initiailizing the Socket
            if ((sockfd = net_server_init()) < 0)
                perror("Error in Connection");

            // Accepting Connection
            if ((newsockfd = net_accept_client(sockfd, NULL)) < 0)
                perror("Error in Accepting Connection");

            // Reading the common message
            if (read(newsockfd, &common_mes, sizeof(message_t)) == -1)
                error("ERROR in reading votation message");
            send_time = msg_get_sent(&common_mes);
            recv_time = msg_get_recv(&common_mes);
            difference = ((send_time.tv_sec * 1000000000) + send_time.tv_usec) - ((recv_time.tv_sec * 1000000000) + recv_time.tv_usec);
            fly_time = fly_time + difference;

            if (j == (iptab_len_av - 2))
            {
                my_recv_last = msg_get_recv(&common_mes);
            }
        }
        total_time = ((my_first_send.tv_sec * 1000000000) + my_first_send.tv_usec) - ((my_recv_last.tv_sec * 1000000000) + my_recv_last.tv_usec);
        int bit = 8 * sizeof(message_t);
        long int avg_tot_time = total_time / 10;
        float tot_bandwidth = bit / avg_tot_time;
        long int avg_fly_time = fly_time / 10;
        float fly_bandwidth = bit / avg_fly_time;

        stat_message_set_totBitrate(&stat_mes, tot_bandwidth);
        stat_message_set_flyBitrate(&stat_mes, fly_bandwidth);

        RURZ_addr = stat_get_RURZ_addr();

        // Connecting to the RURZ IP address
        if ((sockfd = net_client_connection(next_avail_IP)) < 0)
            perror("Error in connecting to client");

        // Writing the stat message to RURZ IP Address
        if (write(sockfd, &stat_mes, sizeof(stat_t)) == -1)
            perror("ERROR in writing to node");
    }

    else
    {
        // Set the reception time of the messaga
        msg_set_recv(&common_mes);

        // Mark node as visited
        msg_mark(&common_mes, MY_INDEX);

        // Check if there is at least one node that is not yet visited
        retval = msg_all_visited(&common_mes);
        if (retval == 1)
        {
            // Get the id of the turn leader
            turn_leader = msg_get_turnLeader(&common_mes);

            // Connecting to the turn leader
            if ((sockfd = net_client_connection(next_avail_IP)) < 0)
                perror("Error in connecting to client");

            // Writing the updated common message to the turn leader
            if (write(sockfd, &common_mes, sizeof(message_t)) == -1)
                perror("ERROR in writing to node");
        }
        else if (retval == 0)
        {
            // Get the next available node ID
            if ((next_avail_node = msg_rand(&common_mes)) < 0)
                printf("There are no available unvisited node\n");

            // Setting the required ID's for the common message
            msg_set_ids(&common_mes, MY_INDEX, turn_leader);

            // Setting sending time
            msg_set_sent(&common_mes);

            // Connecting to the next available node
            next_avail_IP = iptab_getaddr(next_avail_node);
            if ((sockfd = net_client_connection(next_avail_IP)) < 0)
                perror("Error in connecting to client");

            // Writing the updated common message to the next available unvisited node
            if (write(sockfd, &common_mes, sizeof(message_t)) == -1)
                perror("ERROR in writing to node");

            // Connecting to the turn leader to send thesame message to it
            turn_leader_IP = iptab_getaddr(turn_leader);
            if ((sockfd = net_client_connection(turn_leader_IP)) < 0)
                perror("Error in connecting to client");

            // Writing the updated common message to the turn leader
            if (write(sockfd, &common_mes, sizeof(message_t)) == -1)
                perror("ERROR in writing to node");
        }
    }

    return 0;
}