#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <netdb.h>

#define BUFLEN 512
#define PORT 1235
#define PORTR 1234
#define TAM 20000


void error(int e){
 if(e < 0){                                                //Si el valor arrojado por las funciones anteriores es menor que 0 imprime un mensaje de error
   printf("Lo siento ha ocurrido un error\n");
   exit (1);   
}  
}

 struct cliente_env
 {
  int t_cliente[2];
  char ip[16];
   
 };
 struct cliente_env ObtenerHoraLocal();
 void recibe_hora(int puerto);
    struct sockaddr_in serv_addr,serv_addrhora,clie_addrhora;
    struct hostent *he;
    int sockfd;
    unsigned int slen=sizeof(serv_addr);
    int n,i,s,infoRecibida;
    char buf[BUFLEN];
    char recvline[BUFLEN];
    char buffer[TAM];
    char fecha[40];

int main(int argc, char** argv)
{
  struct cliente_env t1;

    if (argc!=2)
     {
         perror("Ingrese argv[1]=IP del servidor");
          exit(1);
     }
    if ((he=gethostbyname(argv[1])) == NULL)
  {  
    perror("gethostbyname");
    exit(1);
  }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
        error(sockfd);

      	memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        //serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
      	serv_addr.sin_addr=*((struct in_addr *)he->h_addr);
         
         t1 = ObtenerHoraLocal();
         
         printf("Hora del sistema %d:%d hrs\n ",t1.t_cliente[0],t1.t_cliente[1]);
                 if((n=sendto(sockfd, &t1, sizeof(t1), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))==-1)
                   {
                    printf("error en el sent\n");
                   }
                  
           recibe_hora(PORTR);

                   
          	
      

    close(sockfd);
    return 0;
}

struct cliente_env ObtenerHoraLocal(){    
    struct cliente_env act;
    char auxC[16];              
    char *ptr;                       
    int i;
    time_t t;
    struct tm *tmp;              
    t = time(NULL);
    tmp = localtime(&t);
    char output[128];
    strftime(output,128,"%H %M",tmp);
    i=1;
    ptr = strtok( output, " " );
    act.t_cliente[0] = atoi(ptr);

   while( (ptr = strtok( NULL, " " )) != NULL ){    
     act.t_cliente[i] = atoi(ptr);
     i++;
    } 
  return act;
  
  }

 
void recibe_hora(int puerto){
  int s;
  unsigned int slen1=sizeof(clie_addrhora);
  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
      printf("Error en el socket de recibe hora\n");
    return; 
  } 

  bzero(&serv_addrhora, sizeof(serv_addrhora));
  serv_addrhora.sin_family = AF_INET; 
  serv_addrhora.sin_port = htons(puerto); 
  serv_addrhora.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(s, (struct sockaddr *) &serv_addrhora, sizeof(serv_addrhora))!= 0) { 
    printf("Error al escuchar\n");
    close(s);  
  } 
 while(1){
        if((n=recvfrom(s,recvline,sizeof(recvline),0,(struct sockaddr*)&clie_addrhora,&slen1))==-1)
                   {
                    error(n);
                     }else{
                              system("clear");
                                printf("Mensaje del servidor %s\n", recvline);
                                sprintf(fecha, "sudo date --set \"%s\"", recvline);
                                if (system(fecha))
                                {
                                    printf("fecha cambiada a : %s\n", recvline);
                                }
                                //close(s);
                            }
}
close(s);
}