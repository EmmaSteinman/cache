#include "cachelab.h"
#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    int     V;
    int   	tag;
	int 	time;
} Line;

typedef struct
{
    Line 	*lines;
} Set;

typedef struct
{
	int 	numSets;
	int		numLines;
	Set		*sets;
} Cache;



int main(int argC, char *argV[])
{
//	printf("%d\n", argC);
	int s, E, b;
	int print = 0;
	char buffer[1000];
	char* file;
	FILE *trace;
	//static int r[3];
/*
	while ((opt = getopt(argC, argV, "vs:E:b:t:"))!=-1)
	{
		switch(opt)
		{
			case 'v':
				printf("%s", "v: ");
				printf("%s\n", optarg);
				break;
			case 's':
				printf("%s", "s: ");
				printf("%s\n", optarg);
				s = atoi(optarg);
				break;
			case 'E':
				printf("%s", "E: ");
				printf("%s\n", optarg);
				E = atoi(optarg);
				break;
			case 'b':
				printf("%s", "b: ");
				printf("%s\n", optarg);
				b = atoi(optarg);
				break;
			case 't':
				file = optarg;
				printf("%s\n", file);
				break;
		}
	}*/
	//make better error 
	if (argC< 7)							//not enough arguments
	{
		printf("%s\n", "Invalid input");
		exit(1);
	}
	for (int i = 0; i < argC; i++)						//parse arguments
	{		
		if (strcmp(argV[i],"-h")==0)					//help arg?
			{printf("%s\n", "Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\nOptions:\n-h\tPrint this help message.\n-v 				\tOptional verbose flag.\n-s <num> \tNumber of set index bits.\n-E <num> \tNumber of lines per set.\n-b <num> \tNumber of 				block offset bits.\n-t <file>  \tTrace file.\nExamples: \nlinux>\t./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace");}
		else if (strcmp(argV[i],"-v")==0)				//verbose arg
			print = 1;
		else if (strcmp(argV[i],"-E")==0)				//E - #lines per set
		{
			E = atoi(argV[i+1]);
			i++;
		}
		else if (strcmp(argV[i],"-s")==0)				//s - set index bits
		{
			s = atoi(argV[i+1]);
			i++;
		}
		else if (strcmp(argV[i],"-b")==0)				//b - block index bits
		{
			b = atoi(argV[i+1]);
			i++;
		}
		else if (strcmp(argV[i],"-t")==0)				//t - trace
		{
			file = argV[i+1];
			i++;
		}
		//need error
	}
	printf("%s\n", file);
	int S = pow(2, s);
	int tagb = s+b;
//	int B = math.pow(2, b);
	printf("%s", "b: ");
	printf("%d\n", b);
	Cache cache;
	cache.numSets = S;
	cache.numLines = E;
	cache.sets = (Set*)malloc(cache.numSets*sizeof(Set));	//allocate array for sets
	for (int i = 0; i < cache.numSets; i++)
	{
		cache.sets[i].lines = (Line*)malloc(cache.numLines*sizeof(Line));
		for (int j = 0; j < cache.numLines; j++)		//array for lines per set
		{
			cache.sets[i].lines[j].V = 0;				//initialize valid bits to 0
			cache.sets[i].lines[j].time = 0;			//initialize tags to 0
		}
	}

	char operation;
	unsigned long address;
	int size, taggedBits, setBits, found; 
	int tagMask = 0x7fffffff<<tagb;						//all 1s in tagged bits
	int setMask = (0x7fffffff<< b)^tagMask;				//only 1s in set bits
	trace = fopen(file, "r");
	if (trace == NULL)
	{
		printf("%s\n", "File not found.");				//file not found
		exit(EXIT_FAILURE);
	}

	int hits = 0;
	int miss = 0;										//initialize values
	int evict = 0;
	int min, LRU;
	int times = 0;
	while ((fgets(buffer, 1000, trace))!= NULL)			//reads in each line
	{
		if (buffer[0] != 'I')
		{
			
//			printf("%s\n", "---------------------------------------------------------");
			sscanf(buffer, " %c %lx,%d", &operation, &address, &size);
//			printf("%s", "operation: ");
//			printf("%c\n", operation);
//			printf("%s", "address: ");
//			printf("%lx\n", address);
//			printf("%s", "size: ");
//			printf("%d\n", size);
			taggedBits = (address & tagMask)>>tagb;		//get tagged bits
			setBits = (address & setMask)>>b;			//get set index bits
//			printf("%s", "setBits: ");
//			printf("%d\n", setBits);
//			printf("%s", "taggedBits: ");
//			printf("%d\n", taggedBits);
			found = 0;
			for (int r = 0; r < cache.numLines; r++)					//look through cache 
			{															//first time
				if (cache.sets[setBits].lines[r].V==1)
				{
					if (cache.sets[setBits].lines[r].tag == taggedBits)	//hit
					{ 
						hits++;
						cache.sets[setBits].lines[r].time = ++times;	//increase timestamp
						if (operation == 'M')							//L or S
						{
							hits++;
							cache.sets[setBits].lines[r].time = ++times;//increase LRU array
						}
						if (print)										//verbose mode
						{
							printf("%c\t", operation);
							printf("%lx\t", address);
							printf("%d\t", size);
							if (operation != 'M')
								printf("%s\n", "hit");	
							else 
								printf("%s\n", "hit hit");
						}
						found = 1;								//will not look for miss
						r = cache.numLines;						//exit for loop
					}
				}
			}
			if (!found)											//miss
			{

				for (int n = 0; n < cache.numLines; n++)
				{
					if (cache.sets[setBits].lines[n].V==0)
					{
						cache.sets[setBits].lines[n].V = 1;					//insert data
						cache.sets[setBits].lines[n].tag = taggedBits;
						cache.sets[setBits].lines[n].time = ++times;//increase timestamp
						if (operation != 'M')						//L or S
							miss++;
						else										//M
						{
							miss++;
							hits++;
							cache.sets[setBits].lines[n].time = ++times;//increase timestamp
						}

						if (print)										//verbose mode
						{
							printf("%c\t", operation);
							printf("%lx\t", address);
							printf("%d\t", size);
							if (operation != 'M')
								printf("%s\n", "miss");
							else 
								printf("%s\n", "miss hit");
						}	
						found = 1;									//successfully in cache
						n = cache.numLines;							//exit for loop		
					}
				}
			}
			
			if (!found)											//need to evict
			{
				min = 0;
				LRU = cache.sets[setBits].lines[min].time;		//set LRU to first line in set
				for (int f = 1; f < cache.numLines; f++)
				{
					if (cache.sets[setBits].lines[f].time < LRU)	//finds LRU comparing to
					{												//first line
						min = f;
						LRU = cache.sets[setBits].lines[f].time;
					}	
				}
//				printf("%s", "to be evicted: ");
//				printf("%d\n", cache.sets[setBits].lines[min].tag);
				cache.sets[setBits].lines[min].V = 1;					//evict :(
				cache.sets[setBits].lines[min].tag = taggedBits; 
				miss++;
				evict++;
				cache.sets[setBits].lines[min].time =++times;	//increase timestamp
				if (operation == 'M')
				{
					hits++;
					cache.sets[setBits].lines[min].time = ++times;//increase timestamp
				}

				if (print)
				{
					printf("%c\t", operation);					//verbose mode
					printf("%lx\t", address);
					printf("%d\t", size);
					if (operation != 'M')
						printf("%s\n", "miss eviction");
					else
						printf("%s\n", "miss eviction hit");
				}

			}

			
		}
	}

	
// deallocate memory
	for (int k = 0; k < cache.numSets; k++)
	{
		free(cache.sets[k].lines);
	}
	free(cache.sets);
    printSummary(hits,miss, evict);
    return 0;
}




