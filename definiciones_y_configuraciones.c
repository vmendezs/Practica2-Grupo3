#include "funciones.h"
#include "definiciones_y_configuraciones.h"
#include <avr/io.h>

//Coeficientes IIR PASA-BANDAS Fs=100 y corte en 25Hz (Entre 22Hz y 28Hz) @ 3dB
float num_Section1[3] = { 1, -0.000000000000000124672979561347551519246, 1}; //Numerador Seccion 1
float den_Section1[3] = {1, 0.2381868958, 0.7661067247}; //Denominador Seccion 1
float num_Section2[3] = { 1, -0.000000000000000124672979561347551519246, 1}; //Numerador Seccion 2
float den_Section2[3] = {1, -0.2381868958, 0.7661067247}; //Denominador Seccion 2
float w[3] = {0, 0, 0};// condición inicial punto intermedio,  por defecto para ejemplo {0, 0, 0}

// //Coeficientes IIR PASO-BAJOS y IIR PASO-ALTOS Fs=100 y corte en 10Hz @ 3dB
// float num_Section1[3] = { 1, 2, 1}; //Numerador Seccion 1
// float den_Section1[3] = {1, -1.320913434, 0.6327387691}; //Denominador Seccion 1
// float num_Section2[3] = { 1, 2, 1}; //Numerador Seccion 2
// float den_Section2[3] = {1, -1.048599601, 0.296140343}; //Denominador Seccion 2

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

void DyC_Procese_ADC(short SALIDA, coef_iir_2_ord *ir, long *sal, float *saliir2){
	short adcval= leaADC();                     //Lectura del ADC
	void DyC_Filtrar(SALIDA, adcval, &ir, &sal, &saliir2);
}

void DyC_Filtrar(short SALIDA, short adcval, coef_iir_2_ord *ir, long *sal, float *saliir2 ){
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
        inicializar_iir_2_ord(num_Section1, den_Section1, w, &ir);
        saliir1 = filtrarIIR((float)adcval, &ir);
        inicializar_iir_2_ord(num_Section2, den_Section2, w, &ir);
        saliir2 = filtrarIIR(saliir1, &ir);
        break;
    }
}