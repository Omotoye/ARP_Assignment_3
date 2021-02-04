#ifndef _STATISTICS_H_
#define _STATISTICS_H_

char* RURZ_IP_STR = '\0';
int RURZ_PORTNO = -1;

// the type of the message reserved to saving the statistics
typedef struct stat_message
{
    float tot_bitrate;
    float fly_bitrate;
}
stat_t;

/**********************************************//**
* Simple initialization of the message, proforma
*
* Arguments
*   st_msg - pointer to the message to initialize 
*
**************************************************/
void stat_message_init(stat_t *st_msg);

/**********************************************//**
* Fill the tot_bitrate field in the passed message
*
* Arguments
*   st_msg - pointer to the message in which to set
*			the bitrate
*	tot_bitrate - total bitrate value computed
*
**************************************************/
void stat_message_set_totBitrate(stat_t *st_msg, float tot_bitrate);

/**********************************************//**
* Fill the fly_bitrate field in the passed message
*
* Arguments
*   st_msg - pointer to the message in which to set
*			the bitrate
*	fly_bitrate - total fly-time bitrate value 
+			computed
*
**************************************************/
void stat_message_set_flyBitrate(stat_t *st_msg, float fly_bitrate);

/**********************************************//**
* A way of retrieving RURZserver infos
*
* Return values are stored in the pointed arguments.
*
* Arguments
*   **p_strIP - pointer to the string in which the
*			IPv4 address of the server will be saved
*	*p_portno - pointer to the int in which the
*			port number of the server will be saved
*
**************************************************/
void stat_get_serverInfo(char** p_strIP, int* p_portno);

#endif