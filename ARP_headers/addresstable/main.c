#include <stdio.h>
#include "addresstable.h"

#define YES_OR_NO(question, yesorno) { printf( question ); if( yesorno ) printf( " yes!" ); else printf( " no." ); printf( "\n" ); }
#define SPACE printf( "\n----\n\n" )

int main()
{
    printf( "TEST 1: initializatuion and flag toggle\n" );
    for( int i=0; i<iptab_len(); i++ )
    {
        printf("(%d) ", i); YES_OR_NO( "available?", iptab_is_available( i ) );
        printf("changing flag ...\n"); iptab_set_available(i);
        printf("(%d) ", i); YES_OR_NO( "now, available?", iptab_is_available( i ) );
    }

    printf( "TEST 2: reset and redo\n" );
    printf("resetting table...\n"); iptab_reset_flags();
    for( int i=0; i<iptab_len(); i++ )
    {
        printf("(%d) ", i); YES_OR_NO( "available?", iptab_is_available( i ) );
        printf("changing flag ...\n"); iptab_set_available(i);
        printf("(%d) ", i); YES_OR_NO( "now, available?", iptab_is_available( i ) );
    }
}