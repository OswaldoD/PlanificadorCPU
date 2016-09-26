#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "PCB.h"

#define n 9999

typedef struct Cola_Procesos{	
	struct PCB_Type cola[n];
}colaProcesos;
