#include "funciones.h"
#include "definiciones_y_configuraciones.h"
#include <avr/io.h>

void DyC_inicialice_ADC()
{
	ADMUX = 0x00;
	ADCSRA = 0x00;

	ADMUX |= 0b00000000;  //ADMUX ADC0,  0b00000000
	ADMUX |= 0b11000000;  //REF 1.1V, 0b11000000
	ADCSRA |= 0b10101111; //preescalización de 128 (1 << ADEN)(1 << ADATE)(1 << ADIE)(1 << ADPS2)(1 << ADPS1)
	ADCSRB |= 0b00000011; // Trigger Timer/Counter0 Compare Match A
}

short leaADC(){
	short adcval= (ADCL | (ADCH << 8)); //Toma el valor del bus de datos ADC
	return adcval;
}

void DyC_Procese_ADC(int SALIDA, coef_iir_2_ord *ir, long *sal, float *saliir){
	short adcval= leaADC();                     //Lectura del ADC
	void DyC_Filtrar(SALIDA, adcval, &ir, &sal, &saliir);
}

void DyC_Filtrar(int SALIDA, short adcval, coef_iir_2_ord *ir, long *sal, float *saliir ){
    switch (SALIDA)
    {
      case FIR1:
        sal = filtrarFIR1((int)adcval);
        break;
      case FIR2:
        sal = filtrarFIR2((int)adcval);
        break;
      case FIR3:
        sal = filtrarFIR3((int)adcval);
        break;
      case IIR:
        saliir = filtrarIIR((float)adcval, &ir);
        break;
    }
}