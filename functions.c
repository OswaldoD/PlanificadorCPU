#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "functions.h"


int randomNM(int n, int m, int seed)
{ 
	// aleatorio de n a m
//	srand(seed);

	int result=0,low_num=0,hi_num=0;
	if(n<m)
	{
		low_num=n;
        hi_num=m+1; // this is done to include max_num in output.
    }
    else{
        low_num=m+1;// this is done to include max_num in output.
        hi_num=n;
    }
    //srand(seed); porque la semilla me tira aleatorios iguales? se setea en otro lado?
    result = (rand()%(hi_num-low_num))+low_num;
    return result;

}