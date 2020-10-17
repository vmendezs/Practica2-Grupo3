
#ifdef __cplusplus
extern "C" {
#include "funciones.h"
#include "nuestrostimers.h"
#include "definiciones_y_configuraciones.h"
#include "Timers.h"
} // extern "C"
#endif


/*Definiciones del precompilador*/
#define MEDIRTIEMPOS 0//Cambiar a 1 y abrir el monitor serial "de la grafica no se aprecia mucho ya que la columna del centro está en microsegundos"
#define MOSTRARENTRADA 1// cambiar a 1 para mostrar la entrada
#define PERIODOSENAL 100// numero de muestras antes de resetear la respuesta paso e impulso, subir en sistemas lentos

/*Variables globales de configuración*/
//Si selecciona FIR# y IMPULSO o PASO use ENTERA8BITS en la funcion inicializarGenerador para suponer  una entrada con punto fijo a la izquierda del bit 8
//Si selecciona IIR  y IMPULSO o PASO  use UNITARIA en la funcion inicializarGenerador
#define SALIDA IIR // Seleccione FIR1,FIR2,FIR3,FIR_OPT o IIR ver funciones.h
const long interval = 10;           // Intervalo a medir periodico en milisegundos 100Hz=10ms esto asegura una simulación de un tiempo de muestreo


/*****Variables globales del filtro IIR*****/
coef_iir_2_ord ir1, ir2;/*esta estructura debe ser global y se va a pasar por referencia a la funcion que la inicializa*/
Tm_Periodico sondeoADC,sondeoDisplay;

/*Variables especificas del ejemplo:*/
long sal;//para FIR
float saliir1, saliir2;// para IIR
volatile int walk;
volatile int cont;
unsigned long previousMillis;
unsigned long ts1, ts2;
int banderaADC = 0;
int banderaMili = 0;
int banderainicio=1;


void setup() {
  cont = 0;
  walk = 0;
  previousMillis = 0;
  pinMode(LED_BUILTIN, OUTPUT); //LED_BUILTIN

  Serial.begin(1000000);/*velocidad maxima para que no interfiera en tiempos 1000000*/
  while (!Serial) {
    ; // Esperar a que el puerto inicie
  }
  previousMillis = 0;
  /*/Coeficientes IIR PASA-BANDAS Fs=100 y corte en 25Hz (Entre 22Hz y 28Hz) @ 3dB
  float num_Section1[3] = { 1, 0, 1}; //Numerador Seccion 1
  float den_Section1[3] = {1, 0.2381868958, 0.7661067247}; //Denominador Seccion 1
  float num_Section2[3] = { 1, 0, 1}; //Numerador Seccion 2
  float den_Section2[3] = {1, -0.2381868958, 0.7661067247}; //Denominador Seccion 2
  float w1[3] = {0.8749856949, 0, 0};// Ganancia Seccion 1
  float w2[3] = {0.8749856949, 0, 0};// Ganancia Seccion 2inicializar_iir_2_ord(num, den, w, &ir);/*

  /*Coeficientes IIR PASO-BAJOS y IIR PASO-ALTOS Fs=100 y corte en 10Hz @ 3dB
   float num_Section1[3] = { 1, 2, 1}; //Numerador Seccion 1
   float den_Section1[3] = {1, -1.320913434, 0.6327387691}; //Denominador Seccion 1
   float num_Section2[3] = { 1, 2, 1}; //Numerador Seccion 2
   float den_Section2[3] = {1, -1.048599601, 0.296140343}; //Denominador Seccion 2
   float w1[3] = {0.07795634121, 0, 0};// Ganancia Seccion 1
   float w2[3] = {0.07795634121, 0, 0};// Ganancia Seccion 1*/

  /*INICIALIZAR LAS DOS SECCIONES DE IIR*/
  //inicializar_iir_2_ord(num_Section1, den_Section1, w1, &ir1);
  //inicializar_iir_2_ord(num_Section2, den_Section2, w2, &ir2);

  if (MOSTRARENTRADA)
  {
    Serial.print("Entrada");
    Serial.print(" ");
  }
  if (MEDIRTIEMPOS)
  {
    Serial.print("Tiempo_us");
    Serial.print(" ");
  }
  Serial.println("Salida");

  T_inicie_timer();
  DyC_inicialice_ADC();
}
ISR(TIMER0_COMPA_vect){
  banderaMili=1;
}

ISR(ADC_vect){
  banderaADC=1;
}

void loop() {
  if(banderaADC)// Interrupción lea ADC
  {
    banderaADC=0;
    //DyC_Procese_ADC(SALIDA, &ir1, &ir2, &sal, &saliir2);
    /*en este lugar leemos el ADC y convertimos el valor leido a 
     grados y a unidades y decenas para el display
    */
    short adcval= ADC;
    DyC_Filtrar(SALIDA, adcval, &ir1, &ir2, &sal, &saliir2);
   // short adcval;
    //adcval = analogRead(A0)
    Serial.print(adcval);
    Serial.print(" ");
    Serial.println(saliir2,4);
   
  }
}
/***
        ___   ___   _  _
       | __| |_ _| | \| |
       | _|   | |  | .` |
       |_|   |___| |_|\_|

*/
