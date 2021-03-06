/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int blocki, blockj, i, j, temp, diag;
	for (blocki = 0; blocki < 32; blocki+=8)			//goes through columns in blocks of 8
	{
		for (blockj = 0; blockj < 32; blockj+= 8)		//goes through rows in blocks of 8
		{
			for (i = blocki; i< (blocki+8); i++)		//goes through columns of blocks
			{
				for (j = blockj; j< (blockj+8); j++)	//goes through rows of blocks
				{
					if (i!=j)
						B[j][i] = A[i][j];				//if not on diagonal, swap row and column
					else
					{
						temp = A[i][j];					//save value in diagonal
						diag = j;						//save coordinates
					}
					
				}	
				if (blocki==blockj)
					B[diag][diag] = temp;			//set value on diagonal without accessing it in cache
			}
		}
	}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char trans_block[] = "skipping diagonal";
void trans_blocking(int M, int N, int A[N][M], int B[N][M])
{
	int i, j, tmp1, tmp2;
		for (i = 0; i < N; i++)
		{
			for (j = i; j < M; j++)
			{
				tmp1 = A[i][j];
				tmp2 = A[j][i];
				B[j][i] = tmp1;
				B[i][j] = tmp2;
			}
		}
	


}

char trans_skip[] = "skipping";
void trans1(int M, int N, int A[N][M], int B[M][N])
{
    int blocki, blockj, i, j;
	for (blocki = 0; blocki < N; blocki+=8)
	{
		for (blockj = 0; blockj < M; blockj+= 8)
		{
			for (i = blocki; i< (blocki+8); i++)
			{
				for (j = blockj; j< (blockj+8); j++)
				{
					B[j][i] = A[i][j];
					
					
				}			
			}
		}
	}   

}
/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
	registerTransFunction(trans_blocking, trans_block); 
	registerTransFunction(trans1, trans_skip);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

