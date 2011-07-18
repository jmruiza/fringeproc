/*
** demodula.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Tue Aug 29 16:02:51 2006 Julio Cesar Estrada Rico
** Last update Thu Oct 19 10:33:56 2006 Julio Cesar Estrada Rico
*/
/**
 * @file demodula.h
 * Declaraci�n de operaciones para la demodulaci�n de interferogramas.
 *
 * @defgroup Demodula Operaciones y m�todos para la demodulaci�n.
 * @ingroup fringeproc
 * Aqui est�n reunidos m�todos y operaciones para la demodulaci�n de im�genes
 * con patrones de franjas. 
 */


#ifndef   	DEMODULA_H_
# define   	DEMODULA_H_

#ifdef __cplusplus
extern "C" {
#endif

//@{
/**
 * @ingroup Demodula
 * Calcula el potencial de segundo orden y regresa el valor (@c float).
 * Un potencial de segundo orden es definido de la siguiente manera (caso
 * discreto:
 * @f[
 * V[f(x)]=[f(x-1)-2f(x)+f(x+1)]^2.
 * @f]
 * Esta funci�n realiza el c�lculo de este potencial, resiviendo un vector con
 * los tres valores que se requieren para calcular este potencial. 
 * @param cliq aqui van los tres valores para calcular el potencial. Los
 * valores deben de ir en orden. Se asume que la dimensi�n de este arreglo es
 * de 3. 
 * @return el valor del potencial calculado.
 */
float demod_potential_s2ord(float *restrict cliq);

/**
 * Calcula el potencial de segundo orden y regresa el valor (@c double).
 * Un potencial de segundo orden es definido de la siguiente manera (caso
 * discreto:
 * @f[
 * V[f(x)]=[f(x-1)-2f(x)+f(x+1)]^2.
 * @f]
 * Esta funci�n realiza el c�lculo de este potencial, resiviendo un vector con
 * los tres valores que se requieren para calcular este potencial. 
 * @param cliq aqui van los tres valores para calcular el potencial. Los
 * valores deben de ir en orden. Se asume que la dimensi�n de este arreglo es
 * de 3. 
 * @return el valor del potencial calculado.
 */
double demod_potential_d2ord(double *restrict cliq);

/**
 * Estima la frecuencia dado un valor de fase y un valor cosenoidal (@c float).
 * Esta funci�n es �til para estimar la frecuencia (derivada de la fase) cuando
 * conocemos solo un valor de fase en un punto y el valor cosenoidal en un
 * punto siguiente (o anterior). Aqui estamos suponiendo entoces que el valor
 * coseniodal viene dado de la siguiente manera:
 * @f[
 * g(x)=cos[\phi(x)],
 * @f]
 * donde @f$x@f$ es el punto y @f$\phi(x)@f$ es la fase. Entonces se 
 * estima la frecuencia dado los valores @f$ \phi(x) @f$ y @f$ g(x_+) @f$,
 * donde @f$ x_+ @f$ puede tomar los valores @f$ x+1 @f$ o @f$ x-1 @f$.
 * <p>
 * Esta funci�n calcula dos valores de frecuencias. Ambas frecuencias son
 * v�lidas en la vecindad del punto @f$ x @f$ ya que la funci�n coseniodal es
 * una funci�n par.
 *
 * @param g es el valor cosenoidal.
 * @param p es el valor de fase.
 * @param freqs aqu� se guardan los valores de frecuencia estimados. 
 * @param dir Indica si se estima la frecuencia hacia atr�s (valor -1) o hacia 
 * adelante (valor 1). 
 */
void demod_calc_sfrequencies(const float g, const float p, 
			     float *restrict freqs, const int dir);

/**
 * Estima la frecuencia dado un valor de fase y un valor cosenoidal (@c double).
 * Esta funci�n es �til para estimar la frecuencia (derivada de la fase) cuando
 * conocemos solo un valor de fase en un punto y el valor cosenoidal en un
 * punto siguiente (o anterior). Aqui estamos suponiendo entoces que el valor
 * coseniodal viene dado de la siguiente manera:
 * @f[
 * g(x)=cos[\phi(x)],
 * @f]
 * donde @f$ x @f$ es el punto y @f$ \phi(x) @f$ es la fase. Entonces se 
 * estima la frecuencia dado los valores @f$ \phi(x) @f$ y @f$ g(x_+) @f$,
 * donde @f$ x_+ @f$ puede tomar los valores @f$ x+1 @f$ o @f$ x-1 @f$.
 * <p>
 * Esta funci�n calcula dos valores de frecuencias. Ambas frecuencias son
 * v�lidas en la vecindad del punto @f$ x @f$ ya que la funci�n coseniodal es
 * una funci�n par.
 *
 * @param g es el valor cosenoidal.
 * @param p es el valor de fase.
 * @param freqs aqu� se guardan los valores de frecuencia estimados. 
 * @param dir Indica si se estima la frecuencia hacia atr�s (valor -1) o hacia 
 * adelante (valor 1). 
 */
void demod_calc_dfrequencies(const double g, const double p, 
			     double *restrict freqs, const int dir);

/**
 * Calcula el arcocoseno validando su valor (@c float).
 * Esta funci�n encapsula la funci�n @c acos de la librer�a estandar de "C".
 * Antes de calcular el arcocoseno primero valida que el argumento se encuentre
 * en el intervalo @f$[-1,1]@f$. Si el argumento queda fuera del intervalor, se
 * toma el l�mite del intervalo mas sercano al valor del argumento.
 *
 * @param g es el valor del argumento del arcocoseno a calcular.
 * @return 0 si el argumento es mayor o igual que cero, @f$\pi@f$ si el 
 * argumento es menor o igual que -1. En otro caso se devuelve el valor del
 * arcocoseno del argumento.
 */
float demod_calc_sacos(float g);

/**
 * Calcula el arcocoseno validando su valor (@c double).
 * Esta funci�n encapsula la funci�n @c acos de la librer�a estandar de "C".
 * Antes de calcular el arcocoseno primero valida que el argumento se encuentre
 * en el intervalo @f$[-1,1]@f$. Si el argumento queda fuera del intervalor, se
 * toma el l�mite del intervalo mas sercano al valor del argumento.
 *
 * @param g es el valor del argumento del arcocoseno a calcular.
 * @return 0 si el argumento es mayor o igual que cero, @f$\pi@f$ si el 
 * argumento es menor o igual que -1. En otro caso se devuelve el valor del
 * arcocoseno del argumento.
 */
double demod_calc_dacos(double g);

/**
 * Estima la fase inicial en la vecindad de la semilla dada (@c float).
 * Esta es una funci�n de utiler�a para ser usada en m�todos de demodulaci�n
 * como el Frequency Tracker (FT) que requieren una vecindad de 3 elementos (en
 * 1 dimenci�n) inicializada. Entonces, dado el valor de fase de la semilla 
 * inicial, se estima la fase en los vecinos adyacentes. Para esto, se utiliza 
 * un vector de 3 elementos, donde el elemento central tiene el valor de de fase
 * de la semilla inicial. Otro vector de 3 elementos contienen los valores
 * cosenoidales en esa vecindad, o en otras palabra, la informaci�n de donde se
 * estimara la fase. Una vez ejecutado este proceso, el vector que guarda la
 * semilla inicial contendra los valores de fase estimados en las posiciones
 * adyacentes de la semilla inicial.
 *
 * @param g son los valores cosenoidales de la vecindad de la semilla inicial.
 * Vector de 3 elementos.
 * @param p Vector de 3 elementos, donde el elemento central contiene el valor
 * de fase de la semilla inicial. En los elementos adyacentes se guardara la
 * fase estimada al rededor de la semilla inicial.
 */
void demod_set_sseed(float *restrict g, float *restrict p);

/**
 * Estima la fase inicial en la vecindad de la semilla dada (@c double).
 * Esta es una funci�n de utiler�a para ser usada en m�todos de demodulaci�n
 * como el Frequency Tracker (FT) que requieren una vecindad de 3 elementos (en
 * 1 dimenci�n) inicializada. Entonces, dado el valor de fase de la semilla 
 * inicial, se estima la fase en los vecinos adyacentes. Para esto, se utiliza 
 * un vector de 3 elementos, donde el elemento central tiene el valor de de fase
 * de la semilla inicial. Otro vector de 3 elementos contienen los valores
 * cosenoidales en esa vecindad, o en otras palabra, la informaci�n de donde se
 * estimara la fase. Una vez ejecutado este proceso, el vector que guarda la
 * semilla inicial contendra los valores de fase estimados en las posiciones
 * adyacentes de la semilla inicial.
 *
 * @param g son los valores cosenoidales de la vecindad de la semilla inicial.
 * Vector de 3 elementos.
 * @param p Vector de 3 elementos, donde el elemento central contiene el valor
 * de fase de la semilla inicial. En los elementos adyacentes se guardara la
 * fase estimada al rededor de la semilla inicial.
 */
void demod_set_dseed(double *restrict g, double *restrict p);

/**
 * Demodula un rengl�n dada la matriz de datos de un patr�n de franjas 
 * (@c float).
 * Demodula un rengl�n tomando un punto inicial dado. Esta demodulaci�n del
 * rengl�n se hace de acuerdo al m�todo Frequency Tracker. El punto inicial a
 * demodular es la semilla, la cual debe estar inicializada previamente (ver
 * demod_set_dseed(double* restrict,double* restrict)).
 *
 * @param p es la fase demodulada. Aqui se van guardando los valores
 * demodulados del rengl�n dado.
 * @param g es la matriz de datos del patr�n de franjas o interferograma.
 * @param row es el n�mero de regl�n a demodular
 * @param seed es el n�mero de elemento del rengl�n con el cual se inicial la
 * demodulaci�n.
 * @param N Dimenci�n del rengl�n.
 * @param nr n�mero de renglones del patr�n de franjas.
 */
void demod_demodula_srow(float* restrict p, float* restrict g, 
			 const int row, const int seed, const int N, 
			 const int nr);

/**
 * Demodula un rengl�n dada la matriz de datos de un patr�n de franjas 
 * (@c float).
 * Demodula un rengl�n tomando un punto inicial dado. Esta demodulaci�n del
 * rengl�n se hace de acuerdo al m�todo Frequency Tracker. El punto inicial a
 * demodular es la semilla, la cual debe estar inicializada previamente (ver
 * demod_set_dseed(double* restrict,double* restrict)).
 *
 * @param p es la fase demodulada. Aqui se van guardando los valores
 * demodulados del rengl�n dado.
 * @param g es la matriz de datos del patr�n de franjas o interferograma.
 * @param row es el n�mero de regl�n a demodular
 * @param seed es el n�mero de elemento del rengl�n con el cual se inicial la
 * demodulaci�n.
 * @param N Dimenci�n del rengl�n.
 * @param nr n�mero de renglones del patr�n de franjas.
 */
void demod_demodula_drow(double* restrict p, double* restrict g, 
			 const int row, const int seed, const int N, 
			 const int nr);

/**
 *Demodula el sub-renglon especificado en @a row y limitado por @a li
 *y @a ls, iniciando en el elemento @a seed dado.
 *@param p aqui se coloca la demodulación. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param row es el número de renglón a demodular.
 *@param seed es el sitio inicial donde comienza la demodulación. @a
 *seed ya debe estar inicializada antes con
 *demod_det_deed().
 *@param li limite inferior del subrenglon.
 *@param ls limite superior del subrenglon.
 *@param nr es el numero de renglones que tiene el interferograma.
 */
void demod_demodula_dsubrow(double* restrict p, double* restrict g, 
			    double* restrict s,
			    const int row, const int seed,
			    const int li, const int ls, 
			    const int nr);
/**
 *Demodula la columna especificada en @a column, iniciando en el elemento
 *@a seed dado.
 *@param p aqui se coloca la demodulación. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param column es el número de columna a demodular.
 *@param seed es el sitio inicial donde comienza la demodulación. @a
 *seed ya debe estar inicializada antes con
 *demod_set_seed(float*,float*).
 *@param N es la dimención de la columna.
 */
void demod_demodula_scolumn(float* restrict p, float* restrict g, 
			    const int column, const int seed, const int N);

/**
 *Demodula la columna especificada en @a column, iniciando en el elemento
 *@a seed dado.
 *@param p aqui se coloca la demodulación. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param column es el número de columna a demodular.
 *@param seed es el sitio inicial donde comienza la demodulación. @a
 *seed ya debe estar inicializada antes con
 *demod_det_deed(double*,double*).
 *@param N es la dimención de la columna.
 */
void demod_demodula_dcolumn(double* restrict p, double* restrict g, 
			    const int column, const int seed, const int N);

/**
 *Demodula la sub-columna especificada en @a column y limitado por @a li
 *y @a ls, iniciando en el elemento @a seed dado.
 *@param p aqui se coloca la demodulación. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param row es el número de renglón a demodular.
 *@param seed es el sitio inicial donde comienza la demodulación. @a
 *seed ya debe estar inicializada antes con
 *demod_det_deed().
 *@param li limite inferior del subrenglon.
 *@param ls limite superior del subrenglon.
 *@param nr numero de renglones de @a g
 */
void demod_demodula_dsubcolumn(double* restrict p, double* restrict g, 
			       double* restrict s,
			       const int column, const int seed, const int li,
			       const int ls, const int nr);

void demod_local_calc_dfreqx(double* restrict p, double* restrict s,
                             double* restrict fi, double* restrict psi,
                             double* wx, double* wy,
                             const int ir, const int ic,
                             const int nr, const int nc, const int N);

/**
 *Demodula un interferograma usando el método RPT.
 *@param p es la fase demodulada.
 *@param g es el interferograma.
 *@param lambda es el parámetro de regularización.
 *@param N es el tamaño de la vecindad que usa el RPT. la dimension es
 *N X N.
 *@param initc es la columna inicial
 *@param initr es el renglón inicial.
 *@note funcion no implementada
 */
void demod_drpt(double* restrict p, double* restrict g, const double lambda,
		const int N, const int initc, const int initr);
//@}
#ifdef __cplusplus
}
#endif

#endif 	    /* !DEMODULA_H_ */
