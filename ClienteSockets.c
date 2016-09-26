#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <string.h>
#include "functions.c"
 
int ConfiguracionCliente()
{
    /*Se define el descriptor del cliente*/
    int clienteSocketDescriptor;
    
    /*Se define la estructura del Cliente*/
    struct sockaddr_in serv_addr;
    
    /*Se asigna el tamaño de la estructura*/
    socklen_t addrLEN = sizeof (serv_addr);
 
    /*Familia usada y declara del socket en este caso el TCP
     * PF_INET = especifica la familia del protocolo (en este caso el protocolo TCP/IP)
     * SOCK_STREAM = indica el tipo de servicio que se está utilizando
     * IPPROTO_TCP = especifica el protocolo, en este caso particular el que defina por defecto la familia que se eligió*/
    clienteSocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (clienteSocketDescriptor < 0)//if encargado de validar que se cree el socket
    {
        printf("Error en el socket\n");
        exit(1);
    }
    
    /*Zera a estrutura*/
    bzero((char *) & serv_addr, sizeof (serv_addr));
    
    /*Se asigna la familia*/
    serv_addr.sin_family = AF_INET;
    
    /*Se asigna la dirección específica de la estructura al descriptor del socket*/
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//localhost es el número de IP escogido
    
    /*Se define el puerto*/
    serv_addr.sin_port = htons(6881);
    
   /*Función encargada de realizar la conexión con el servidor*/
   if(connect(clienteSocketDescriptor,(struct sockaddr *)&serv_addr, addrLEN) < 0)
     {
            printf("Error en el socket\n");
			exit(1);
    }
    
    return clienteSocketDescriptor; //retorna el descriptor del socket
}
 
/*
 
void Cliente(int clienteSocketDescriptor)
{
  Se declara el buffer de datos a ser enviados al servidor
  char BufferHaciaServidor[256];
  
  //Se solicita el mensaje para el servidor
  printf("Igrese el mensaje apra el servidor: ");
  
  //Este ciclo se ejecuta mietras existan procesos para enviarle al servidor
   do {
 
        scanf("%s",BufferHaciaServidor);// se almacena el mensaje en el buffer
        fflush(stdin);
        
		Esta función se encarga de escribir datos a través del descriptor del socket
        write(clienteSocketDescriptor, BufferHaciaServidor, sizeof (BufferHaciaServidor));
 
      Se encarga de verificar que existan procesos para pasarle al descriptor
    } while (strcmp(BufferHaciaServidor, "salir") != 0);
    
    En caso de ya no existir procesos apra enviar al servidor se cierra el descriptor del socket
    close(clienteSocketDescriptor);
}

*/
void Cliente(int clienteSocketDescriptor, int numbers[])
{
  /*Se declara el buffer de datos a ser enviados al servidor*/
  int BufferHaciaServidor[5];

  BufferHaciaServidor[0] = numbers[0];
  BufferHaciaServidor[1] = numbers[1];
  BufferHaciaServidor[2] = numbers[2];
  BufferHaciaServidor[3] = numbers[3];
  BufferHaciaServidor[4] = numbers[4];
  
  //Se solicita el mensaje para el servidor
  //printf("Igrese el mensaje apra el servidor: ");
  
  //Este ciclo se ejecuta mietras existan procesos para enviarle al servidor
   //do {
 
     //   scanf("%s",BufferHaciaServidor);// se almacena el mensaje en el buffer
      //  fflush(stdin);
        
		/*Esta función se encarga de escribir datos a través del descriptor del socket*/
  printf("Enviando...%i\n", BufferHaciaServidor[2]);
        write(clienteSocketDescriptor, BufferHaciaServidor, sizeof (BufferHaciaServidor));
 printf("Enviado...\n");
      /*Se encarga de verificar que existan procesos para pasarle al descriptor*/
    //} while (strcmp(BufferHaciaServidor, "salir") != 0);
    
    /*En caso de ya no existir procesos apra enviar al servidor se cierra el descriptor del socket*/
    close(clienteSocketDescriptor);
}
int passData(int numbers[])
{
	//system("clear");//llamada al sistema para una limpieza de la consola
		
	/*Se declara la estructura del cliente*/
		
	struct sockaddr_in serv_addr;
		
	/*Se asigan el tamaño de la estructura*/
    socklen_t addrLEN = sizeof (serv_addr);
	
	/*DSe declara el descriptor del cliente*/
	int descriptorCliente;
	descriptorCliente = ConfiguracionCliente();
		
	/*Llama y ejecuta la función del cliente pasando como parámentro el descriptor del socket del cliente*/
	Cliente(descriptorCliente, numbers);
}
/**
 * Modo automático
 * Genera aleatoriamente los burst y prioridades 
 * de los procesos
 **/
