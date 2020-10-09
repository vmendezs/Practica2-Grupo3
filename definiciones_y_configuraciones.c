#include "display.h"
#include "definiciones_y_configuraciones.h"
#include <avr/io.h>

void DyC_inicialice_ADC(int8_t *banderaADC2)
{
	ADMUX = 0x00;
	ADCSRA = 0x00;

	ADMUX |= 0b00001000;  //ADMUX 8,  0b00001000
	ADMUX |= 0b11000000;  //REF 1.1V, 0b11000000
	ADCSRA |= 0b10001111; //preescalización de 128 (1 << ADPS2)(1 << ADPS1)(1 << ADEN)
	*banderaADC2= 0;       //inicializa banderaADC en 0
}

void DyC_empiece_ADC(int8_t *banderaADC2)
{
	ADCSRA |= 0b01000000; //inicia la conversión (1 << ADSC)
	*banderaADC2= 1;       //Levanta la banderaADC
}

short leaADC(){
	short adcval= (ADCL | (ADCH << 8)); //Toma el valor del bus de datos ADC
	return adcval;
}

int8_t convierta_a_Celsius(short adcval)
{
	int8_t tempVal;                        //Guarda valor temperatura en Celcius
	tempVal = (adcval - TEMPOFFSET)/1.22F; //Convierte el dato en celcius
	return tempVal;                        //Retorna el valor de temperatura en Celcius
}

int8_t saqueUnidades(int8_t tempVal)
{
	int8_t decenas= tempVal/10;              //Divide el valor de temperatura en 10 para hallar las decenas
	int8_t unidades=tempVal - (10*decenas);  //Se restan las decenas al valor de temperatura para hallar las unidades
	return unidades;                         //Retorna el valor de unidades
}

int8_t saqueDecenas(int8_t tempVal)
{
	int8_t decenas= tempVal/10;       //Divide el valor de temperatura en 10 para hallar las decenas
	return decenas;                   //Retorna el valor de decenas
}

void DyC_Procese_ADC(int8_t *temperatura,int8_t *tempUnidades,int8_t *tempDecenas,int8_t *tempUnidadesD,int8_t *tempUnidadesC,int8_t *tempDecenasD,int8_t *tempDecenasC,int8_t *banderaADC2)
{
	*banderaADC2=0;                             //Se apaga la banderaADC2
	short adcval= leaADC();                     //Lectura del ADC
	*temperatura=convierta_a_Celsius(adcval);   //Conversion del temperatura a Celcius
	*tempUnidades=saqueUnidades(*temperatura);  //Saca las unidades del valor de temperatura
	*tempDecenas=saqueDecenas(*temperatura);    //Saca las decenas del valor de temperatura
	*tempUnidadesD=D_num2portD(*tempUnidades);  //Saca el valor de unidades de la tabla de look up para el puerto D
	*tempUnidadesC=D_num2portC(*tempUnidades);  //Saca el valor de unidades de la tabla de look up para el puerto C
	*tempDecenasD=D_num2portD(*tempDecenas);    //Saca el valor de decenas de la tabla de look up para el puerto D
	*tempDecenasC=D_num2portC(*tempDecenas);    //Saca el valor de decenas de la tabla de look up para el puerto C
}

