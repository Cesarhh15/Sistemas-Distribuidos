#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

/** Puerto  */
#define PORT 1235 

/** Número máximo de hijos */
#define MAX_CHILDS 3

/** Longitud del buffer  */
#define BUFFERSIZE 512

int escuchaCliente(int socket, struct sockaddr_in addr);
int esperaClientes(int socket, struct sockaddr_in addr);
void error(int code, char *err);
void temporizador(int loop);

int main(int argv, char** argc){

    int socket_host;
    struct sockaddr_in client_addr;
    struct sockaddr_in my_addr;
    struct timeval tv;      /* Para el timeout del accept */
    socklen_t size_addr = 0;
    int socket_client;
    fd_set rfds;        /* Conjunto de descriptores a vigilar */
    int childcount=0;
    int exitcode;

    int childpid;
    int pidstatus;

    int activated=1;
    int loop=0;
    socket_host = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_host == -1){

      error(1, "No puedo inicializar el socket");
    }
    
    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY ;

    
    if( bind( socket_host, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 )
      error(2, "El puerto está en uso"); /* Error al hacer el bind() */

    if(listen( socket_host, 10) == -1 )
      error(3, "No puedo escuchar en el puerto especificado");

    size_addr = sizeof(struct sockaddr_in);


    while(activated)
      {
    temporizador(loop);
    /* select() se carga el valor de rfds */
    FD_ZERO(&rfds);
    FD_SET(socket_host, &rfds);

    /* select() se carga el valor de tv */
    tv.tv_sec = 0;
    tv.tv_usec = 500000;    /* Tiempo de espera */
    if (select(socket_host+1, &rfds, NULL, NULL, &tv))
      {
        
        if((socket_client = accept( socket_host, (struct sockaddr*)&client_addr, &size_addr))!= -1)
          {
        loop=-1;        /* Para reiniciar el mensaje de Esperando conexión... */
        printf("\nSe ha conectado %s por su puerto %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        switch ( childpid=fork() )
          {
          case -1:  /* Error */
            error(4, "No se puede crear el proceso hijo");
            break;
          case 0:   /* Somos proceso hijo */
            if (childcount<MAX_CHILDS)
              exitcode=escuchaCliente(socket_client, client_addr);
            else
              exitcode=esperaClientes(socket_client, client_addr);

            exit(exitcode); /* Código de salida */
          default:  /* Somos proceso padre */
            childcount++; /* Acabamos de tener un hijo */
            close(socket_client); /* Nuestro hijo se las apaña con el cliente que 
                         entró, para nosotros ya no existe. */
            break;
          }
          }
        else
          fprintf(stderr, "ERROR AL ACEPTAR LA CONEXIÓN\n");
      }

    /* Miramos si se ha cerrado algún hijo últimamente */
    childpid=waitpid(0, &pidstatus, WNOHANG);
    if (childpid>0)
      {
        childcount--;   /* Se acaba de morir un hijo */

        /* Muchas veces nos dará 0 si no se ha muerto ningún hijo, o -1 si no tenemos hijos
         con errno=10 (No child process). Así nos quitamos esos mensajes*/

        if (WIFEXITED(pidstatus))
          {

        /* Tal vez querremos mirar algo cuando se ha cerrado un hijo correctamente */
        if (WEXITSTATUS(pidstatus)==99)
          {
            printf("\nSe ha pedido el cierre del programa\n");
            activated=0;
          }
          }
      }
    loop++;
    }

    close(socket_host);

    return 0;
}

    /* No usamos addr, pero lo dejamos para el futuro */
int esperaClientes(int socket, struct sockaddr_in addr)
{
    char buffer[BUFFERSIZE];
    int bytecount;

    memset(buffer, 0, BUFFERSIZE);
    
    sprintf(buffer, "Demasiados clientes conectados. Por favor, espere unos minutos\n");

    if( sendto(socket,buffer,strlen(buffer),0, (struct sockaddr*)&addr, sizeof(addr))){
          error(6, "No puedo enviar información");
     }
    
    close(socket);

    return 0;
}

int escuchaCliente(int socket, struct sockaddr_in addr)
{

    char buffer[BUFFERSIZE];
    char aux[BUFFERSIZE];
    int bytecount;
    int fin=0;
    int code=0;         /* Código de salida por defecto */
    time_t t;
    struct tm *tmp;
    socklen_t slen=sizeof(addr);
    while (!fin)
      {

    memset(buffer, 0, BUFFERSIZE);


       if((bytecount=recvfrom(socket,buffer,BUFFERSIZE,0,(struct sockaddr*)&addr,&slen))==-1)
       {
       error(5, "No puedo recibir información");
         }
printf("%s\n",buffer);
    /* Evaluamos los comandos */
    /* El sistema de gestión de comandos es muy rudimentario, pero nos vale */
    /* Comando TIME - Da la hora */
    if (strncmp(buffer, "TIME", 4)==0)
      {
        memset(buffer, 0, BUFFERSIZE);

        t = time(NULL);
        tmp = localtime(&t);
        strftime(buffer, BUFFERSIZE, "%Y-%m-%d %H:%M:%S", tmp);
      }
    
    /* Comando EXIT - Cierra la conexión actual */
    else if (strncmp(buffer, "EXIT", 4)==0)
      {
        memset(buffer, 0, BUFFERSIZE);
        sprintf(buffer, "Hasta luego. Vuelve pronto %s\n", inet_ntoa(addr.sin_addr));
        fin=1;
      }
    /* Comando CERRAR - Cierra el servidor */
    else if (strncmp(buffer, "CERRAR", 6)==0)
      {
        memset(buffer, 0, BUFFERSIZE);
        sprintf(buffer, "Adiós. Cierro el servidor\n");
        fin=1;
        code=99;        /* Salir del programa */
      }
    else
      {     
        sprintf(aux, "ECHO: %s\n", buffer);
        strcpy(buffer, aux);
      }

     if(bytecount = sendto(socket,buffer,strlen(buffer),0, (struct sockaddr*)&addr, sizeof(addr))==-1){
          error(6, "No puedo enviar información");
     }
}
 
    close(socket);
    return code;
}

void temporizador(int loop)
{
  if (loop==0)
    printf("[SERVIDOR] Esperando al cliente ");

  printf("\033[1D");        /* Introducimos código ANSI para retroceder 2 caracteres */
  switch (loop%4)
    {
    case 0: printf("|"); break;
    case 1: printf("/"); break;
    case 2: printf("-"); break;
    case 3: printf("\\"); break;
    default:            /* No debemos estar aquí */
      break;
    }

  fflush(stdout);       /* Actualizamos la pantalla */
}

void error(int code, char *err)
{
  char *msg=(char*)malloc(strlen(err)+14);
  sprintf(msg, "Error %d: %s\n", code, err);
  fprintf(stderr, msg);
  exit(1);
}