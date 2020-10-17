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

void DyC_Procese_ADC(short SALIDA, coef_iir_2_ord *ir1, coef_iir_2_ord *ir2, long *sal, float *saliir2){
	short adcval= leaADC();                     //Lectura del ADC
	void DyC_Filtrar(SALIDA, adcval, &ir1, &ir2, &sal, &saliir2);
}

void DyC_Filtrar(short SALIDA, short adcval, coef_iir_2_ord *ir1, coef_iir_2_ord *ir2, long *sal, float *saliir2 ){
    float saliir1; 
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
        saliir1 = filtrarIIR((float)adcval, &ir1);
        saliir2 = filtrarIIR(saliir1, &ir2);
        break;
    }
}