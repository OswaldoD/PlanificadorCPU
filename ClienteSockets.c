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
 
void Cliente(int clienteSocketDescriptor)
{
  /*Se declara el buffer de datos a ser enviados al servidor*/
  char BufferHaciaServidor[256];
  
  //Se solicita el mensaje para el servidor
  printf("Igrese el mensaje apra el servidor: ");
  
  //Este ciclo se ejecuta mietras existan procesos para enviarle al servidor
   do {
 
        scanf("%s",BufferHaciaServidor);// se almacena el mensaje en el buffer
        fflush(stdin);
        
		/*Esta función se encarga de escribir datos a través del descriptor del socket*/
        write(clienteSocketDescriptor, BufferHaciaServidor, sizeof (BufferHaciaServidor));
 
      /*Se encarga de verificar que existan procesos para pasarle al descriptor*/
    } while (strcmp(BufferHaciaServidor, "salir") != 0);
    
    /*En caso de ya no existir procesos apra enviar al servidor se cierra el descriptor del socket*/
    close(clienteSocketDescriptor);
}
 
//Función principal main encargada de ejcutar todas las funcionalidades del cliente 
int main()
{
    system("clear");//llamada al sistema para una limpieza de la consola
    
    /*Se declara la estructura del cliente*/
    
    struct sockaddr_in serv_addr;
    
    /*Se asigan el tamaño de la estructura*/
    socklen_t addrLEN = sizeof (serv_addr);
    
    /*DSe declara el descriptor del cliente*/
    int descriptorCliente;
    descriptorCliente = ConfiguracionCliente();
    
    /*Llama y ejecuta la función del cliente pasando como parámentro el descriptor del socket del cliente*/
    Cliente(descriptorCliente);
    exit(0);
}
