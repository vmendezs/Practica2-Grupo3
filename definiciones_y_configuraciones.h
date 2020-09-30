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
	void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas,int8_t *tempUnidadesD,int8_t *tempUnidadesC,int8_t *tempDecenasD,int8_t *tempDecenasC,int8_t *banderaADC);

	#ifdef __cplusplus
} // extern "C"
#endif



#endif