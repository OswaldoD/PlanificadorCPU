/*
toda la intro
*/

// carga de archivos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cliente.h"
#include "PCB.h"

int main(int argc, char *argv[])
{ 
	printf("Hola\n");
	openFile(argv[1]);
}

/*
 * Abre un archivo y retorna un PCB con la informacion del archivo, 
 * falta que retorne
 * 
 * */
void openFile(char* route)
{
	FILE *fp;
	char buffer[255];
	
	fp = fopen(route, "r"); // tipo solo lectura
	
	int number;
	int counter = 1;
	
	while (fscanf(fp, "%s", buffer) != EOF){
		number = atoi(buffer);
		printf("->  %i  ", number);
		if (counter == 3)
		{
			printf("\ncambio linea %i\n\n", counter);
			counter = 1 ;
		}
		else
		{
			counter++;
		}
		

	}
	
	fclose(fp);
}

/*
 * Parametros aleatorios
 * este no se detiene hasta que le indicque
 * Burst (1-20)
 * Prioridad (1-5)
 * 
 * */
void randomParameters()
{
	
	
}


