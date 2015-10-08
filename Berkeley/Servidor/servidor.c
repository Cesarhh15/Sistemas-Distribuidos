// gcc servidor.c  -lpthread -o  servidor
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


 struct cliente_env
 {
  int t_cliente[2];
  char *ip;
 };

 struct ip_client
 {
  char *ip1;
  int puerto;
   
 }listaip[100];

struct clientes_hora
{
  int tiempo[2];
  char *ip;
  int puerto;
  struct clientes_hora *siguiente;
};

struct param{
  char *mensaje;
 char *ip;
 int puerto;
};

 void anadir_cliente(struct sockaddr_in addr,struct cliente_env hora_C);
 void err(char *str);
 void mostrar_clientes();
 void ejecutar_regreso(int duracion);
 void *regresa_cliente(struct param *h1);
 char * Berkeley();
 int * ObtenerHoraLocal();
 int diferencia(int *hr,int h1,int min1);
 int promedio(int res,int num);
 int tiempoE(int t);
    struct clientes_hora *primero, *ultimo;
    struct sockaddr_in my_addr, cli_addr;
    int sockfd; 
    socklen_t slen=sizeof(cli_addr);
    char buf[BUFLEN];
    int i,a,n,w=1;
    time_t t;
    struct tm *tmp;
    struct hostent *gethostbyname(); 


int main(int argc, char **argv)
{
     struct cliente_env tc;
     char horaaux[15];
     pid_t pid;
     primero = (struct clientes_hora *) NULL;
     ultimo = (struct clientes_hora *) NULL;
     

     if (argc!=2)
     {
        err(" argv[1]=tiempo en segundos para recibir a clientes");
     }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
      {
          err("socket");
      }
    
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT); 
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1)
      {
          err("bind");
      }
    else{

      printf("Servidor activo...\n");
    }
       
   /* pid=fork();
    if (pid==0)
    {
      int r;
      r=tiempoE(atoi(argv[1]));
      w=0;
    }  */
    
    while(w)
    {
       
     
       memset(buf, 0, BUFLEN);
       memset(&cli_addr,0,sizeof(cli_addr));
       char mensaje[15];
       if(n=recvfrom(sockfd, &tc, sizeof(tc), 0, (struct sockaddr*)&cli_addr, &slen)==-1){

       }else{
             sprintf(buf, "TIME");
             printf("%s\n",buf ); 
       }
       
       if (strncmp(buf, "TIME", 4)==0){
          
           system("clear");
           printf("\nSe ha conectado %s por su puerto %d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
           anadir_cliente(cli_addr,tc);
           mostrar_clientes(primero);
          
       }
       
        
       if(w==0){
        break;
       }   
       
     ejecutar_regreso(atoi(argv[1]));          
     }
    
      

    close(sockfd);
    return 0;
  

}
int tiempoE(int t){
  sleep(t);
  return 0;

}
void ejecutar_regreso(int duracion){

    
    struct clientes_hora *auxiliar1; /* lo usamos para recorrer la lista */
    struct param h1;
    struct ip_client ip2[100];
    pthread_t hilo;
    int k=0,*h,d;
    h=(int *)malloc(2*sizeof(int)); 
    char recDura[10];
    char *hora_env;
    hora_env=(char *)malloc(10*sizeof(char));
    sprintf(recDura, "sleep \"%d\"", duracion);
    system(recDura);
 //   d=duracion*1000000;
  //  fflush (stdout);
  // usleep(d);
    printf("\n\n¡¡¡Listo para enviar Hora con Berkeley!!!\n"); 
   strcpy(hora_env,Berkeley());
   printf("hora que se enviara %s\n",hora_env );
          auxiliar1 = primero;
          while (auxiliar1!=NULL) {
                      ip2[k].ip1= auxiliar1->ip;
                      ip2[k].puerto= auxiliar1->puerto;
                      auxiliar1 = auxiliar1->siguiente;
                      h1.ip=ip2[k].ip1;
                      h1.mensaje=hora_env;
                      h1.puerto=1234;
                      pthread_create (&hilo, NULL, (void*)regresa_cliente, (void*)&h1);
                     // regresa_cliente(hora_env,ip2[k].ip1 , 1234);
                      pthread_join(hilo, NULL);
                k++;
          }

}
 void anadir_cliente(struct sockaddr_in addr,struct cliente_env hora_C) {
      
      struct clientes_hora *nuevo;
      char *ipaux;
      ipaux=inet_ntoa(addr.sin_addr);
      /* reservamos memoria para el nuevo elemento */
      nuevo = (struct clientes_hora *) malloc (sizeof(struct clientes_hora));
      nuevo->ip = (char *) malloc (50 * sizeof (char));
      if (nuevo==NULL) printf( "No hay memoria disponible!\n");
      fflush(stdout);
      nuevo->tiempo[0]=hora_C.t_cliente[0];
      fflush(stdout);
      nuevo->tiempo[1]=hora_C.t_cliente[1];
      fflush(stdout);
      strcpy (nuevo->ip, ipaux);
      fflush(stdout); 
      nuevo->puerto=addr.sin_port;
      /* el campo siguiente va a ser NULL por ser el último elemento
         de la lista */
      nuevo->siguiente = NULL;
      /* ahora metemos el nuevo elemento en la lista. lo situamos
         al final de la lista */
      /* comprobamos si la lista está vacía. si primero==NULL es que no
         hay ningún elemento en la lista. también vale ultimo==NULL */
      if (primero==NULL) {
           printf( "Primer elemento\n");
           primero = nuevo;
           ultimo = nuevo;
         }
      else {
           /* el que hasta ahora era el último tiene que apuntar al nuevo */
           ultimo->siguiente = nuevo;
           /* hacemos que el nuevo sea ahora el último */
           ultimo = nuevo;
           }
      }
    

 void err(char *str)
{
    perror(str);
    exit(1);
}

