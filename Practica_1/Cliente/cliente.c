#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <netdb.h>

#define BUFLEN 512
#define PORT 1235
void error(int code, char *err);
 struct sockaddr_in serv_addr;
    struct hostent *he;
    int sockfd;
    unsigned int slen=sizeof(serv_addr);
    int n,i,s,infoRecibida,conect;
    char buf[BUFLEN];

int main(int argc, char** argv)
{
    if ((he=gethostbyname(argv[1])) == NULL)
  {  
    error(1, "Ingrese un host");
  }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1){
    	 error(2, "no puedo inicializar el socket");
       
    }
	memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    //serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_addr=*((struct in_addr *)he->h_addr);
	
   if (conect=connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) 
  {
     error(3, "error al conectar");
  }else{
  	printf("se conecto\n");
  }

    while(1)
    {
    	memset(buf, 0, BUFLEN);
		printf("1.-Escribe TIME para actualizar a la hora del servidor\n");
		printf("2.-Escribe EXIT para cerrar la conexion actual\n");
		printf("3.-Escribe CERRAR para cerrar al servidor\n");
        scanf("%s",buf);
        getchar();
        char fecha[40];
        printf("\n");
        if(strcmp(buf,"EXIT") == 0){
        	
        	       if((n=sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))==-1)
					   		{
						    error(4, "Error al enviar paquete al servidor");
						   }else{
						   	printf("se envio EXIT\n");
						   }
				   if((n=recvfrom(sockfd,buf,BUFLEN,0,(struct sockaddr*)&serv_addr,&slen))==-1)
						   {
							   error(5, "Error al recibir paquete del servidor");
							}else{
								printf("Mensaje del servidor %s\n", buf);
							system("clear");
							exit(0);

							}
        }
          else if(strcmp(buf,"TIME") == 0){
			       if((n=sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))==-1)
					   		{
						    error(4, "Error al enviar paquete al servidor");
						   }else{
						   	printf("se envio paquete\n");
						   }
				   if((n=recvfrom(sockfd,buf,BUFLEN,0,(struct sockaddr*)&serv_addr,&slen))==-1)
						   {
							   error(5, "Error al recibir paquete del servidor");
							}else{
								printf("Mensaje del servidor %s\n", buf);
								sprintf(fecha, "sudo date --set \"%s\"", buf);
								if (system(fecha))
								{
									printf("fecha cambiada a : %s\n", buf);
								}

								
								
								
							}

  
       }
       else if(strcmp(buf,"CERRAR") == 0){
       	 if((n=sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))==-1)
					   		{
						    error(4, "Error al enviar paquete al servidor");
						   }else{
						   	printf("se envio CERRAR\n");
						   }
				   if((n=recvfrom(sockfd,buf,BUFLEN,0,(struct sockaddr*)&serv_addr,&slen))==-1)
						   {
							   error(5, "Error al recibir paquete del servidor");
							}else{
								printf("Mensaje del servidor %s\n", buf);
								system("clear");
							exit(0);

							}

       }
       else{
       	 if((n=sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))==-1)
					   		{
						    error(4, "Error al enviar paquete al servidor");
						   }else{
						   	printf("se envio Texto\n");
						   }
				   if((n=recvfrom(sockfd,buf,BUFLEN,0,(struct sockaddr*)&serv_addr,&slen))==-1)
						   {
							   error(5, "Error al recibir paquete del servidor");
							}else{
								printf("Mensaje del servidor %s\n", buf);
							

							}

       }
   }

    close(sockfd);
    return 0;
}



void error(int code, char *err)
{
  char *msg=(char*)malloc(strlen(err)+14);
  sprintf(msg, "Error %d: %s\n", code, err);
  fprintf(stderr, msg);
  exit(1);
}