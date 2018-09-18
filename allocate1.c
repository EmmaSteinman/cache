//========================================================
// Matt Kretchmar
// April 25, 2019
// allocate1.c
// This program illustrates simple use of malloc and free.
//========================================================

#include <stdio.h>
#include <stdlib.h>

int main ( void )
{
	int *iptr;
	
	// create space for a single integer
	iptr = (int *)malloc(sizeof(int));
	
	*iptr = 10;
	printf("iptr: %d\n",*iptr);
	
	// delete the allocated space;
	free(iptr);
	
	return 0;
}