void mostrar_clientes() {
      struct clientes_hora *auxiliar; /* lo usamos para recorrer la lista */
      int i=0;
      auxiliar = primero;
      printf("\nLista de dispositivos a Sincronizar:\n");
      while (auxiliar!=NULL) {
            printf( "IP: %s, Hora: %d, minutos: %d, puerto: %d\n",
                    auxiliar->ip,auxiliar->tiempo[0],auxiliar->tiempo[1],auxiliar->puerto);
            auxiliar = auxiliar->siguiente;
            i++;
      }
      if (i==0) printf( "\nLa lista está vacía!!\n" );
 }





 
void *regresa_cliente(struct param *h1) { 
 
  int n,s;  
  int longitud;
  struct hostent *maqInfo;
  struct sockaddr_in tuDirSocket;
  tuDirSocket.sin_family = AF_INET; 
  if ((maqInfo = gethostbyname(h1->ip)) == NULL) { 
      err("error host regresa_cliente"); 
  } 

  tuDirSocket.sin_port = htons(h1->puerto); 
  tuDirSocket.sin_addr = *((struct in_addr *)maqInfo->h_addr);  
   
  if (( s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 

      printf("fallo al abrir socket enviar\n" );
  } 
  printf("Hora actualizada se enviará a:  %s, %d\n", inet_ntoa(tuDirSocket.sin_addr), ntohs(tuDirSocket.sin_port));
  longitud = strlen(h1->mensaje);

  if ((n = sendto(s,h1->mensaje,longitud, 0, (struct sockaddr *) &tuDirSocket, sizeof(tuDirSocket))) < 0) {
      err("error sent regresa_cliente"); 
  }else{
      printf("se envio el mensaje %s al cliente \n",h1->mensaje );
       
  }
  close(s); 
} // mabraham.vazquez@gmail.com bionick.tronick@gmail.com

char * Berkeley(){
  char *hora;
  int horas,min,dif,*res,*res1,*hlocal,suma=0,promedio1,H,M,HoraF,MinF;
  hlocal=(int *)malloc(2*sizeof(int));
  hora=(char *)malloc(10*sizeof(char));
  res1=(int *)malloc(2*sizeof(int));
   res=(int *)malloc(100*sizeof(int));
  struct clientes_hora *auxiliar; /* lo usamos para recorrer la lista */
    
      int i=0;
      hlocal=ObtenerHoraLocal();
      auxiliar = primero;
      while (auxiliar!=NULL) {
           
          horas=auxiliar->tiempo[0];
           min= auxiliar->tiempo[1];
           printf("horas %d , min %d\n",horas,min );
           dif=diferencia(hlocal,horas,min);
           res[i]=dif;
           printf("diferencia %d\n", dif);
           suma+=res[i];
            auxiliar = auxiliar->siguiente;
            i++;
      }
      printf(" la suma %d\n",suma );
      promedio1=promedio(suma,i);
      printf("promedio %d valor de i:%d\n",promedio1,i);
      H=promedio1/60;
      M= promedio1 - H*60;
      if (promedio1>0)
      {
         if(M<60){
        HoraF=H+hlocal[0];
        MinF=M+hlocal[1];
          }
          if (M>=60)
          {
           HoraF=H+hlocal[0]+1;
           MinF=60-(M+hlocal[1]); 
          }
      }else{
        
        HoraF=H-hlocal[0];
        MinF=M-hlocal[1];
        if (MinF>0)
        {
          HoraF=H-hlocal[0];
          MinF=M-hlocal[1];
        
        }else{
          HoraF=hlocal[0]-1;
           MinF=60+MinF; 
          
        }
           
      }
     sprintf(hora,"%d:%d",HoraF,MinF);

  return hora;
}

int * ObtenerHoraLocal(){    
    char auxC[16];
    int *horas;
    horas=(int *)malloc(2*sizeof(int));             
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
    horas[0] = atoi(ptr);

   while( (ptr = strtok( NULL, " " )) != NULL ){    
     horas[i] = atoi(ptr);
     i++;
    } 
  return horas;
  
  }

  int diferencia(int *hr,int h1,int min1)
  {
    int n,k;
     if (hr[0]<h1)
       {
       
          k=h1-hr[0]; 
        
          if (k>=2)
          {
             n=(min1-hr[1])+(k*60);
          }
          else{
            
            n=(min1-hr[1])+60;
          }
         

       }else if(hr[0]==h1){
       
        if (min1>hr[1])
        {
          n=min1-hr[1];
        }else{
           n=hr[1]-min1;
           n=n*-1;
        }

       }
       else if(hr[0]>h1){
        
          k=hr[0]-h1; 
         
          if (k>=2)
          {
            n=(hr[1]-min1)+(k*60);
            n=n*-1;
          }else{
           
             n=(hr[1]-min1)+60;
             n=n*-1;
          }
         

       }  
       return n;
  }

 int promedio(int res,int num){
  int sal;
  sal=res/(num+1);
  return sal;

 } 