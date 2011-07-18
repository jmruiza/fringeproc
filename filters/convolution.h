/*
** convolution.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Mon Aug 21 15:19:14 2006 Julio Cesar Estrada Rico
** Last update Sun Sep  3 11:58:13 2006 Julio Cesar Estrada Rico
*/

/**
 *@file convolution.h
 *Definici�n de m�todos para hacer convoluci�n entre dos se�ales.
 */

#ifndef   	CONVOLUTION_H_
# define   	CONVOLUTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//@{
/**
 *@ingroup fringeproc
 *Realiza una convoluci�n con un kernel separable (@c float). La convoluci�n
 * que se 
 *realiza esta normalizada. Significa que se divide entre la suma acumulativa
 *del kernel de convoluci�n.
 *<p>
 *Un kernel separable es aquel que puede ser descompuesto como funciones de
 *una sola variable. Por ejemplo, sea @f$g(x,y)@f$ la funci�n del kernel. Si
 *@f$g(x,y)@f$ es separable, entonces lo podemos escribir de la siguiente
 *manera: @f$g(x,y)=g_1(x)g_2(y)@f$. Esta propiedad, permite escribir un
 *algoritmo de convoluci�n r�pido.
 *</p>
 *@param f es el resultado de la convoluci�n.
 *@param g es la se�al a convolver.
 *@param hx es el kernel de convoluci�n en direcci�n x, i.e @f$g_1(x)@f$.
 *@param hy es el kernel de convoluci�n en direcci�n y, i.e @f$g_2(y)@f$.
 *@param nr es el n�mero de renglones.
 *@param nc es el n�mero de columnas.
 *@param kr es la dimenci�n del kernel en direcci�n x
 *@param kc es la dimenci�n del kernel en direcci�n y
 */
void convolution_snormalized(float *restrict f, float *restrict g, 
			     float *restrict hx, float *restrict hy,
			     const int nr, const int nc, 
			     const int kr, const int kc);
/**
 *Realiza una convoluci�n con un kernel separable (@c double). La convoluci�n
 *que se 
 *realiza esta normalizada. Significa que se divide entre la suma acumulativa
 *del kernel de convoluci�n.
 *<p>
 *Un kernel separable es aquel que puede ser descompuesto como funciones de
 *una sola variable. Por ejemplo, sea @f$g(x,y)@f$ la funci�n del kernel. Si
 *@f$g(x,y)@f$ es separable, entonces lo podemos escribir de la siguiente
 *manera: @f$g(x,y)=g_1(x)g_2(y)@f$. Esta propiedad, permite escribir un
 *algoritmo de convoluci�n r�pido.
 *</p>
 *@param f es el resultado de la convoluci�n.
 *@param g es la se�al a convolver.
 *@param hx es el kernel de convoluci�n en direcci�n x, i.e @f$g_1(x)@f$.
 *@param hy es el kernel de convoluci�n en direcci�n y, i.e @f$g_2(y)@f$.
 *@param nr es el n�mero de renglones.
 *@param nc es el n�mero de columnas.
 *@param kr es la dimenci�n del kernel en direcci�n x
 *@param kc es la dimenci�n del kernel en direcci�n y
 */
void convolution_dnormalized(double *restrict f, double *restrict g, 
			     double *restrict hx, double *restrict hy,
			     const int nr, const int nc, 
			     const int kr, const int kc);
           
//@}
#ifdef __cplusplus
}
#endif

#endif 	    /* !CONVOLUTION_H_ */