int autoMode()
{
	int ID = 1;
	int numbers[5];
	int times = 1;
	int nn = 50;
	int seed = 79;
	//srand(7919);
/**/
		//system("clear");//llamada al sistema para una limpieza de la consola
		
	/*Se declara la estructura del cliente*/
		
	struct sockaddr_in serv_addr;
		
	/*Se asigan el tamaño de la estructura*/
    socklen_t addrLEN = sizeof (serv_addr);
	
	/*DSe declara el descriptor del cliente*/
	int descriptorCliente;
	descriptorCliente = ConfiguracionCliente();
		
	/*Llama y ejecuta la función del cliente pasando como parámentro el descriptor del socket del cliente*/
	
/**/

	while(times){
		srand(seed);
		if(nn == 0){
			times = 0;
		}
		else{

			numbers[0] = 1;
			numbers[1] = ID; // numero proceso
			numbers[2] = randomNM(1, 20, 7919); // burst
			numbers[3] = randomNM(1, 5, 7919); // prioridad
			numbers[4] = 4; // quantum
			//numbers[5] = 5000; // tamaño

			printf("Proceso %i burst: %i prioridad: %i creado \n", numbers[1], numbers[2], numbers[3]);
			
			fflush(stdin);
			// envia el proceso al servidor
			write(descriptorCliente, numbers, sizeof (numbers));
 			close(descriptorCliente);
 			
			ID++;
			nn--;
			seed *= 3;
			sleep(1);
		
		}
	}
}
/**
 * Modo manual
 * Carga todos los procesos desde un archivo
 * dado por el usuario
 **/
int manualMode(){

	char route[256];

	do{
		printf("Ingrese el nombre del archivo: ");
		scanf("%s", route);
        fflush(stdin);

		printf("\nCargar archivo %s\n", route);

		FILE *fp;
		char buffer[255];
		
		fp = fopen(route, "r"); // tipo solo lectura
		
		int number;
		int counter = 1;
		int numbers[5];

		while (fscanf(fp, "%s", buffer) != EOF){
			number = atoi(buffer);
			//printf("->  %i  ", number);
			numbers[0] = 1 ; //tipo algoritmo
			numbers[counter] = number; // numero 

			numbers[4] = 0; // tamaño quantum si se requiere

			if (counter == 3)
			{
				//printf("\ncambio linea %i\n\n", counter);
				counter = 1 ;
/*
				for(int i = 0; i < 5; i++){

					printf(" -> %i", numbers[i]);

				}
				printf("\n");*/

				// envia el proceso al servidor

			}
			else
			{
				counter++;
			}
			
			sleep(1);

		}
		
		fclose(fp);
	}while(strcmp(route, "stop") != 0);
}
/***************************************************************************************************/
/**
 * Ayuda del sistema
 * Si se ejecuta esta función, entonces se detiene la ejecucion
 * Suele suceder por un error en los parámetros de entrada
 **/
int help()
{
	printf("comando no válido\n");
	return 0;
	exit(0);
	
}

/**
* Determina el modo del servidor
* Automático : -a
* Manual : -m
**/
char tipoServer(char *argv[]){
	char* tipo = argv[2];
	switch(tipo[1])
	{
		case 'a':
				printf("Modo Automático \n");
				// automático
				return 'a';
				break;
		case 'm':
				printf("Manual \n");
				// manual
				return 'm';
				break;
		default:
				// error
				help();
				return 'e';
	}
}

/**
 * Determina el tamaño del quantum a utilizar
 * -q [tamaño]
 *
 **/
int quantumSize(char *argv[])
{
	char* tipo = argv[3];
	switch(tipo[1])
	{
		case 'q':
				printf("quantum: %s  %i \n", tipo, atoi(argv[4]));
				return atoi(argv[4]);
				break;
		default:
				help();
				return 0;
	}
}

/* Verifica el tipo de algoritmo a ejecutar 
 * FIFO = -f
 * SFJ = -s
 * HPF = -h
 * RR = -r
 */
char tipoAlgoritmo(char *argv[])
{
	char* tipo = argv[1];

	switch(tipo[1])
	{ // algoritmo
		case 'f': 
				printf("FIFO : ");
				return 'f';
				break;
		case 's': 
				printf("SJF : ");
				return 's';
				break;
		case 'h': 
				printf("HPF : ");
				return 'h';
				break;
		case 'r': 
				printf("Round Robin : ");
				return 'r';
				break;
		default:					
				help();
				return ' ';
	}
}
 
//Función principal main encargada de ejcutar todas las funcionalidades del cliente 
int main(int argc, char *argv[])
{
	if ((argc == 3) || (argc == 5)){
		// ./cliente -f -a
		// ./cliente -r -a -q n
		char mode, tipo_algoritmo;
		int q_size = 0;
		
		tipo_algoritmo = tipoAlgoritmo(argv);

		mode = tipoServer(argv);

		if (tipo_algoritmo == 'r' && argc == 5){
			q_size = quantumSize(argv);
		}
		/*
		else
		{
			printf("Error, tamaño de quantum no especificado \n");
			help();
		}*/

		if(mode == 'a')
		{
			autoMode();
		}
		else if (mode == 'm')
		{
			manualMode();
		}
	}
	else{
		printf("Error\n");
	}
    exit(0);
}
