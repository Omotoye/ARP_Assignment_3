#include "statistics.h"
// initialize stat message
void stat_message_init(stat_t *st_msg){
	st_msg->tot_bitrate = 0.0;
	st_msg->fly_bitrate = 0.0;
}
// set the bitrate computed on the total time (sending of first message by Turn Leader, receival of last message from Turn Leader)
void stat_message_set_totBitrate(stat_t *st_msg, float tot_bitrate){
	st_msg->tot_bitrate = tot_bitrate;
}
// set the bitrate computed 
void stat_message_set_flyBitrate(stat_t *st_msg, float fly_bitrate){
	st_msg->fly_bitrate = fly_bitrate;
}

void stat_get_serverInfo(char** p_strIP, int* p_portno){
	*(p_strIP) = RURZ_IP_STR;
	*(p_portno)= RURZ_PORTNO;
}
