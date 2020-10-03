#if !defined(DEFYCONF_H)
#define DEFYCONF_H
#include <avr/io.h>

#define TIEMPOADC 1000  //Tiempo en el que se procesa el ADC
#define TIEMPODISPLAY 2 //Tiempo en el que se prcesa el display
#define TEMPOFFSET 337  //Offset del valor de temperatura se debe ajustar antes de la ejecución
#define VREFF 1.22      //El valor de voltaje de referencia sobre el canal es de 1.22V


#ifdef __cplusplus
extern "C" {
	#endif
	//aqui ponemos nuestras funciones de C varias
	/*suponemos que short es un entero de 16 bit con signo*/
	void DyC_inicialice_ADC(void);
	short leaADC(void);
	void DyC_Procese_ADC(short SALIDA, coef_iir_2_ord *ir1, coef_iir_2_ord *ir2, long *sal, float *saliir2);
	void DyC_Filtrar(short SALIDA, short adcval, coef_iir_2_ord *ir1, coef_iir_2_ord *ir2, long *sal, float *saliir2 );
	#ifdef __cplusplus
} // extern "C"
#endif



#endif