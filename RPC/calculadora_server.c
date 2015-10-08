/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculadora.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

float *
suma_1_svc(flotantes *argp, struct svc_req *rqstp)
{
	static float  result;

		result = argp->a + argp->b;

	return &result;
}

float *
resta_1_svc(flotantes *argp, struct svc_req *rqstp)
{
	static float  result;

		result = argp->a - argp->b;
	
	return &result;
}

float *
multiplica_1_svc(flotantes *argp, struct svc_req *rqstp)
{
	static float  result;

		result = argp->a * argp->b;

	return &result;
}

float *
divide_1_svc(flotantes *argp, struct svc_req *rqstp)
{
	static float  result;

		result = argp->a / argp->b;

	return &result;
}

float *
seno_1_svc(flotantes_trigonometricas *argp, struct svc_req *rqstp)
{
	static float  result;

		result = sin(argp->a);

	return &result;
}

float *
coseno_1_svc(flotantes_trigonometricas *argp, struct svc_req *rqstp)
{
	static float  result;

		result = cos(argp->a);

	return &result;
}

float *
tangente_1_svc(flotantes_trigonometricas *argp, struct svc_req *rqstp)
{
	static float  result;

		result = tan(argp->a);

	return &result;
}