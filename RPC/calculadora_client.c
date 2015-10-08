/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora.h"


void
rpc_1(char *host)
{
	CLIENT *clnt;
	float  *result_1;
	flotantes  suma_1_arg;
	float  *result_2;
	flotantes  resta_1_arg;
	float  *result_3;
	flotantes  multiplica_1_arg;
	float  *result_4;
	flotantes  divide_1_arg;
	float  *result_5;
	flotantes_trigonometricas  seno_1_arg;
	float  *result_6;
	flotantes_trigonometricas  coseno_1_arg;
	float  *result_7;
	flotantes_trigonometricas  tangente_1_arg;
	int opc;
	float a,b;
	char c;

#ifndef	DEBUG
	clnt = clnt_create (host, rpc, rpcprueba, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	
	do{
	system("clear");
	printf("Escoge una operación\n");
	printf("1.-Suma(a+b)\n");
	printf("2.-Resta(a+b)\n");
	printf("3.-Multiplicación(a*b)\n");
	printf("4.-División(a/b)\n");
	printf("5.-Seno(a)\n");
	printf("6.-Coseno(a)\n");
	printf("7.-Tangente(a)\n");
	printf("8.-Salir\n\n");
	printf("Elige opción: \n");
	scanf("%d",&opc);
	switch(opc){
		case 1:
		printf("Opción Suma\n\n");
		printf("Ingresa primer digito\n");
		scanf("%f",&a);
		printf("Ingresa segundo digito\n");
		scanf("%f",&b);
		suma_1_arg.a=a;
		suma_1_arg.b=b;

		result_1 = suma_1(&suma_1_arg, clnt);
		printf("\n%.2f + %.2f = %.2f\n",a,b,*result_1 );
			if (result_1 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}

		break;

		case 2:
		printf("Opción Resta\n\n");
		printf("Ingresa primer digito\n");
		scanf("%f",&a);
		printf("Ingresa segundo digito\n");
		scanf("%f",&b);
		resta_1_arg.a=a;
		resta_1_arg.b=b;
		
		result_2 = resta_1(&resta_1_arg, clnt);
		printf("\n%.2f - %.2f = %.2f\n",a,b,*result_2 );
			if (result_2 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}

		break;

		case 3:
		printf("Opción Multiplicación\n\n");
		printf("Ingresa primer digito\n");
		scanf("%f",&a);
		printf("Ingresa segundo digito\n");
		scanf("%f",&b);
		multiplica_1_arg.a=a;
		multiplica_1_arg.b=b;


		result_3 = multiplica_1(&multiplica_1_arg, clnt);
		printf("\n%.2f * %.2f = %.2f\n",a,b,*result_3 );
			if (result_3 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}
		break;

		case 4:
		printf("Opción División\n\n");
		printf("Ingresa primer digito\n");
		scanf("%f",&a);
		printf("Ingresa segundo digito\n");
		scanf("%f",&b);
		divide_1_arg.a=a;
		divide_1_arg.b=b;

		result_4 = divide_1(&divide_1_arg, clnt);
		printf("\n%.2f / %.2f = %.2f\n",a,b,*result_4 );
			if (result_4 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}
		break;

		case 5:
		printf("Opción Seno\n\n");
		printf("Ingresa un digito\n");
		scanf("%f",&a);
		seno_1_arg.a=a;

		result_5 = seno_1(&seno_1_arg, clnt);
		printf("\n seno( %.2f ) = %.2f\n",a,*result_5 );
			if (result_5 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}
		break;

		case 6:
		printf("Opción Coseno\n\n");
		printf("Ingresa un digito\n");
		scanf("%f",&a);
		coseno_1_arg.a=a;

		result_6 = coseno_1(&coseno_1_arg, clnt);
		printf("\n coseno( %.2f ) = %.2f\n",a,*result_6 );
			if (result_6 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}
		break;

		case 7:
		printf("Opción Tangente\n\n");
		printf("Ingresa un digito\n");
		scanf("%f",&a);
		tangente_1_arg.a=a;

		result_7 = tangente_1(&tangente_1_arg, clnt);
		printf("\n tangente( %.2f ) = %.2f\n",a,*result_7 );
			if (result_7 == (float *) NULL) {
				clnt_perror (clnt, "call failed");
			}
		break;

		case 8:
		printf("Fin de la calculadora\n");
		exit(1);
		break;
		default:
		printf("No se encuentra esa opción\n");
		break;

	}
	getchar();
	printf("Oprime 's' para realizar otra operación\n");
	scanf("%c",&c);
}while(c=='s');

	
	
	
	
	
	
	
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	rpc_1 (host);
exit (0);
}
