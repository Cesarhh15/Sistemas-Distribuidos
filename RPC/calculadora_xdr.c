/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calculadora.h"

bool_t
xdr_flotantes (XDR *xdrs, flotantes *objp)
{
	register int32_t *buf;

	 if (!xdr_float (xdrs, &objp->a))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->b))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_flotantes_trigonometricas (XDR *xdrs, flotantes_trigonometricas *objp)
{
	register int32_t *buf;

	 if (!xdr_float (xdrs, &objp->a))
		 return FALSE;
	return TRUE;
}
