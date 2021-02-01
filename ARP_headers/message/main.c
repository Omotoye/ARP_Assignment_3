#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "message.h"

#define YES_OR_NO(question, yesorno) { printf( question ); if( yesorno ) printf( " yes!" ); else printf( " no." ); printf( "\n" ); }
#define SPACE printf( "\n----\n\n" )
#define NTEST 10

int main(int argc, char** argv)
{
    srand(time(NULL));

    printf( "inizializing the message...\n" );
    message_t msg;
    msg_init( &msg );
    SPACE;

    printf( "filling message in...\n" );
    int id = 1;
    int turnleader = 0;
    msg_set_ids( &msg, id, turnleader );
    msg_set_recv( &msg );
    msg_set_sent( &msg );
    SPACE;

    printf( "playing with bitvector in the message...\n" );
    YES_OR_NO( "was node zero visited?", msg_visited( &msg, 0 ) );
    printf( "marking node zero\n" );
    msg_mark( &msg, 0 );
    YES_OR_NO( "now, was node zero visited?", msg_visited( &msg, 0 ) );
    YES_OR_NO( "are all the nodes visited?", msg_all_visited( &msg ) );
    printf( "marking all the nodes ... (3 nodes in the network)\n" );
    msg_mark( &msg, 1 );
    msg_mark( &msg, 2 );
    YES_OR_NO( "and now, are all the nodes visited?", msg_all_visited( &msg ) );
    SPACE;

    printf( "test: random function\n" );
    printf( "no nodes are marked\n" );
    msg_init( &msg );
    for( int i=0; i<NTEST; i++ )
    {
        int n = msg_rand( &msg );
        printf( "id: %d | address: %s\n", n, iptab_getaddr(n) );
    }
    printf( "marking the first address...\n" );
    msg_mark( &msg, 0 );
    for( int i=0; i<NTEST; i++ )
    {
        int n = msg_rand( &msg );
        printf( "id: %d | address: %s\n", n, iptab_getaddr(n) );
    }
    printf( "marking also the second address...\n" );
    msg_mark( &msg, 1 );
    for( int i=0; i<NTEST; i++ )
    {
        int n = msg_rand( &msg );
        printf( "id: %d | address: %s\n", n, iptab_getaddr(n) );
    }
    SPACE;

    return 0;
}