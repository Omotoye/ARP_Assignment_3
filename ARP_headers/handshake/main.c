#include <stdio.h>
#include <string.h>
#include "handshake.h"

int main()
{
    printf( " a is equal to a ... isn't it? %d\n", strcmp( "a", "a" ) );
    printf( " a is equal to b ... isn't it? %d\n", strcmp( "a", "b" ) );

    char str1[] = "a string to be copied";
    char str2[50];
    strcpy( str2, str1 );
    printf( "TO COPY: %s\nCOPIED: %s\n", str1, str2 );

    return 0;
}