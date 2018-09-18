//======================================
// Matt Kretchmar
// April 25, 2018
// cmdline.c
// Illustrates simple command line params
//======================================

#include <stdio.h>
#include <stdlib.h>

int main ( int argc, char *argv[] )
{
    for ( int i = 0; i < argc; i++ )
        printf("%s\n",argv[i]);

    return 0;
}
