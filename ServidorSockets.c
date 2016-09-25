#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <pthread.h>
#include "PCB.h"

int tipoAlgoritmo;
PCB datosPCB;

pthread_mutex_t obtenerTipoAlgoritmo;
pthread_mutex_t ejecutarAlgoritmo;

/*La función servidor es la encargada de comunicar los sockets del servidor y los hilos de los procesos.*/
 
void* Servidor(void* arg)
{	
    /*Delcaración del buffer de entrada (se encargará de almacenar el buffer de entrada del cliente)*/
    int BufferCliente[5];    
    
    /*Se declara el puntero del socket de etrada*/
    int sockEntrada = *(int *) arg;
    
    pthread_mutex_lock(&obtenerTipoAlgoritmo);
    
    /*Ciclo infinito que se encargará de estar a la espera de los mensajes del cliente"*/
    printf("Esperando los mensajes... ");
    for (;;)
    {
        /*Función encargada de leer el mensaje o los datos de la conexión del cliente*/
        read(sockEntrada, BufferCliente, sizeof (BufferCliente));         
                      
        
        //Ciclo encargado de verificar el momento en el que termina un hilo del mensaje
        /*Si el buffer es igual a salir se procede a mostrar al usuario el mensjae que contiene el hilo del socket*/
         //printf("%d\n",BufferCliente[1]); 
        
        //aquí se debe indentificar el algoritmo que viene en la primera posicion y lo guarda en la variable global 
        //Procesar los datos recibidos
        tipoAlgoritmo = BufferCliente[0];        
        datosPCB.PID = BufferCliente[1];        
        datosPCB.burst = BufferCliente[2];
        datosPCB.prioridad = BufferCliente[3];
        //falta contemplar el tamaño la posicion 4 del buffer
        

         usleep(1000000);            
    }
         /*terminar el descriptor del socket*/
         close(sockEntrada);

		pthread_mutex_unlock(&obtenerTipoAlgoritmo);                 
		
         /*Se cierra el hilo del socket*/
         pthread_exit((void*) 0);    
}

/*Mediante este método se realiza toda la configuración de la creación del socket*/ 
int ConfiguracionServidor()
{
    /*Se define el descriptor del socket*/
    int SocketDescriptor;
    
    /*Se declara la estructura del socket*/
    struct sockaddr_in serverAddr;
    
    /*Se crea el socket con el siguiente protocolo en sus parámetros de entrada
     * PF_INET = especifica la familia del protocolo (en este caso el protocolo TCP/IP)
     * SOCK_STREAM = indica el tipo de servicio que se está utilizando
     * IPPROTO_TCP = especifica el protocolo, en este caso particular el que defina por defecto la familia que se eligió
     * */
    if ((SocketDescriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)//este if valida que   exista u socket en caso contrario le informa del error al usuario 
    {
      printf("Error en el socket\n");
      exit(1);
    }
    
    /*Esta función acpeta las conexiones, pide y acepta las siguiente conexiones de clientes al sistema operativo*/
    //Se le asocia una dirección al socket, se encargará de avisar cada vez que reciba una conexión del cliente
    memset(&serverAddr, 0, sizeof (serverAddr));
    
    /*Se asigna la familia*/
    serverAddr.sin_family = AF_INET;
    
    /*Se asigna la dirección específica de la estructura al descriptor del socket*/
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);//dirección IP específica
    
    /*Se define el puerto a utilizar*/
    serverAddr.sin_port = htons(6881);
    
    /*bind se encargará de asociar una dirección al socket, devuelve un 0 en caso de éxito, un -1 en caso de un error*/
    /*Se verifica que asocia una dirección al socket mediante el prototipo bind 
     * con los siguientes parámetros(descriptor, la dirección específica y el tamaño de la estructura)
     * en caso de no ser así se indica un mensaje de error en el socket para el usuario*/
    if (bind(SocketDescriptor, (struct sockaddr *) & serverAddr, sizeof (serverAddr)) < 0)
    {
      printf("Error en el socket\n");
      exit(1);
     }
     
    /*Esta función se encarga de atender las conexiones, avisa alsistema operativo que comience a atender una conexion de red
     * si llega conexiones de clientes muy rápido de su capacidad para atender, las asigna a una cola.*/
    if (listen(SocketDescriptor, 5) < 0)
    {
      printf("Error en el socket\n");
      exit(1);
    }
    
    return SocketDescriptor;//se devuelve el socket
}

