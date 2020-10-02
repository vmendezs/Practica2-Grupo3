#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "funciones.h"
#include "nuestrostimers.h"
#include "definiciones_y_configuraciones.h"
#include "serialutility.h"
#include "Timers.h"

uint8_t banderaSerial=0;
uint8_t banderaMili=0;
uint8_t banderaADC=0;
// En nuestra implementacion esta deberia ser un global 
// si vamos a trabajar por interrupción o deberia estar en el
// espacio de memoria del main.

short SALIDA FIR1 // Seleccione FIR1,FIR2,FIR3, o IIR ver funciones.h

/*****Variables globales del filtro IIR*****/
coef_iir_2_ord ir; /*esta estructura debe ser global y se va a pasar por referencia a la funcion que la inicializa*/

long sal;
float saliir2;

void main (void)
{
    Tm_Periodico sondeoADC,sondeoDisplay;
    // suponemos que int8_t es un typedef de char entero con sigo de 8 bits
    uint16_t ubrr=103;      //valor para conseguir los 9600 baudios
    Tm_Inicie_periodico (&sondeoADC,TIEMPOADC);           // iniciar periodico de ADC
    Tm_Inicie_periodico (&sondeoDisplay,TIEMPODISPLAY);   // iniciar periodico de Display
    Su_inicie_uart(ubrr);     //inicializa el uart
    T_inicie_timer();                  //inicializa los timers 0 y 1
    DyC_inicialice_ADC();   //inicializa el ADC

    for(;;)
    {
        //loop(..);
        if(banderaMili)// supongamos que tenemos un timer por hardware de 1 ms
        {
            banderaMili=0;
            //reseteamos el timer
            Tm_Procese_tiempo (&sondeoADC);
            Tm_Procese_tiempo (&sondeoDisplay);
        }
        
        if(banderaADC)// Interrupción lea ADC
        {
            banderaADC=0;
            DyC_Procese_ADC(SALIDA, &ir, &sal, &saliir2);
            /*en este lugar leemos el ADC y convertimos el valor leido a 
             grados y a unidades y decenas para el display
            */
        }
        
        if(banderaSerial)// condicion de Display entra cada 
        //TIEMPODISPLAY 200 milisegundos
        {
            banderaSerial=0;
            Su_Transmision(&tempUnidades,&bandera_tx,&tempDecenas);
        }
        //if (/*apague el timer tyt*/)
        //    Tm_Termine_periodico (&tyt);
    }
// Nunca deberia llegar aquí
}

ISR(TIMER1_COMPA_vect){
	//se habilita la transmisión serial
	banderaSerial=1;
}
ISR(TIMER0_COMPA_vect){
	banderaMili=1;
}

ISR(ADC_vect){
    banderaADC=1;
}