#include "display.h"
#include "definiciones_y_configuraciones.h"
#include <avr/io.h>

void DyC_inicialice_ADC(int8_t *banderaADC)
{
	ADMUX = 0x00;
	ADCSRA = 0x00;

	ADMUX |= 0b00000000;  //ADMUX ADC0,  0b00000000
	ADMUX |= 0b11000000;  //REF 1.1V, 0b11000000
	ADCSRA |= 0b10101111; //preescalización de 128 (1 << ADEN)(1 << ADATE)(1 << ADIF)(1 << ADPS2)(1 << ADPS1)(1 << ADEN)
	ADCSRB |= 0b00000011; // Trigger Timer/Counter0 Compare Match A
	*banderaADC= 0;       //inicializa banderaADC en 0
}

void DyC_empiece_ADC(int8_t *banderaADC)
{
	ADCSRA |= 0b01000000; //inicia la conversión (1 << ADSC)
	*banderaADC= 1;       //Levanta la banderaADC
}

short leaADC(){
	short adcval= (ADCL | (ADCH << 8)); //Toma el valor del bus de datos ADC
	return adcval;
}

void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas,int8_t *tempUnidadesD,int8_t *tempUnidadesC,int8_t *tempDecenasD,int8_t *tempDecenasC,int8_t *banderaADC)
{
	*banderaADC=0;                              //Se apaga la banderaADC
	short adcval= leaADC();                     //Lectura del ADC
}

