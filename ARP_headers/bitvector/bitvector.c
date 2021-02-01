#include "bitvector.h"

// inizialization of the bit vector
int bv_init( bitvector_t* bv, int len )
{
    if( len < 1 )
        return -1;
    if( bv == NULL )
        return -1;

    bv->len = len;
    RIGHT_BITMASK( bv->v, bv->len );

    return 0;
}

// it sets all the bits of the vector to zero
void bv_clear( bitvector_t* bv )
{
    RIGHT_BITMASK( bv->v, bv->len );
}

// check if a node is marked as visited
int bv_marked( bitvector_t* bv, int pos )
{
    if( pos < 0 ) return -1;
    if( pos >= bv->len ) return -1;

    // example
    // pos 4
    // 1000 <- bit
    // 1111 <- bit vector
    // 1000
    unsigned int bit = BIT( pos );
    if( ( bit & bv->v ) == 0 )
        return 1;
    else
        return 0;
}

// mark a node as visited
void bv_mark( bitvector_t* bv, int pos )
{
    unsigned int bit = ~BIT( pos );
    bv->v &= bit;
}

// check if at least one node is available
int bv_all_marked( bitvector_t* bv )
{
    unsigned int bmask = 0;
    RIGHT_BITMASK( bmask, bv->len );

    if( ( bmask & bv->v ) == 0 )
        return 1;
    else
        return 0;
}
