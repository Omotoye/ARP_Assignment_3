#include <stdio.h>
#include <limits.h>
#include "bitvector.h"

#define BITVECTOR_LEN 4

int main( int argc, char** argv )
{
    printf( "small test on bit operators\n" );
    unsigned int bitval = 1;
    printf( "macro BIT()\n" );
    for( int i=0; i<BITVECTOR_LEN; i++ ) printf( "%u\n", BIT( i ) );
    printf( "macro RIGHT_BITMASK()\n" );
    for( int i=0; i<BITVECTOR_LEN; i++ )
    {
        int value = 0;
        RIGHT_BITMASK( value, i );
        printf( "%u\n", value );
    }
    printf("\n");

    printf( "test: intiializing a bit vector\n" );
    bitvector_t bitv;
    bv_init( &bitv, BITVECTOR_LEN );

    printf( "test: are all nodes marked? " );
    if( bv_all_marked( &bitv ) ) printf( "yes, of course!\n" );
    else                         printf( "no: there are unmarked nodes.\n" );

    printf( "marking the first entry of the vector\n" );
    if( bv_marked( &bitv, 0 ) )  printf( "first bit is marked\n" );
    else                         printf( "first bit is unmarked\n" );
    printf( "marking...\n" ); bv_mark( &bitv, 0 );
    if( bv_marked( &bitv, 0 ) )  printf( "first bit is marked\n" );
    else                         printf( "first bit is unmarked\n" );
    printf( "test: are all nodes marked? " );
    if( bv_all_marked( &bitv ) ) printf( "yes, of course!\n" );
    else                         printf( "no: there are unmarked nodes.\n" );

    printf( "marking the remaining bits...\n" );
    bv_mark( &bitv, 1 );
    bv_mark( &bitv, 2 );
    bv_mark( &bitv, 3 );
    printf( "test: are all nodes marked? " );
    if( bv_all_marked( &bitv ) ) printf( "yes, of course!\n" );
    else                         printf( "no: there are unmarked nodes.\n" );

    return 0;
}