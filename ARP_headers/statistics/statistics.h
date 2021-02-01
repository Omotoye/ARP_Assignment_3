#ifndef _STATISTICS_H_
#define _STATISTICS_H_
#include "../addresstable/addresstable.h"

// the type of the message reserved to saving the statistics
typedef struct stat_message
{
    float *bitrate;
}
stat_t;

// size contains the number of fields in "bitrate", namely INNERLOOPS
void stat_message_init(stat_t *st_msg, int size);
// in the .c file there will be the last position to save in 
void stat_message_add_bitrate(stat_t *st_msg);

#endif