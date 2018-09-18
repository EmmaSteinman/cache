//========================================================
// Matt Kretchmar
// April 25, 2019
// allocate2.c
// This program illustrates malloc for 2D arrays.
//========================================================

#include <stdio.h>
#include <stdlib.h>

int main ( void )
{
    int **A;        // allocation for the 2D array
    int ROWS = 5;   // number of rows
    int COLS = 3;   // number of cols
    
	
	// create an array of int pointers
	// create a pointer for each row
	A = (int **)malloc(ROWS * sizeof(int *));
	
	// create a row for each row pointer
	for ( int i = 0; i < ROWS; i++ )
	    A[i] = (int *)malloc(COLS * sizeof(int));
	    

    // store values in the 2D array	
	for ( int i = 0; i < ROWS; i++ )
	    for (int j = 0; j < COLS; j++ )
	        A[i][j] = (i*COLS) + j + 1;
	        
    // print values
	for ( int i = 0; i < ROWS; i++ )
	{
	    for (int j = 0; j < COLS; j++ )
	        printf("%2d ",A[i][j]);
	    printf("\n");
	}
	
	        
	// delete the allocated space;
	for ( int i = 0; i < ROWS; i++ )
	    free(A[i]);
	free(A);
	
	return 0;
}