void* CPUScheduler(){
	pthread_mutex_lock(&ejecutarAlgoritmo);
	pthread_mutex_lock(&obtenerTipoAlgoritmo);
	pthread_mutex_unlock(&ejecutarAlgoritmo);
	printf("%d\n",tipoAlgoritmo);
	switch(tipoAlgoritmo)
	{
			case 1: //FIFO
				//llamar al algoritmo FIFO
				printf("FIFO\n");
				break;
			case 2: //SJF
				//llamar al algoritmo SJF
				printf("SJF\n");
				break;
			case 3: //HPF
				//llamar al algoritmo HPF
				printf("HPF\n");
				break;
			case 4: //RR
				//llamar al algoritmo RR
				printf("RR\n");
				break;
	}
	pthread_mutex_lock(&ejecutarAlgoritmo);
	usleep(1000000);
	printf("ID %d\n",datosPCB.PID);
	printf("BURST %d\n",datosPCB.burst);
	printf("PRIORIDAD %d\n",datosPCB.prioridad);
	pthread_mutex_unlock(&obtenerTipoAlgoritmo);
	pthread_mutex_unlock(&ejecutarAlgoritmo);
}

/*Función principal main encargada de ejecutar todas als funcionalidades del servidor*/ 
int main()
{
    system("clear");//llamada al sistema para una limpieza de la consola
    
    /*Declara la estructura*/
    struct sockaddr_in serverAddr;
    
    /*Se devuelve la función y el descriptor*/
    int SocketDescriptor = ConfiguracionServidor();
 
    /*Inicia el cilco infinito para los sockets e hilos"*/
    while (1)
    {
        int clienteSocketDescriptor;//se define la variable tipo int para el descriptor del socket del cliente
        
        struct sockaddr_in clienteAddr;
        
        /*Se asigna el tamaño de la estructura*/
        unsigned int clienteLEN;
        clienteLEN = sizeof (clienteAddr);
        
        /*se declaran los hilos*/
		pthread_t thread_JOB_SCHEDULER;
		pthread_t thread_CPU_SCHEDULER;
		
		/*Se declaran los mutex para la concurrencia del semáforo*/
		pthread_mutex_init(&obtenerTipoAlgoritmo, NULL);
		pthread_mutex_init(&ejecutarAlgoritmo, NULL);
		
    
		/*Este if analiza el resultado de la conexión del cliente, en casod e fracaso muestra el mensaje al cliente*/
        if ((clienteSocketDescriptor = accept(SocketDescriptor, (struct sockaddr *) & clienteAddr, &clienteLEN)) < 0)
        {
			printf("Error en el socket\n");
			exit(1); //sale de la ejecución
		}
		
        /*Este if analiza el resultado de la inicialización del hilo, en caso de fracaso muestra el mensaje al cliente */
        if (pthread_create(&thread_JOB_SCHEDULER, NULL, Servidor, &clienteSocketDescriptor) != 0)
		{
            printf("Error en el hilo\n");
            exit(1);//sale de la ejecución
		}
		
		pthread_create(&thread_CPU_SCHEDULER, NULL, CPUScheduler, "Iniciar");

		
		pthread_join(thread_JOB_SCHEDULER,NULL);
		pthread_join(thread_CPU_SCHEDULER,NULL);
	
		pthread_mutex_destroy(&obtenerTipoAlgoritmo);
		pthread_mutex_destroy(&ejecutarAlgoritmo);
        
    }
    exit(0);
}
