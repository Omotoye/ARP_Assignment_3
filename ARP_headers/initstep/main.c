#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "votation.h"

#define YES_OR_NO(question, yesorno) { printf( question ); if( yesorno ) printf( " yes!" ); else printf( " no." ); printf( "\n" ); }
#define SPACE printf( "\n----\n\n" )
#define PRINT_MSG(msg) printf( "counter: %d\t votes: [", msg.counter ); for( int j=0; j<iptab_len(); j++ ) printf( "%d ", msg.node[j] ); printf("\b]\n");

int main()
{
    srand(time(NULL));
    
    votation_t vtmsg;
    vote_init( &vtmsg );

    PRINT_MSG( vtmsg );

    printf("votation step!\n");
    for( int i=0; i<iptab_len(); i++)
    {
        vote_do_votation( &vtmsg );
        YES_OR_NO( "is the votation done?", vote_endVotation( &vtmsg ) );
    }

    PRINT_MSG( vtmsg );

    printf( "the winner is ... rull of drums ... %d!\n", vote_getWinner( &vtmsg )+1 );

    return 0;
}