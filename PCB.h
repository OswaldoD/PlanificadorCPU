#include <stdio.h>
#include <stdlib.h>

typedef struct PCB_Type
{
	int PID;
	char* estado;
	int burst;
	int prioridad;
	int t_exec;
	int t_wait;
	int t_end;
	 // se ocupa mas para context switch
	
}PCB;
